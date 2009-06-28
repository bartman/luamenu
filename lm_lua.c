#include "lm_lua.h"
#include "lm_util.h"

#include <lualib.h>
#include <lauxlib.h>

// ------------------------------------------------------------------------
// local helpers

static lua_State *lm_lua_handle(void);
static int lm_lua_startup(lua_State *L, int load_status);

// ------------------------------------------------------------------------
// external API

const char *lua_status_string(void)
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

int lm_handle_lua_file(const char *file)
{
	lua_State *L = lm_lua_handle();
	int rc = luaL_dofile(L, file);
	return lm_lua_startup(L, rc);
}

int lm_handle_lua_code(const char *code)
{
	lua_State *L = lm_lua_handle();
	int rc = luaL_dostring(L, code);
	return lm_lua_startup(L, rc);
}

// ------------------------------------------------------------------------
// handle lua state

static lua_State *the_lua_state = NULL;

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

static int lm_lua_startup(lua_State *L, int load_status)
{
	if (load_status)
		lm_die("luamenu: cannot execute lua code: %s\n",
				lua_status_string());

	// TODO: call init() in lua context, if present

	return 0;
}

void lm_lua_cleanup(void)
{
	if (!the_lua_state)
		return;

	// TODO: call cleanup() in lua context, if present

	lua_close(the_lua_state);
	the_lua_state = NULL;
}
