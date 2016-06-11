#include <QSizePolicy>
#include "mainwindow.h"

MainWindow::MainWindow() : QMainWindow() {

    resize(800,600);

    editor = new MainEditor();
    setCentralWidget(editor);
}

MainWindow::~MainWindow() {
    delete main_layout;
}

void MainWindow::on_actionExit_triggered() {
    this->close();
}

void MainWindow::InitFilebrowser() {
    filebrowser = new QTextEdit();
    filebrowser->setText("filebrowser");
}

void MainWindow::InitTools() {
    tools = new QTextEdit();
    tools->setText("tools");
}
