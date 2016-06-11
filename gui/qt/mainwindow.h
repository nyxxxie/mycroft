#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QTextEdit>
#include <QSplitter>
#include <QVBoxLayout>
#include <QWidget>
#include "maineditor.h"

// TODO: edit this to just host tabs.  Maybe also modify it to be the initialwindow as well?  (start off small for dragging files and etc, then expand to the hex edit tab view)

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QWidget* window;
    QVBoxLayout* main_layout;
    QWidget* tools_container;
    QVBoxLayout* tools_layout;

    QTextEdit* filebrowser;
    QTextEdit* tools;

    MainEditor* editor;

    void InitFilebrowser();
    void InitTools();

public:
    MainWindow();
    ~MainWindow();

private slots:
    void on_actionExit_triggered();
};

#endif // MAINWINDOW_H
