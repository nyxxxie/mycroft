#include <QPainter>
#include <QColor>
#include "hexeditor.h"

void HexEditor::init() {
    setFont(QFont("Courier", 10));
}

void HexEditor::setFont(const QFont& font) {

    QWidget::setFont(font);
    //TODO: figure out font-related size info here
    viewport()->update();
}

HexEditor::HexEditor() {
    init();
}

void HexEditor::keyPressEvent(QKeyEvent *event) {

}

void HexEditor::mouseMoveEvent(QMouseEvent * event) {

}

void HexEditor::mousePressEvent(QMouseEvent * event) {

}

void HexEditor::paintEvent(QPaintEvent *event) {

    QPainter painter(viewport());

    painter.fillRect(QRect(0, 0, 40, 40), QColor(255, 0, 0));
    painter.drawText(50, 50, "Have some text, my dear.");
}

void HexEditor::resizeEvent(QResizeEvent *) {

}
