#ifndef MAINHEXEDITOR_H
#define MAINHEXEDITOR_H

#include <QWidget>
#include "mycroft.h"

namespace Ui {
class MainHexEditor;
}

class MainHexEditor : public QWidget
{
    friend class Mycroft;
    Q_OBJECT

public:
    explicit MainHexEditor(QWidget* parent = 0);
    ~MainHexEditor();

private:
    Ui::MainHexEditor* ui;
};

#endif // MAINHEXEDITOR_H
