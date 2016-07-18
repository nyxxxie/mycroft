#include <QAbstractScrollArea>
#include <QScrollBar>
#include <QPainter>
#include <QColor>
#include <mycroft/mdb.h>
#include <mycroft/file.h>
#include <stdio.h>
#include "mainwindow.h"
#include "hexeditor.h"

void HexEditor::init() {

    curfile = NULL;
    element_offset = 0;
    element_gap = QMC_HEXEDIT_ELEMENT_GAP;
    rows_shown = 0;
    font_cwidth = 0;
    font_cheight = 0;
    text_offset = 3;
    row_offset = 4;
    addrbar_offset = 0;
    addrbar_width = 0;
    addrbar_num_max = 0;
    hexarea_offset = 0;
    hexarea_width = 0;
    hexarea_text_gap = 7;
    asciiarea_offset = 0;
    asciiarea_width = 0;
    textarea_width = 0;
    textarea_rows = 0;
    textarea_toprow = 0;
    textarea_rows_visible = 0;

    setFont(QFont("Courier", 11));
    verticalScrollBar()->setValue(getCursor()/QMC_HEXEDIT_BYTESPERROW);
}

void HexEditor::adjust() {

    if (curfile == NULL)
        return;

    textarea_width = 2*element_gap + addrbar_width + hexarea_width + asciiarea_width;
    addrbar_num_max = QString("%1").arg(QString::number(file_size(curfile), 16)).size();
    addrbar_width = ((addrbar_num_max+1)*font_cwidth) + (2*text_offset);
    textarea_rows = file_size(curfile) / QMC_HEXEDIT_BYTESPERROW;
    asciiarea_width = (QMC_HEXEDIT_BYTESPERROW*font_cwidth) + (2*text_offset);
    hexarea_width = QMC_HEXEDIT_BYTESPERROW * (2*font_cwidth)
        + (QMC_HEXEDIT_BYTESPERROW-1) * hexarea_text_gap
        + (2*text_offset);
    textarea_rows_visible = ((viewport()->height() / (font_cheight+row_offset)) + 1);

    horizontalScrollBar()->setRange(0, textarea_width - viewport()->width());
    horizontalScrollBar()->setPageStep(viewport()->width());
    verticalScrollBar()->setRange(0, textarea_rows);
    verticalScrollBar()->setPageStep(textarea_rows_visible);

    setCurLine(verticalScrollBar()->value());
}

int HexEditor::getNumLines() {
    return textarea_rows;
}

void HexEditor::setCurLine(int pos) {
    file_set_cursor(curfile, pos*QMC_HEXEDIT_BYTESPERROW);
}

void HexEditor::setCursor(int pos) {
    file_set_cursor(curfile, pos);
}

int HexEditor::getCursor() {
    if (curfile == NULL)
        return 0;

    return file_get_cursor(curfile);
}

void HexEditor::setCurrentFile(mc_file_t* file) {
    curfile = file;
    textarea_rows = file_size(file) / QMC_HEXEDIT_BYTESPERROW;
    adjust();
    viewport()->update();
}

void HexEditor::setFont(const QFont& font) {

    QWidget::setFont(font);

    QFontMetrics fm(font);
    font_cwidth = fm.width(' ');
    font_cheight = fm.height() / 2;

    adjust();
    viewport()->update();
}

void HexEditor::drawNoFile(QPainter& painter) {
    painter.drawText(geometry(),
        Qt::AlignHCenter|Qt::AlignVCenter,
        "No file loaded.");
}

void HexEditor::drawAddressBar(QPainter& painter) {

    addrbar_offset = element_offset;

    /* Draw bar part */
    painter.fillRect(
        QRect(element_offset, 0, addrbar_width, viewport()->height()),
        QColor(255, 0, 0));

    /* Draw address part */
    for (int row=1, addr=getCursor();
        row <= textarea_rows_visible && addr < file_size(curfile);
        row++, addr += QMC_HEXEDIT_BYTESPERROW) {

        painter.drawText(element_offset+text_offset,
            row*(font_cheight+row_offset),
            QString("%1h").arg(addr, addrbar_num_max, 16, QChar('0')).toUpper());
    }

    element_offset += addrbar_width + element_gap;
}

