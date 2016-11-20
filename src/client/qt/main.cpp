#ifdef __linux__
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#include <stdlib.h>
#include <QApplication>
#include <mycroft/context.h>
#include <mycroft/plugin.h>
#include "mycroft.h"

int main(int argc, char *argv[])
{
    mc_ctx_t* ctx = NULL;

    /* Open qt application */
    QApplication a(argc, argv);

    /* Create mycroft context */
    ctx = mc_ctx_create();
    if (ctx == NULL) {
        fprintf(stderr, "Failed to create mycroft ctx, editing...\n");
        return 1;
    }

    /* Add plugin locations to path */
    #ifdef __linux__
    MC_DEBUG("Loading plugins as Linux.\n");
    const char* plugin_paths[] = { // TODO: use XDG vars directly
        "~/.config/mycroft/plugins",
        "/etc/xdg/mycroft"
    };

    for (int i = 0; i < 2; i++) {
        struct stat st = {0};
        if (stat(plugin_paths[i], &st) < 0) {
            mc_plugin_addpath(ctx, plugin_paths[i]);
        }
    }

    if (mc_plugin_load_plugins(ctx) < 0) {
        return 1;
    }
    //#elif _WIN32
    //
    #else
    #error "Your OS is not supported."
    #endif

    /* */
    Mycroft w(ctx);
    w.show();

    /* Run all plugins in plugin directory */

    return a.exec();
}
