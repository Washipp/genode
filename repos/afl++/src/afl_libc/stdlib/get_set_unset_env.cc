/* afl_libc includes */
#include "stdlib.h"
#include "sys/null.h"
#include "string.h"


/* Genode includes */
#include <base/log.h>

typedef struct {
    char *name;
    char *value;
} EnvVar;

static EnvVar *afl_env = NULL;
static int env_count = 0;

char *getenv(const char *name) {
    for (int i = 0; i < env_count; i++) {
        if (strcmp(afl_env[i].name, name) == 0) {
            return afl_env[i].value;
        }
    }
    return NULL;
}

int setenv(const char *name, const char *value, int replace) {
    for (int i = 0; i < env_count; i++) {
        if (strcmp(afl_env[i].name, name) == 0) {
            if (replace) {
                free(afl_env[i].value);
                afl_env[i].value = strdup(value);
            }
            return 0;
        }
    }
    if (env_count == 0) {
        afl_env = (EnvVar *) malloc((env_count + 1) * sizeof(EnvVar));
    } else {
        afl_env = (EnvVar *) realloc(afl_env, (env_count + 1) * sizeof(EnvVar));
        if (afl_env == NULL) {
            return -1;
        }
    }
    afl_env[env_count].name = strdup(name);
    afl_env[env_count].value = strdup(value);
    env_count++;
    return 0;
}

int unsetenv(const char *name){
    if (name == NULL || strchr(name, '=') != NULL) {
        return -1;  // Invalid input (NULL or contains '=')
    }

    for (int i = 0; i < env_count; i++) {
        if (strcmp(afl_env[i].name, name) == 0) {
            free(afl_env[i].name);
            free(afl_env[i].value);

            for (int j = i; j < env_count - 1; j++) {
                afl_env[j] = afl_env[j + 1];
            }

            env_count--;

            if (env_count > 0) {
                afl_env = (EnvVar *) realloc(afl_env, env_count * sizeof(EnvVar));
                if (afl_env == NULL) {
                    return -1;
                }
            } else {
                free(afl_env);
                afl_env = NULL;
            }
            return 0;
        }
    }
    return -1;
}

int clear_all_env_vars() {
    for (int i = 0; i < env_count; i++) {
        free(afl_env[i].name);
        free(afl_env[i].value);
    }
    free(afl_env);
    afl_env = NULL;
    return 0;
}