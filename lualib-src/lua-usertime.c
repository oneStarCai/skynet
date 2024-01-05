#include <lua.h>
#include <lauxlib.h>

#include <unistd.h>
#include <sys/time.h>

long gettimestamp() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec*1000 + tv.tv_usec/1000);
}

static int lgetmillisecond(lua_State* L) {
    long timestamp = gettimestamp();
    lua_pushnumber(L, timestamp);
    return 1;
}

int luaopen_usertime(lua_State* L) {
    luaL_checkversion(L);
    luaL_Reg l[] = {
        { "getmillisecond", lgetmillisecond },
        { NULL, NULL },
    };
    luaL_newlib(L, l);
    return 1;
}
