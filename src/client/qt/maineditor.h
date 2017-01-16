#ifndef MAINEDITOR_QT_H
#define MAINEDITOR_QT_H

#include <QWidget>
#include "hexeditor.h"

class MainEditor : public QWidget
{
    Q_OBJECT

private:
    bool createHexEditor();
    bool destroyHexEditor();

public:
    explicit MainEditor(QWidget* parent = 0);
    ~MainEditor();

private:
    HexEditor* hexeditor;
};

#endif // MAINEDITOR_QT_H
