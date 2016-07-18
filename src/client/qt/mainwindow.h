#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QTextEdit>
#include <QSplitter>
#include <QVBoxLayout>
#include <QWidget>
#include <mycroft/mycroft.h>
#include "maineditor.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    mc_ctx_t* ctx;

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
    MainWindow(QWidget* parent = NULL);
    ~MainWindow();

    void setMycroftCtx(mc_ctx_t* ctx);

private slots:
    void on_actionExit_triggered();
};

#endif // MAINWINDOW_H
