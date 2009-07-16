#include "lm_lua.h"
#include "lm_util.h"

#include <lualib.h>
#include <lauxlib.h>
#include <stdarg.h>
#include <string.h>

// ------------------------------------------------------------------------
// local helpers

static void lm_stack_dump (const char *prefix, lua_State *L);
static lua_State *lm_lua_handle(void);
static void lm_lua_startup(lua_State *L);

// avoid using directly, see lm_lua_handle()
static lua_State *the_lua_state = NULL;

#define lm_lua_error(L,fmt,...) do { \
	const char *__str = lua_tostring(L, -1); \
	lm_die("luamenu: "fmt": %s\n", ##__VA_ARGS__, __str); \
} while(0)


// ------------------------------------------------------------------------
// external API

const char *lm_lua_status_string(void)
{
	lua_State *L = lm_lua_handle();
	int status = lua_status(L);
	static char buffer[128];
	int rc;

	switch (status) {
	case 0:             return "OK";
	case LUA_YIELD:     return "Yield";
	case LUA_ERRRUN:    return "Failed to run";
	case LUA_ERRSYNTAX: return "Syntax error";
	case LUA_ERRMEM:    return "Memory allocation error";
	case LUA_ERRERR:    return "Internal error";
	}

	rc = snprintf(buffer, sizeof(buffer),
			"Unknown error %d", status);
	if (rc < 0)
		return "Unknown error";
	return buffer;
}

void lm_handle_lua_file_arg(const char *file)
{
	lua_State *L = lm_lua_handle();
	if (luaL_dofile(L, file))
		lm_lua_error(L, "cannot execute lua file");
	lm_lua_startup(L);
}

void lm_handle_lua_code_arg(const char *code)
{
	lua_State *L = lm_lua_handle();
	if (luaL_dostring(L, code))
		lm_lua_error(L, "cannot execute lua code");
	lm_lua_startup(L);
}

void lm_handle_lua_arg_arg(const char *text)
{
	int rc;
	lua_State *L;

	if (!the_lua_state)
		lm_die("luamenu: arguments to lua script need to be passed "
			"after the luascript\n");

	L = lm_lua_handle();
	lua_getglobal(L, "arg");
	if (!lua_isfunction(L, -1))
		lm_die("luamenu: the lua context does not have an arg() function\n");

	lua_pushstring(L, text);
	rc = lua_pcall(L, 1, 0, 0);
	if (rc)
		lm_lua_error(L, "failed to handle argument '%s'", text);

	lua_pop(L, -1);

	return;
}

bool lm_is_lua_running(void)
{
	return the_lua_state != NULL;
}

// ------------------------------------------------------------------------
// handle lua state

static lua_State *lm_lua_handle(void)
{
	if (the_lua_state)
		return the_lua_state;

	the_lua_state = luaL_newstate();
	luaL_openlibs(the_lua_state);

	// TODO: export various API for the lua script to manipulate our state
	//        - override configuration
	//        - seed initial list of commands
	//        - bind keys to lua handler
	//        ... etc.

	return the_lua_state;
}

static void lm_lua_startup(lua_State *L)
{
	// TODO: call init() in lua context, if present

	return;
}

void lm_lua_cleanup(void)
{
	if (!the_lua_state)
		return;

	// TODO: call cleanup() in lua context, if present

	lua_close(the_lua_state);
	the_lua_state = NULL;
}

// ------------------------------------------------------------------------

static int lm_stack_sprint_value (char *buffer, int blen, lua_State *L, int idx)
{
	int rc, t = lua_type(L, idx);

	if (!buffer || blen<0)
		return -1;
	*buffer = 0;

	switch (t) {
	case LUA_TNIL: /* nothing */
		rc = snprintf(buffer, blen, "(NIL)");
		break;

	case LUA_TSTRING:  /* strings */
		rc = snprintf(buffer, blen, "`%s'",
				lua_tostring(L, idx));
		break;

	case LUA_TBOOLEAN:  /* booleans */
		rc = snprintf(buffer, blen, "%s",
				lua_toboolean(L, idx) ? "TRUE" : "FALSE");
		break;

	case LUA_TNUMBER:  /* numbers */
		rc = snprintf(buffer, blen, "%g",
				lua_tonumber(L, idx));
		break;

	case LUA_TTABLE:   /* table */
		rc = snprintf(buffer, blen, "table(%d)",
				luaL_getn(L, idx));
		break;

	default:  /* other values */
		rc = snprintf(buffer, blen, "%s",
				lua_typename(L, t));
		break;
	}

	return rc;
}

#define SD_TABLE_INDENT 16

static void lm_stack_dump_at (const char *prefix, int level, lua_State *L,
		int idx)
{
	char b[128];
	char *p = NULL;
	int prefixlen = strlen(prefix);
	int rc, nlen, len;

	rc = lm_stack_sprint_value(b, sizeof b, L, idx);
	printf("%s - %s\n", prefix, b);

	if (lua_type(L, idx) == LUA_TTABLE) {
		/* table is in the stack at index 'idx' */
		lua_pushnil(L);  /* first key */
		while (lua_next(L, idx) != 0) {
			/* uses 'key' (at index -2) and 'value' (at index -1) */
			len = lm_stack_sprint_value(b, sizeof b, L, -2);

			nlen = len + prefixlen + 16 + SD_TABLE_INDENT;
			p = realloc(p, nlen);
			rc = snprintf(p, nlen, "%s   [%s]%*s",
					prefix, b,
					len>=SD_TABLE_INDENT ? 0
					: SD_TABLE_INDENT - len, "");
			lm_stack_dump_at(p, level+1, L, -1);

			/* removes 'value'; keeps 'key' for next iteration */
			lua_pop(L, 1);
		}
	}

	free(p);
}

static void lm_stack_dump (const char *prefix, lua_State *L)
{
	int idx;
	int top = lua_gettop(L);

	fflush (stdout);
	fprintf (stderr, "%s--- stack ---\n", prefix);

	for (idx = 1; idx <= top; idx++) {  /* repeat for each level */
		lm_stack_dump_at (prefix, 0, L, idx);
	}

	fprintf (stderr, "%s-------------\n", prefix);

	lua_settop(L, top);
}

