#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QTextEdit>
#include <QSplitter>
#include <QVBoxLayout>
#include <QWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QWidget* window;
    QVBoxLayout* main_layout;
    QWidget* tools_container, *editor_container;
    QVBoxLayout* tools_layout, *editor_layout;

    QTextEdit* filebrowser;
    QTextEdit* tools;
    QTextEdit* hexedit;
    QTextEdit* templateedit;

    void InitFilebrowser();
    void InitTools();
    void InitHexEditor();
    void InitTemplateEditor();

public:
    MainWindow();
    ~MainWindow();

private slots:
    void on_actionExit_triggered();
};

#endif // MAINWINDOW_H
