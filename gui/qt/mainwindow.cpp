#include <QSizePolicy>
#include "mainwindow.h"

MainWindow::MainWindow() : QMainWindow() {

    InitFilebrowser();
    InitTools();
    InitHexEditor();
    InitTemplateEditor();

    /* Create the left container */
    QSplitter* split1 = new QSplitter;
    split1->setOrientation(Qt::Vertical);
    split1->addWidget(filebrowser);
    split1->addWidget(tools);

    tools_layout = new QVBoxLayout();
    tools_layout->setContentsMargins(0,0,0,0);
    tools_layout->addWidget(split1);

    tools_container = new QWidget();
    tools_container->setLayout(tools_layout);

    /* Create the right container */
    QSplitter* split2 = new QSplitter;
    split2->setOrientation(Qt::Vertical);
    split2->addWidget(hexedit);
    split2->addWidget(templateedit);

    editor_layout = new QVBoxLayout();
    editor_layout->setContentsMargins(0,0,0,0);
    editor_layout->addWidget(split2);

    editor_container = new QWidget();
    editor_container->setLayout(editor_layout);

    /* Place both containers in another central layout */
    QSplitter* split3 = new QSplitter;
    split3->addWidget(tools_container);
    split3->addWidget(editor_container);

    main_layout = new QVBoxLayout;
    main_layout->setContentsMargins(0,0,0,0);
    main_layout->addWidget(split3);

    /* Create main window */
    window = new QWidget();

    QSizePolicy policy = window->sizePolicy();
    policy.setHorizontalStretch(0);
    policy.setVerticalStretch(0);

    window->setLayout(main_layout);
    window->setSizePolicy(policy);
    setCentralWidget(window);

    resize(800,600);
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

void MainWindow::InitHexEditor() {
    hexedit = new QTextEdit();
    hexedit->setText("hexedit");
}

void MainWindow::InitTemplateEditor() {
    templateedit = new QTextEdit();
    templateedit->setText("templateedit");
}
