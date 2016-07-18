#include <QSizePolicy>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {

    resize(800,600);

    ctx = NULL;

    this->editor = new MainEditor(this);
    setCentralWidget(editor);
}

MainWindow::~MainWindow() {
    delete main_layout;
}

void MainWindow::on_actionExit_triggered() {
    this->close();
}

void MainWindow::InitFilebrowser() {
    this->filebrowser = new QTextEdit();
    this->filebrowser->setText("filebrowser");
}

void MainWindow::InitTools() {
    this->tools = new QTextEdit();
    this->tools->setText("tools");
}

void MainWindow::setMycroftCtx(mc_ctx_t* ctx) {
    this->ctx = ctx;
    this->editor->setMycroftCtx(ctx);
}
