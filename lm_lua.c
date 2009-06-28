#include "lm_lua.h"
#include "lm_util.h"

#include <lualib.h>
#include <lauxlib.h>

static lua_State *L = NULL;

const char *lua_status_string(void)
{
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
	int rc;

	L = lua_open();
	luaopen_base(L);

	rc = luaL_dofile(L, file);
	if (rc) lm_die("luamenu: cannot execute lua code: %s\n",
				lua_status_string());

	return 0;
}

int lm_handle_lua_code(const char *code)
{
	int rc;

	L = lua_open();
	luaopen_base(L);

	rc = luaL_dostring(L, code);
	if (rc) lm_die("luamenu: cannot execute lua code: %s\n",
				lua_status_string());

	return 0;
}

void lm_lua_cleanup(void)
{
	if (!L) return;

	lua_close(L);
	L = NULL;
}
