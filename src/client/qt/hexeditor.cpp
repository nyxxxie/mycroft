/*****************************************************************************\
Mycroft - A hex editor and file format reverse engineering tool.
Copyright (C) 2016 Nyxxie <github.com/nyxxxie>

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.
This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 51 Franklin
Street, Fifth Floor, Boston, MA  02110-1301, USA.

******************************************************************************

FILE TODO:
    * Some general refactoring would be nice; this file is a mess...
    * Create a modular row display system, so plugins and etc can dynamically
      add new row displays.
    * Create a cursor variable that represents the line cursor position.
        * Move this cusor up and down when the file is scrolled (?)
    * Move all file_* operations to areas where bytes are read and written.
        * Rogue set and get custor operations are all over, remove them.
\*****************************************************************************/

#include <QAbstractScrollArea>
#include <QScrollBar>
#include <QPainter>
#include <QKeyEvent>
#include <mycroft/mdb.h>
#include <mycroft/file.h>
#include <stdio.h>
#include "mainwindow.h"
#include "hexeditor.h"

HighlightArea::HighlightArea() {
    init(0, 0);
}

HighlightArea::HighlightArea(int start, int end) {
    init(start, end);
}

HighlightArea::HighlightArea(int start, int end, const QColor& color) {
    init(start, end, color);
}

void HighlightArea::init(int start, int end) {
    init(start, end, QColor(255, 255, 255));
}

void HighlightArea::init(int start, int end, const QColor& color) {
    setStart(start);
    setEnd(end);
    setColor(color);
}

void HighlightArea::setStart(int start) {
    this->start = start;
}

void HighlightArea::setEnd(int end) {
    this->end = end;
}

void HighlightArea::setColor(const QColor& color) {
    this->color = color;
}

int HighlightArea::getStart() {
    return this->start;
}

int HighlightArea::getEnd() {
    return this->end;
}

QColor HighlightArea::getColor() {
    return this->color;
}

/**
 * Initializes all variables to their default values.
 */
void HexEditor::init() {

    curfile = NULL;

    rows_total = 0;
    rows_shown = 0;
    row_top = 0;

    cursor = 0;
    selection_start = 0;

    element_offset = 0;
    element_gap = QMC_HEXEDIT_ELEMENT_GAP;

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

    highlights.append(new HighlightArea(50, 30, QColor(255, 0, 0)));
    highlights.append(new HighlightArea(53, 51, QColor(255, 0, 255)));
    highlights.append(new HighlightArea(70, 60, QColor(0, 255, 0)));
    highlights.append(new HighlightArea(200, 100, QColor(0, 0, 255)));
}

/**
 * Recalculates spacing variables when something is changed.
 */
void HexEditor::adjust() {

    if (curfile == NULL)
        return;

    rows_shown = ((viewport()->height() / (font_cheight+row_offset)) + 1);
    rows_total = file_size(curfile) / QMC_HEXEDIT_BYTESPERROW;

    addrbar_num_max = QString("%1").arg(QString::number(file_size(curfile), 16)).size();

    addrbar_width = ((addrbar_num_max+1)*font_cwidth) + (2*text_offset);
    asciiarea_width = (QMC_HEXEDIT_BYTESPERROW*font_cwidth) + (2*text_offset);
    hexarea_width = QMC_HEXEDIT_BYTESPERROW * (2*font_cwidth)
        + (QMC_HEXEDIT_BYTESPERROW-1) * hexarea_text_gap
        + (2*text_offset);
    textarea_width = 2*element_gap + addrbar_width + hexarea_width + asciiarea_width;

    horizontalScrollBar()->setRange(0, textarea_width - viewport()->width());
    horizontalScrollBar()->setPageStep(viewport()->width());
    verticalScrollBar()->setRange(0, rows_total);
    verticalScrollBar()->setPageStep(rows_shown);

    setCurLine(verticalScrollBar()->value());
}

/**
 * Gets the total number of lines in the current file.
 *
 * @return Total number of lines in the current file.
 */
int HexEditor::getNumLines() {
    return rows_total;
}

/**
 * Sets the top line in the editor.
 *
 * @param line Line to focus on.
 */
void HexEditor::setCurLine(int line) {
    row_top = line;
    file_set_cursor(curfile, line*QMC_HEXEDIT_BYTESPERROW);
}

/**
 * Gets the line number at the top of the editor.
 *
 * @return Line number of the top line in the editor.
 */
int HexEditor::getCurLine() {
    return row_top;
}

void HexEditor::setCursorPos(int pos) {
    file_set_cursor(curfile, pos*QMC_HEXEDIT_BYTESPERROW);
}

int HexEditor::getCursorPos() {
    if (curfile == NULL)
        return 0;

    return file_get_cursor(curfile);
}

