#include <QFileDialog>
#include <QSizePolicy>
#include <QContextMenuEvent>
#include <mycroft/plugin.h>
#include "mainwindow.h"

//TODO: move this to a file
#define QUOTE_AMOUNT 3
QString quotes[QUOTE_AMOUNT] = {
    QString("FIRMLY GRASP IT."),
    QString("Where's the leak, Ma'am?"),
    QString("WHAT'S IN THE BOX?!?!"),
};

MainWindow::MainWindow(mc_ctx_t* ctx) : QMainWindow() {

    resize(800,800);

    this->ctx = ctx;

    /* Create widget to place all of the main elements in */
    window = new QWidget();
    setCentralWidget(window);

    /* Create the main elements */
    CreateActions();
    CreateMenuBar();
    CreateEditor();
    CreateStatusBar();

    /* Add the elements to a layout and set it */
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setMargin(0);
    layout->setMenuBar(menu_bar);
    layout->addWidget(editor);
    layout->addWidget(status_bar);
    window->setLayout(layout);

    /* Print the initial message */
    status_bar->showMessage(quotes[rand() % QUOTE_AMOUNT]);
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event) {
    QMenu menu(this);
    menu.addAction(act_edit_cut);
    menu.addAction(act_edit_copy);
    menu.addAction(act_edit_paste);
    menu.exec(event->globalPos());
}

MainWindow::~MainWindow() {
    //TODO: cleanup
}

void MainWindow::CreateMenuBar() {

    /* Create menu bar object */
    menu_bar = new QMenuBar();

    /* File menu */
    QMenu* menu1 = menu_bar->addMenu("File");
    menu1->addAction(act_file_new);
    menu1->addAction(act_file_open);
    menu1->addAction(act_file_save);
    menu1->addAction(act_file_saveas);
    menu1->addSeparator();
    menu1->addAction(act_file_exit);

    /* Edit menu */
    QMenu* menu2 = menu_bar->addMenu("Edit");
    menu2->addAction(act_edit_undo);
    menu2->addAction(act_edit_redo);
    menu2->addAction(act_edit_cut);
    menu2->addAction(act_edit_copy);
    menu2->addAction(act_edit_paste);

    /* Script menu */
    QMenu* menu3 = menu_bar->addMenu("Script");
    menu3->addAction(act_script_runfile);

    /* Help menu */
    QMenu* menu4 = menu_bar->addMenu("Help");
    menu4->addAction(act_help_about);
}

void MainWindow::CreateEditor() {
    editor = new MainEditor(this);
}

void MainWindow::CreateStatusBar() {
    status_bar = new QStatusBar();
}

void MainWindow::CreateActions() {

    act_file_new = new QAction("New", this);
    connect(act_file_new, &QAction::triggered,
            this, &MainWindow::on_act_file_new_triggered);

    act_file_open = new QAction("Open", this);
    connect(act_file_open, &QAction::triggered,
            this, &MainWindow::on_act_file_open_triggered);

    act_file_save = new QAction("Save", this);
    connect(act_file_save, &QAction::triggered,
            this, &MainWindow::on_act_file_save_triggered);

    act_file_saveas = new QAction("Save As", this);
    connect(act_file_saveas, &QAction::triggered,
            this, &MainWindow::on_act_file_saveas_triggered);

    act_file_exit = new QAction("Exit", this);
    act_file_exit->setShortcuts(QKeySequence::Quit);
    act_file_exit->setStatusTip("Exit Mycroft");
    connect(act_file_exit, &QAction::triggered,
            this, &MainWindow::on_act_file_exit_triggered);

    act_edit_undo = new QAction("Undo", this);
    connect(act_edit_undo, &QAction::triggered,
            this, &MainWindow::on_act_edit_undo_triggered);

    act_edit_redo = new QAction("Redo", this);
    connect(act_edit_redo, &QAction::triggered,
            this, &MainWindow::on_act_edit_redo_triggered);

    act_edit_cut = new QAction("Cut", this);
    connect(act_edit_cut, &QAction::triggered,
            this, &MainWindow::on_act_edit_cut_triggered);

    act_edit_copy = new QAction("Copy", this);
    connect(act_edit_copy, &QAction::triggered,
            this, &MainWindow::on_act_edit_copy_triggered);

    act_edit_paste = new QAction("Paste", this);
    connect(act_edit_paste, &QAction::triggered,
            this, &MainWindow::on_act_edit_paste_triggered);

    act_script_runfile = new QAction("Run File", this);
    connect(act_script_runfile, &QAction::triggered,
            this, &MainWindow::on_act_script_runfile_triggered);

    act_help_about = new QAction("About", this);
    connect(act_help_about, &QAction::triggered,
            this, &MainWindow::on_act_help_about_triggered);
}

void MainWindow::setMycroftCtx(mc_ctx_t* ctx) {
    this->ctx = ctx;
    this->editor->setMycroftCtx(ctx);
}

void MainWindow::on_act_file_new_triggered() {
    printf("on_act_file_new_triggered\n");
}

void MainWindow::on_act_file_open_triggered() {
    printf("on_act_file_open_triggered\n");
}

void MainWindow::on_act_file_save_triggered() {
    printf("on_act_file_save_triggered\n");
}

void MainWindow::on_act_file_saveas_triggered() {
    printf("on_act_file_saveas_triggered\n");
}

void MainWindow::on_act_file_exit_triggered() {
    printf("on_act_file_exit_triggered\n");
    this->close();
}

void MainWindow::on_act_edit_undo_triggered() {
    printf("on_act_edit_undo_triggered\n");
}

void MainWindow::on_act_edit_redo_triggered() {
    printf("on_act_edit_redo_triggered\n");
}

void MainWindow::on_act_edit_cut_triggered() {
    printf("on_act_edit_cut_triggered\n");
}

void MainWindow::on_act_edit_copy_triggered() {
    printf("on_act_edit_copy_triggered\n");
}

void MainWindow::on_act_edit_paste_triggered() {
    printf("on_act_edit_paste_triggered\n");
}

void MainWindow::on_act_script_runfile_triggered() {

    QString filters("Python Scripts (*.py);;All files (*.*)");
    QString defaultFilter("Python Scripts (*.py)");

    /* Static method approach */
    QString file = QFileDialog::getOpenFileName(
        NULL,
        "Open Script File",
        QDir::currentPath(),
        filters,
        &defaultFilter);

    printf("Running script: \"%s\"\n", file.toStdString().c_str());
    int rc = mc_plugin_runfile(file.toStdString().c_str());
    if (rc < 0) {
        printf("Failed to run script!\n");
    }
}

void MainWindow::on_act_help_about_triggered() {
    printf("on_act_help_about_triggered\n");
}
