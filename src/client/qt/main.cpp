/*****************************************************************************\
Copyright (C) 2016-2017 Nyxxie <github.com/nyxxxie>

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.
This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 51 Franklin
Street, Fifth Floor, Boston, MA  02110-1301, USA.
\*****************************************************************************/

#include <stdlib.h>
#include <QApplication>
#include <QCommandLineParser>
#include <mycroft/mycroft.h>
#include <mycroft/plugin.h>
#include <mycroft/script.h>
#include <mycroft/template.h>
#include "mycroft.h"
#include "config.h"

int main(int argc, char *argv[])
{
    const char* python_path[] = {
        "~/.config/mycroft/plugins",
    };
    mc_project_t* project = NULL;
    QApplication a(argc, argv);
    Mycroft w;

    /* Create command line parser */
    QCommandLineParser args;
    args.setApplicationDescription("A file format reverse engineering and rapid prototyping tool");
    args.addHelpOption();
    args.addVersionOption();

    args.addPositionalArgument("file", "File to examine.", "[file]...");

    args.addOptions({
        // NAME             DESCRIPTION                                    VALUE NAME    DEFAULT_VALUE
        {{"p", "project"},  "Mycroft project to open.",                    "project",    ""},
        {"plugdir",         "Additional directory to search for plugins.", "directory",  ""},
        {"testprojects",    "Create test projects for testing."},
        {"testfile",        "Create test file ."},
    });
    args.process(a);

    /* Create mycroft context */
    if (mc_init() != MC_OK) {
        fprintf(stderr, "Failed to initialize mycroft...\n");
        return 1;
    }

    /* Initialize scripting and plugins */
    if (mc_init_scripting(python_path, 1) != MC_OK) {
        fprintf(stderr, "Failed to initialize mycroft...\n");
        return 1;
    }

    /* Create main gui window */

    /* Open project and add it to the ctx */
    QString project_path = args.value("project");
    if (project_path.size() > 0) {
        const char* path = project_path.toStdString().c_str();
        MC_DEBUG("Loading project: \"%s\".\n", path);
        project = mc_project_load(path);
    } else {
        MC_DEBUG("No project specified.\n");
        project = mc_project_create("default");
    }

    if (project == NULL) {
        printf("Failed to create/open project, exiting...\n");
        return 1;
    }

    if (mc_add_project(project) != MC_OK) {
        printf("Failed to add project to context, exiting...\n");
        return 1;
    }

    /* Create test projects (this should be deleted later) */
    if (args.isSet("testprojects")) {
        mc_project_t* projects[] = {
            mc_project_create("project 1"),
            mc_project_create("project 2"),
            mc_project_create("project 3"),
        };

        const char* files[] = {
            "CMakeCache.txt",
            "cmake_install.cmake",
            "Makefile",
        };

        for (mc_project_t* project : projects) {
            for (const char* file : files) {
                mc_project_add_file(project, mc_file_open(file));
            }

            mc_add_project(project);
        }
    }

    /* Create test projects (this should be deleted later) */
    if (args.isSet("testfile")) {
        template_t* t;
        mc_file_t* file;

        /* Open file */
        file = mc_file_open("testfile");
        if (file == NULL) {
            printf("Failed to open test file.\n");
            return 1;
        }

        /* */
        t = template_create_from_file("testfile.mtf");
        if (t == NULL) {
            printf("Failed to open template file.\n");
            return 1;
        }

        /* */
        if (mc_file_set_template(file, t) < 0) {
            printf("Failed to set file template.\n");
            return 1;
        }

        if (!w.addFile(project, file)) {
            printf("Failed to add file to project.\n");
            return 1;
        }
    }

    /* Add file given in options to project, if one was specified */
    QStringList files = args.positionalArguments();
    if (files.isEmpty()) {
        MC_DEBUG("No files given on command line.\n");
    }
    else {
        MC_DEBUG("Files to open:.\n");
    }

    /* Add all files passed via command line to project */
    for (QString file_path : files) {
        if (file_path.size() > 0) {
            mc_file_t* file = NULL;
            const char* path = NULL;

            /* Convert QString to char array */
            QByteArray ba = file_path.toLatin1();
            path = ba.data();
            MC_DEBUG("\tfile: \"%s\".\n", path);

            /* Open file */
            file = mc_file_open(path);
            if (file == NULL) {
                printf("Failed to open file \"%s\", exiting...\n", path);
                return 1;
            }

            /* Add file to project */
            if (!w.addFile(project, file)) {
                printf("Failed to add file \"%s\" to project, exiting....\n", path);
                return 1;
            }
        }
    }

    ///* Create interpreter ctx */
    //interpreter = mc_interpreter_create();
    //if (interpreter == NULL) {
    //    MC_ERROR("Failed to create interpreter.\n");
    //    return 1;
    //}

    ///* Add to interpreter path */
    //mc_interpreter_add_path(interpreter, MYCROFT_INSTALL_SCRIPT_PATH);

    ///* Run init script */
    //mc_interpreter_runinitscript(interpreter);

    ///* Initialize plugin system */
    //if (mc_plugin_init(ctx) < 0) {
    //    printf("Failed to initialize plugin system, exiting...\n");
    //    return 1;
    //}
    //
    //#ifdef __linux__
    //MC_DEBUG("Loading plugins as Linux.\n");
    //const char* plugin_paths[] = { // TODO: use XDG vars directly
    //    "~/.config/mycroft/plugins",
    //    "/etc/xdg/mycroft/plugins"
    //};
    ////#elif _WIN32
    ////
    //#else
    //#error "Your OS is not supported."
    //#endif

    ///* Add plugin locations to path */
    //for (int i = 0; i < 2; i++) {
    //    struct stat st = {0};
    //    if (stat(plugin_paths[i], &st) < 0) {
    //        mc_plugin_addpath(ctx, plugin_paths[i]);
    //    }
    //}

    //if (mc_plugin_run_plugins(ctx) < 0) {
    //    return 1;
    //}

    /* Run all plugins in plugin directory */

    /* Display main window and exec the application */
    w.show();
    return a.exec();
}
