#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <QApplication>
#include <QCommandLineParser>
#include <mycroft/mycroft.h>
#include "mainwindow.h"

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);

    /* Seed RNG */
    srand(time(NULL));

    /* Set up command line argument parser */
    QCommandLineParser parser;
    parser.setApplicationDescription("qt client for mycroft, a framework for rapid file format reverse engineering.");
    parser.addHelpOption();
    parser.addVersionOption();

    /* Add command line arguments */
    //parser.addPositionalArgument("target", "File to play with.");
    parser.addPositionalArgument("project", "Project to load.");

    /* Create mycroft instance */
    mc_ctx_t* ctx = mc_ctx_create();
    if (ctx == NULL) {
        printf("Failed to instantiate mycroft context.\n");
        return 1;
    }

    /* Parse arguments */
    parser.process(a);
    QString arg_project = parser.value("project");

    /* Check if we were provided a project file to open */
    mc_project_t* project = NULL;
    if (arg_project.size() > 0) {
        project = mc_project_load(arg_project.toUtf8().data());
        if (project == NULL) {
            printf("Failed to open project \"%s\".\n", arg_project.toUtf8().data());
            return 1;
        }
    } else {
        project = mc_project_create("dumb_project"); // TODO: figure out how to name/rename this.
        if (project == NULL) {
            printf("Failed to open project \"%s\".\n", arg_project.toUtf8().data());
            return 1;
        }

        // Set project name and etc
    }
    mc_ctx_add_project(ctx, project); // Add project to ctx

    /* Read files */
    const QStringList args = parser.positionalArguments();
    if (args.size() > 0) {
        QString filename = args.at(0);

        /* Check to see if we were given an empty filename */
        if (filename.size() == 0) {
            printf("File to open was null.");
            return 1;
        }

        /* Try opening the file */
        mc_file_t* file = mc_file_open(filename.toUtf8().data());
        if (file == NULL) {
            printf("Failed to open file.\n");
            return 1;
        }

        /* Add file to project */
        mc_project_add_file(project, file);
    }

    /* Create main window */
    MainWindow w(ctx);
    w.show();

    return a.exec();
}