void HexEditor::setCurrentFile(mc_file_t* file) {
    curfile = file;
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

/**
 * Called internally to display when there is no file loaded.
 */
void HexEditor::drawNoFile(QPainter& painter) {
    painter.drawText(geometry(),
        Qt::AlignHCenter|Qt::AlignVCenter,
        "No file loaded.");
}

/**
 * Called internally to draw the hex editor address bar.
 */
void HexEditor::drawAddressBar(QPainter& painter) {

    addrbar_offset = element_offset;

    /* Draw bar part */
    painter.fillRect(
        QRect(element_offset, 0, addrbar_width, viewport()->height()),
        QColor(255, 0, 0));

    /* Draw address part */
    for (int row=1, addr=getCursorPos();
        row <= rows_shown && addr < file_size(curfile);
        row++, addr += QMC_HEXEDIT_BYTESPERROW) {

        painter.drawText(element_offset+text_offset,
            row*(font_cheight+row_offset),
            QString("%1h").arg(addr, addrbar_num_max, 16, QChar('0')).toUpper());
    }

    element_offset += addrbar_width + element_gap;
}

int byteToScreenX(int byte_number) {

}

/**
 * Called internally to draw the hex editor's current file's bytes.
 */
void HexEditor::drawHexContent(QPainter& painter) {

    hexarea_offset = element_offset;

    /* Draw background part */
    painter.fillRect(
        QRect(element_offset, 0, hexarea_width, viewport()->height()),
        QColor(240, 240, 240));

    /* Draw selection (if there is one) */
    for (HighlightArea* area : highlights) {
        cursor = area->getStart();
        selection_start = area->getEnd();
        if (cursor >= selection_start) {
            const int startpos = selection_start % QMC_HEXEDIT_BYTESPERROW;
            const int startline = selection_start / QMC_HEXEDIT_BYTESPERROW;
            const int endpos = cursor % QMC_HEXEDIT_BYTESPERROW;
            const int endline = cursor / QMC_HEXEDIT_BYTESPERROW;

            for (int curline = startline; curline <= endline; curline++) {

                /* Figure out where to start and end the selection line */
                int start = 0;
                int end = QMC_HEXEDIT_BYTESPERROW;
                if (curline == startline) {
                    start = startpos;
                }
                if (curline == endline) {
                    end = endpos;
                }

                /* Turn the byte position into a px position */
                //start = 20;
                //end = 40;
                int hexarea_sel_extra = 1;
                int vert_space = row_offset / 2;
                start = start * ((font_cwidth*2)+hexarea_text_gap) - hexarea_sel_extra;
                end = (end * ((font_cwidth*2)+hexarea_text_gap)) - start - hexarea_text_gap + (hexarea_sel_extra*2) - 1;
                start += element_offset + text_offset;
                int tmp = curline - row_top;
                int vstart = (row_offset*(tmp+1)) + (font_cheight*tmp) - vert_space;

                /* Draw the selection line */
                painter.fillRect(
                    QRect(start, vstart, end, font_cheight + (vert_space*2)),
                    area->getColor());
            }
        }
    }

    /* Draw bytes */
    int cursor_prev = file_get_cursor(curfile);
    for (int row=1; row <= rows_shown; row++) {

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

/**
 * Draws the ascii representation of the current set of file bytes.
 */
void HexEditor::drawAsciiContent(QPainter& painter) {

    asciiarea_offset = element_offset;

    /* Draw bar part */
    painter.fillRect(
        QRect(element_offset, 0, asciiarea_width, viewport()->height()),
        QColor(0, 0, 255));

    /* Draw bytes */
    int cursor_prev = file_get_cursor(curfile);
    for (int row=1; row <= rows_shown; row++) {

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
                QString("%1").arg(QChar(bytes[j]))); //TODO: check if displayable
        }
    }
    file_set_cursor(curfile, cursor_prev);

    element_offset += asciiarea_width + element_gap;
}

/**
 * Handles a key press event.
 */
void HexEditor::keyPressEvent(QKeyEvent* event) {

    /* Move down one */
    if (event->matches(QKeySequence::MoveToNextLine) ||
        event->key() == Qt::Key_J) {
        verticalScrollBar()->setValue(getCurLine() + 1);
        viewport()->update();
    }

    /* Move up one */
    if (event->matches(QKeySequence::MoveToPreviousLine) ||
        event->key() == Qt::Key_K) {
        verticalScrollBar()->setValue(getCurLine() - 1);
        viewport()->update();
    }
}

/**
 * Handles a move movement event.
 */
void HexEditor::mouseMoveEvent(QMouseEvent* event) {

}

/**
 * Handles a mouse press event.
 */
void HexEditor::mousePressEvent(QMouseEvent* event) {

}

/**
 * Handles a paint event.
 */
void HexEditor::paintEvent(QPaintEvent* event) {

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
            QString("  rows:     %1").arg(rows_shown));
        painter.drawText(element_offset, 4*(font_cheight+row_offset),
            QString("  all rows: %1").arg(rows_total));
        painter.drawText(element_offset, 5*(font_cheight+row_offset),
            QString("  top row:  %1").arg(row_top));

        /* Figure out how much area is taken up by the drawn area */
        textarea_width = element_offset;
    }
}

/**
 * Handles a resize event.
 */
void HexEditor::resizeEvent(QResizeEvent*) {
    adjust();
}

HexEditor::HexEditor(QWidget* parent) : QAbstractScrollArea(parent) {

    /* Connect signals to slots */
    connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(adjust()));
    connect(horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(adjust()));

    /* Initialize variables */
    init();

    /* Initialize other element stuff */
    setFont(QFont("Courier", 11));
    verticalScrollBar()->setValue(getCursorPos()/QMC_HEXEDIT_BYTESPERROW);
}
