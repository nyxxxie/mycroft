#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QTextEdit>
#include <QSplitter>
#include <QVBoxLayout>
#include <QWidget>
#include <QStatusBar>
#include <QMenuBar>
#include <QAction>
#include <mycroft/mycroft.h>
#include "maineditor.h"

class MainWindow : public QMainWindow {

    Q_OBJECT

    mc_ctx_t* ctx;

    QWidget* window;

    /* Main elements */
    MainEditor* editor;
    QStatusBar* status_bar;
    QMenuBar* menu_bar;

    /* Actions */
    QAction* act_file_new;
    QAction* act_file_open;
    QAction* act_file_save;
    QAction* act_file_saveas;
    QAction* act_file_exit;
    QAction* act_edit_undo;
    QAction* act_edit_redo;
    QAction* act_edit_cut;
    QAction* act_edit_copy;
    QAction* act_edit_paste;
    QAction* act_help_about;

    void CreateMenuBar();
    void CreateEditor();
    void CreateStatusBar();
    void CreateActions();

public:
    MainWindow(QWidget* parent = NULL);
    ~MainWindow();

    void setMycroftCtx(mc_ctx_t* ctx);

protected:
    void contextMenuEvent(QContextMenuEvent* event) Q_DECL_OVERRIDE;

private slots:
    void on_actionExit_triggered();

};

#endif // MAINWINDOW_H
