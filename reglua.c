#include <lauxlib.h>
#include <lua.h>
#include <regex.h>
#include <stdlib.h>

static int rmatch(lua_State *L) {
    int args = 0;
    const char *pattern = luaL_checkstring(L, 1);
    const char *value = luaL_checkstring(L, 2);

    regex_t regex;
    if (regcomp(&regex, pattern, REG_EXTENDED)) {
        luaL_error(L, "Failed to compile regex.");
    }

    int n = regex.re_nsub + 1;
    regmatch_t *matches = malloc(n * sizeof(regmatch_t));
    if (!matches) {
        luaL_error(L, "Failed to allocate memory for regex matches.");
    }

    const char *offset = value;
    while (!regexec(&regex, offset, n, matches, 0)) {
        size_t start = matches[0].rm_so + (offset - value);
        size_t end = matches[0].rm_eo + (offset - value);
        size_t len = end - start;

        lua_pushlstring(L, value + start, len);
        offset += matches[0].rm_eo;
        args++;
    }

    if (args == 0) {
        lua_pushnil(L);
        args++;
    }

    free(matches);
    regfree(&regex);
    return args;
}

int luaopen_reglua(lua_State *L) {
    static const struct luaL_Reg lib[] = {{"rmatch", rmatch}, {NULL, NULL}};
    luaL_register(L, "reglua", lib);
    return 1;
}
