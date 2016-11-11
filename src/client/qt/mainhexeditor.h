#ifndef MAINHEXEDITOR_H
#define MAINHEXEDITOR_H

#include <QWidget>

namespace Ui {
class MainHexEditor;
}

class MainHexEditor : public QWidget
{
    Q_OBJECT

public:
    explicit MainHexEditor(QWidget *parent = 0);
    ~MainHexEditor();

private:
    Ui::MainHexEditor *ui;
};

#endif // MAINHEXEDITOR_H
