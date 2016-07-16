#include <stdio.h>
#include <QApplication>
#include <QCommandLineParser>
#include <mycroft/mycroft.h>
#include "mainwindow.h"

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);

    /* Set up command line argument parser */
    QCommandLineParser parser;
    parser.setApplicationDescription("qt client for mycroft, a framework for rapid file format reverse engineering.");
    parser.addHelpOption();
    parser.addVersionOption();

    /* Add command line arguments */
    parser.addPositionalArgument("target", "File to play with.");

    /* Process parser arguments */
    parser.process(a);
    const QStringList args = parser.positionalArguments();

    QString filename = args.at(0);

    /* Check if we were given a file */
    if (filename.size() == 0) {
        printf("You didn't specify a file to open, please do that.\n");
        return 1;
    }

    printf("File: %s\n", filename.toStdString().c_str());

    /* Create mycroft instance */

    /* Create main window */
    MainWindow w;
    w.show();

    return a.exec();
}
