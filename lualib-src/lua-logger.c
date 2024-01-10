#include <lua.h>
#include <lauxlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "log.h"

FILE* file = NULL;

char g_szFullName[100] = {0};
const char* g_szLogDir = NULL;
const char* g_szLogName = NULL;

void checklog() {
    time_t t = time(NULL);
    struct tm* time = localtime(&t);
    char buf[64] = {0};
    buf[strftime(buf, sizeof(buf), "%Y-%m-%d", time)] = '\0';
    
    assert(g_szFullName);
    if (strstr(g_szFullName, buf) == NULL) {
        if (file) {
            fclose(file);
        }
       
        assert(g_szLogDir);
        assert(g_szLogName);
        sprintf(g_szFullName, "%s/%s.%s.log", g_szLogDir, g_szLogName, buf);
        file = fopen(g_szFullName, "w+");
        log_update_fp(file);
    }
}

int open_logfile(int logLevel) {
    int ret = -1;
    if (access(g_szLogDir, 0) == -1) {
        int flag = mkdir(g_szLogDir, 0777);
        if (flag == 0) {
            printf("linit mkdir %s success.\n", g_szLogDir);
        }
        else {
            printf("linit mkdir %s error.\n", g_szLogDir);
            return ret;
        }
    }

    time_t t = time(NULL);
    struct tm* time = localtime(&t);
    char buf[64];
    buf[strftime(buf, sizeof(buf), "%Y-%m-%d", time)] = '\0';
    
    sprintf(g_szFullName, "%s/%s.%s.log", g_szLogDir, g_szLogName, buf);

    file = fopen(g_szFullName, "w+");
    if (file == NULL) {
        printf("file name %s open file error.\n", g_szFullName);
    } else {
        printf("loglevel[%d] logfile[%s] create success.\n", logLevel, g_szFullName);
        return log_add_fp(file, logLevel);
    }

    return ret;
}

static int linit(lua_State* L) {
    g_szLogName = lua_tostring(L, -1);
    g_szLogDir = lua_tostring(L, -2);
    
    //int logFlushInterval = lua_tointeger(L, -3);
    //int rollSize = lua_tointeger(L, -4);
    int logLevel = lua_tointeger(L, -5);
    //bool quiet = lua_toboolean(L, -6);

    /*
    if (access(g_szLogDir, 0) == -1) {
        int flag = mkdir(g_szLogDir, 0777);
        if (flag == 0) {
            printf("linit mkdir %s success.\n", g_szLogDir);
        }
        else {
            printf("linit mkdir %s error.\n", g_szLogDir);
        }
    }

    log_set_quiet(true); //quiet

    time_t t = time(NULL);
    struct tm* time = localtime(&t);
    char buf[64];
    buf[strftime(buf, sizeof(buf), "%Y-%m-%d", time)] = '\0';

    sprintf(g_szFullName, "%s/%s.%s.log", g_szLogDir, g_szLogName, buf);

    file = fopen(g_szFullName, "w+");
    if (file == NULL) {
        printf("file name %s open file error.\n", g_szFullName);
    } else {
        printf("loglevel:%d logfile %s create success.\n", logLevel, g_szFullName);
        log_add_fp(file, logLevel);
    }
    */
    if (open_logfile(logLevel) == 0) {
        log_set_quiet(true); //quiet
    }
    
    return 0;
}

static int lexit(lua_State* L) {
    if (file) {
        fclose(file);
    }
    return 0;
}

static int ltrace(lua_State* L) {
    checklog();
    const char* fmtstr = lua_tostring(L, -1);
    //const char* strName = lua_tostring(L, -2);
    //const char* strTime = lua_tostring(L, -3);
    log_trace(fmtstr);
    return 0;
}

static int ldebug(lua_State* L) {
    checklog();
    const char* fmtstr = lua_tostring(L, -1);
    //const char* strName = lua_tostring(L, -2);
    //const char* strTime = lua_tostring(L, -3);
    log_debug(fmtstr);
    return 0;
}

static int linfo(lua_State* L) {
    checklog();
    const char* fmtstr = lua_tostring(L, -1);
    //const char* strName = lua_tostring(L, -2);
    //const char* strTime = lua_tostring(L, -3);
    log_info(fmtstr);
    return 0;
}

static int lwarning(lua_State* L) {
    checklog();
    const char* fmtstr = lua_tostring(L, -1);
    //const char* strName = lua_tostring(L, -2);
    //const char* strTime = lua_tostring(L, -3);
    log_warn(fmtstr);
    return 0;
}

static int lerror(lua_State* L) {
    checklog();
    const char* fmtstr = lua_tostring(L, -1);
    //const char* strName = lua_tostring(L, -2);
    //const char* strTime = lua_tostring(L, -3);
    log_error(fmtstr);
    return 0;
}

static int lfatal(lua_State* L) {
    checklog();
    const char* fmtstr = lua_tostring(L, -1);
    //const char* strName = lua_tostring(L, -2);
    //const char* strTime = lua_tostring(L, -3);
    log_fatal(fmtstr);
    return 0;
}

int luaopen_log_core(lua_State* L) {
    luaL_checkversion(L);
    luaL_Reg l[] = {
        {"init", linit},
        {"exit", lexit},
        {"trace", ltrace},
        {"debug", ldebug},
        {"info", linfo},
        {"warning", lwarning},
        {"error", lerror},
        {"fatal", lfatal},
        {NULL, NULL},
    };
    luaL_newlib(L, l);
    return 1;
}
