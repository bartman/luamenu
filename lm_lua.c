#include "lm_lua.h"
#include "lm_util.h"

#include <lualib.h>
#include <lauxlib.h>
#include <stdarg.h>

// ------------------------------------------------------------------------
// local helpers

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
	lua_pushstring(L, text);
	rc = lua_pcall(L, 1, 0, 0);
	if (rc)
		lm_lua_error(L, "failed to handle argument '%s'", text);

	// TODO: call arg() in lua context, fail if not available

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

	the_lua_state = lua_open();
	luaopen_base(the_lua_state);

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