void HexEditor::drawHexContent(QPainter& painter) {

    hexarea_offset = element_offset;

    /* Draw bar part */
    painter.fillRect(
        QRect(element_offset, 0, hexarea_width, viewport()->height()),
        QColor(0, 255, 0));

    /* Draw bytes */
    int cursor_prev = file_get_cursor(curfile);
    for (int row=1; row <= textarea_rows_visible; row++) {

        /* Read file bytes */
        uint8_t bytes[QMC_HEXEDIT_BYTESPERROW];
        int amnt = file_read(curfile, QMC_HEXEDIT_BYTESPERROW, bytes);
        if (amnt == 0) {
            break;
        }
        else if (amnt < 0) {
            return;
        }

        /* Display bytes */
        int offset = text_offset;
        for (int j=0; j < amnt; j++) {
            painter.drawText(offset+element_offset+(j*font_cwidth*2),
                row*(font_cheight+row_offset),
                QString("%1").arg(bytes[j], 2, 16, QChar('0')).toUpper());
            offset += hexarea_text_gap;
        }
    }
    file_set_cursor(curfile, cursor_prev);

    element_offset += hexarea_width + element_gap;
}

void HexEditor::drawAsciiContent(QPainter& painter) {

    asciiarea_offset = element_offset;

    /* Draw bar part */
    painter.fillRect(
        QRect(element_offset, 0, asciiarea_width, viewport()->height()),
        QColor(0, 0, 255));

    /* Draw bytes */
    int cursor_prev = file_get_cursor(curfile);
    for (int row=1; row <= textarea_rows_visible; row++) {

        /* Read file bytes */
        uint8_t bytes[QMC_HEXEDIT_BYTESPERROW];
        int amnt = file_read(curfile, QMC_HEXEDIT_BYTESPERROW, bytes);
        if (amnt == 0) {
            break;
        }
        else if (amnt < 0) {
            return;
        }

        /* Display bytes */
        for (int j=0; j < amnt; j++) {
            painter.drawText(text_offset+element_offset+(j*font_cwidth),
                row*(font_cheight+row_offset),
                QString("%1").arg(QChar(bytes[j])));
        }
    }
    file_set_cursor(curfile, cursor_prev);

    element_offset += asciiarea_width + element_gap;
}

void HexEditor::keyPressEvent(QKeyEvent *event) {
    // TODO: for experimentation purposes, print these events in a qt status
    //       bar (we need to add one anyways)
}

void HexEditor::mouseMoveEvent(QMouseEvent * event) {

}

void HexEditor::mousePressEvent(QMouseEvent * event) {

}

void HexEditor::paintEvent(QPaintEvent *event) {

    QPainter painter(viewport());


    if (curfile == NULL) {
        drawNoFile(painter);
    }
    else {
        element_offset = -horizontalScrollBar()->value();

        /* Draw stuff */
        drawAddressBar(painter);
        drawHexContent(painter);
        drawAsciiContent(painter);

        painter.drawText(element_offset, font_cheight+row_offset, "DEBUG file info:");
        painter.drawText(element_offset, 2*(font_cheight+row_offset),
            QString("  size:     %1").arg(file_size(curfile)));
        painter.drawText(element_offset, 3*(font_cheight+row_offset),
            QString("  rows:     %1").arg(textarea_rows));
        painter.drawText(element_offset, 4*(font_cheight+row_offset),
            QString("  all rows: %1").arg(textarea_rows_visible));

        /* Figure out how much area is taken up by the drawn area */
        textarea_width = element_offset;
    }
}

void HexEditor::resizeEvent(QResizeEvent *) {
    adjust();
}

HexEditor::HexEditor(QWidget* parent) : QAbstractScrollArea(parent) {

    /* Connect signals to slots */
    connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(adjust()));
    connect(horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(adjust()));

    /* Initialize variables */
    init();
}
