#ifndef LM_LUA_H
#define LM_LUA_H

#include <lua.h>
#include <stdbool.h>

extern const char *lm_lua_status_string(void);

extern void lm_handle_lua_file_arg(const char *file);
extern void lm_handle_lua_code_arg(const char *code);
extern void lm_handle_lua_arg_arg(const char *text);

extern bool lm_is_lua_running(void);

extern void lm_lua_cleanup(void);

#endif // LM_LUA_H
