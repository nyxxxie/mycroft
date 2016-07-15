#ifndef HEXEDITOR_H
#define HEXEDITOR_H

#include <QAbstractScrollArea>

#include "hexeditor.h"

class HexEditor : public QAbstractScrollArea {

    Q_OBJECT

    void init();

    void setFont(const QFont& font);

public:

    HexEditor();

protected:

    void keyPressEvent(QKeyEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void paintEvent(QPaintEvent* event);
    void resizeEvent(QResizeEvent* event);
};

#endif // HEXEDITOR_H
