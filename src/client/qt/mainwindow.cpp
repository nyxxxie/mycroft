#include <QSizePolicy>
#include <QContextMenuEvent>
#include "mainwindow.h"

//TODO: move this to a file
#define QUOTE_AMOUNT 3
QString quotes[QUOTE_AMOUNT] = {
    QString("FIRMLY GRASP IT."),
    QString("Where's the leak, Ma'am?"),
    QString("WHAT'S IN THE BOX?!?!"),
};

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {

    resize(800,800);

    ctx = NULL;

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

void MainWindow::on_actionExit_triggered() {
    this->close();
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

    //TODO: add these to the menubar
    act_file_new = new QAction("New", this);

    act_file_open = new QAction("Open", this);

    act_file_save = new QAction("Save", this);

    act_file_saveas = new QAction("Save As", this);

    act_file_exit = new QAction("Exit", this);
    act_file_exit->setShortcuts(QKeySequence::Quit);
    act_file_exit->setStatusTip("Exit Mycroft");
    connect(act_file_exit, &QAction::triggered, this, &QWidget::close);

    act_edit_undo = new QAction("Undo", this);

    act_edit_redo = new QAction("Redo", this);

    act_edit_cut = new QAction("Cut", this);

    act_edit_copy = new QAction("Copy", this);

    act_edit_paste = new QAction("Paste", this);

    act_script_runfile = new QAction("Run File", this);

    act_help_about = new QAction("About", this);
}

void MainWindow::setMycroftCtx(mc_ctx_t* ctx) {
    this->ctx = ctx;
    this->editor->setMycroftCtx(ctx);
}
