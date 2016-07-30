#ifndef MYCROFT_PLUGIN_H
#define MYCROFT_PLUGIN_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct mc_plugin_entry_t mc_plugin_entry_t;

int mc_plugin_init();
int mc_plugin_close();

int mc_plugin_addpath(const char* path);

int mc_plugin_runscript(const char* script);
int mc_plugin_runfile(const char* script);

int mc_plugin_load(const char* path);
int mc_plugin_unload(const char* path);
int mc_plugin_getloaded(const char* path);

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_PLUGIN_H
