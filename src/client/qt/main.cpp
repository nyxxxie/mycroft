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
    parser.addPositionalArgument("target", "File to play with.");

    /* Create mycroft instance */
    mc_ctx_t* ctx = mycroft_init();
    if (ctx == NULL) {
        printf("Failed to instantiate mycroft context.\n");
        return 1;
    }

    /* Process parser arguments */
    parser.process(a);
    const QStringList args = parser.positionalArguments();
    if (args.size() > 0) {
        QString filename = args.at(0);
        if (filename.size() == 0) {
            printf("File to open was null.");
            return 1;
        }

        int rc = mycroft_open_file(ctx, filename.toStdString().c_str());
        if (rc < 0) {
            printf("Failed to open file.\n");
            return 1;
        }
    }

    /* Create main window */
    MainWindow w(ctx);
    w.show();

    return a.exec();
}
