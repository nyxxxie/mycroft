#ifndef MYCROFT_INT_HOOK_H
#define MYCROFT_INT_HOOK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <mycroft/hook.h>

/**
 * Represents a hook reciever.  A hook reciever contains a reciever registered
 * by hook_add to listen for a specific event.
 */
typedef struct mc_hook_reciever_t mc_hook_reciever_t;
struct mc_hook_reciever_t {
    char* name;     /**< @internal String that identifies the hook */
    void* callback; /**< @internal */
    mc_hook_reciever_t* prev; /**< @internal */
    mc_hook_reciever_t* next; /**< @internal */
};

/**
 * Represents a hook.
 */
typedef struct mc_hook_t mc_hook_t;
struct mc_hook_t {
    mc_hook_reciever_t* recievers; /**< @internal */
    mc_hook_id_t id;   /**< @internal */
    mc_hook_t*   prev; /**< @internal */
    mc_hook_t*   next; /**< @internal */
};

/**
 * @internal
 * Start to linked list containing all hooks.
 */
static mc_hook_t* mc_hooks;

/**
 * @internal
 * Built-in hooks.
 */
const char* default_hooks[] = {
    "INIT_PRE",
    "INIT_POST"
};
#define MC_HOOK_BUILTIN_TOTAL sizeof(default_hooks)/sizeof(*default_hooks)

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_INT_HOOK_H