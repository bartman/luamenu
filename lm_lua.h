#ifndef LM_LUA_H
#define LM_LUA_H

#include <lua.h>

extern const char *lm_lua_status_string(void);

extern int lm_handle_lua_file(const char *file);
extern int lm_handle_lua_code(const char *code);

extern void lm_lua_cleanup(void);

#endif // LM_LUA_H
