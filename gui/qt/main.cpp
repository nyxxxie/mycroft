#include "mainwindow.h"
#include <QApplication>
#include <QTextEdit>
#include <QSplitter>
#include <QVBoxLayout>
#include <QWidget>


int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

    QApplication a(argc, argv);

    QWidget wnd;

    QTextEdit* editor1 = new QTextEdit;
    editor1->setText("Editor1");
    QTextEdit* editor2 = new QTextEdit;
    editor2->setText("Editor2");

    QSplitter* split1 = new QSplitter;
    split1->setOrientation(Qt::Vertical);
    split1->addWidget(editor1);
    split1->addWidget(editor2);

    QVBoxLayout* container_editors = new QVBoxLayout;
    container_editors->setContentsMargins(0,0,0,0);
    container_editors->addWidget(split1);

    QWidget* container = new QWidget;
    container->setLayout(container_editors);

    QSplitter* split2 = new QSplitter;
    QTextEdit* editor3 = new QTextEdit;
    editor3->setText("Editor3");

    split2->addWidget(editor3);
    split2->addWidget(container);

    QVBoxLayout* layout_wnd = new QVBoxLayout;
    layout_wnd->setContentsMargins(0,0,0,0);
    layout_wnd->addWidget(split2);

    wnd.setLayout(layout_wnd);
    wnd.show();

    return a.exec();
}
