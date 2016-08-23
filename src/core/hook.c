#include <stdint.h>
#include <stdlib.h>
#include "hook.h"

int hook_init() {

    int rc = 0;

    for (size_t i; i < MC_HOOK_BUILTIN_TOTAL; i++) {
        rc = hook_event_create(default_hooks[i]);
        if (rc < 0) {
            return rc;
        }
    }

    return 0;
}

int hook_close() {

}

int hook_event_create(const char* hook_name) {

}

int hook_event_remove(const char* hook_name) {

}

int hook_add(const char* hook_name, const char* hook_id, mc_hook_cb_t* callback) {

}

int hook_remove(const char* hook_name, const char* hook_id) {

}

int hook_dispatch(const char* hook_name, mc_hook_data_t* args) {

}
