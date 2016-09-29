#ifndef MYCROFT_HOOK_H
#define MYCROFT_HOOK_H

#ifdef __cplusplus
extern "C" {
#endif

/** Indicates the type of a callback function. */
typedef enum {
    UNKNOWN = 0,
    C,
    PYTHON
} mc_cb_type_t;

/** */
typedef unsigned int mc_hook_id_t;

/**
 * Stores a hook callback function.  Since callback functions can be both
 * python objects or c functions, we can't just store a pointer to a function.
 * This struct contains both the pointer of the callback handler and also the
 * type of handler, which indicates to hook_dispatch as to how to handle it.
 */
typedef struct {
    void*        func;
    mc_cb_type_t func_type;
} mc_hook_cb_t;

/**
 * Contains the data passed to a hook callback.  Data will be freed after the
 * callback returns, so don't try and use this struct or any data in it
 * after it is freed.  If you want to use it later outside of the callback,
 * make a copy.
 */
typedef struct {
    int   hook_id;
    void* argv;
    int   argc;
} mc_hook_data_t;

/** Expected type of a C callback function. */
typedef void (*mc_hook_cbfunc_t)(mc_hook_data_t args);


int hook_init();
int hook_free();

int hook_event_create(mc_hook_id_t hook_id);
int hook_event_remove(mc_hook_id_t hook_id);

int hook_add(mc_hook_id_t hook_id, const char* hook_name, mc_hook_cb_t* callback);
int hook_remove(mc_hook_id_t hook_id, const char* hook_name);

int hook_dispatch(mc_hook_id_t hook_id, mc_hook_data_t* args);

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_HOOK_H
