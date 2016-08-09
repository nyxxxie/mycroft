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

HighlightArea::HighlightArea(int a, int b) {
    init(a, b);
}

HighlightArea::HighlightArea(int a, int b, const QColor& color) {
    init(a, b, color);
}

void HighlightArea::init(int a, int b) {
    init(a, b, QColor(255, 255, 255));
}

void HighlightArea::init(int a, int b, const QColor& color) {

    this->a = a;
    this->b = b;
    this->color = color;
}

void HighlightArea::update(int a, int b) {
    init(a, b);
}

void HighlightArea::update(int a, int b, const QColor& color) {
    init(a, b, color);
}

void HighlightArea::setA(int a) {
    this->a = a;
}

void HighlightArea::setB(int b) {
    this->b = b;
}

void HighlightArea::setColor(const QColor& color) {
    this->color = color;
}

int HighlightArea::getA() {
    return this->a;
}

int HighlightArea::getB() {
    return this->b;
}

QColor HighlightArea::getColor() {
    return this->color;
}

void HighlightArea::render(HexEditor* editor, QPainter& painter) {
}

/**
 * Initializes all variables to their default values.
 */
void HexEditor::init() {

    curfile = NULL;

    rows_total = 0;
    rows_shown = 0;
    row_top = 0;
    row_offset = 4;

    cursor = 0;
    verticalScrollBar()->setValue(0);

    setFont(QFont("Courier", 11));

    selector = new HighlightArea(0, 0, palette().highlight().color());
    selector_shouldrender = false;

    highlights.append(new HighlightArea(0, 0, QColor(255, 0, 255)));
    highlights.append(new HighlightArea(1, 1, QColor(255, 255, 0)));
    highlights.append(new HighlightArea(2, 2, QColor(0, 255, 0)));
    highlights.append(new HighlightArea(3, 3, QColor(0, 255, 255)));
    highlights.append(new HighlightArea(5, 7, QColor(0, 255, 255)));
    highlights.append(new HighlightArea(30, 50, QColor(255, 0, 0)));
    highlights.append(new HighlightArea(51, 53, QColor(255, 0, 255)));
    highlights.append(new HighlightArea(60, 70, QColor(0, 255, 0)));

    widget_start = 0;
    widget_gap = QMC_HEXEDIT_ELEMENT_GAP;
    widget_text_offset = 3;
    widgets.append(new AddressView(this));
    widgets.append(new HexView(this));
    widgets.append(new AsciiView(this));
}

/**
 * Recalculates spacing variables when something is changed.
 */
void HexEditor::adjust() {

    widgets_width = (widgets.size() > 0) ? (widgets.size()-1) * widget_gap : 0;
    for (HexEditorWidget* w : widgets) {
        widgets_width += w->getWidth();
    }

    horizontalScrollBar()->setRange(0, widgets_width - viewport()->width());
    horizontalScrollBar()->setPageStep(viewport()->width());

    if (curfile == NULL)
        return;

    rows_shown = ((viewport()->height() / (font_cheight+row_offset)) + 1);
    rows_total = file_size(curfile) / QMC_HEXEDIT_BYTESPERROW;

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
    for (HexEditorWidget* w : widgets) {
        w->mouseMoveEvent(event);
    }
}

/**
 * Handles a mouse press event.
 */
void HexEditor::mousePressEvent(QMouseEvent* event) {
    for (HexEditorWidget* w : widgets) {
        w->mousePressEvent(event);
    }
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
        widget_start = -horizontalScrollBar()->value();

        /* Draw stuff */
        for (HexEditorWidget* w : widgets) {
            w->render(painter);
        }

        //painter.drawText(widget_start, font_cheight+row_offset, "DEBUG file info:");
        //painter.drawText(widget_start, 2*(font_cheight+row_offset),
        //    QString("  size:     %1").arg(file_size(curfile)));
        //painter.drawText(widget_start, 3*(font_cheight+row_offset),
        //    QString("  rows:     %1").arg(rows_shown));
        //painter.drawText(widget_start, 4*(font_cheight+row_offset),
        //    QString("  all rows: %1").arg(rows_total));
        //painter.drawText(widget_start, 5*(font_cheight+row_offset),
        //    QString("  top row:  %1").arg(row_top));
    }
}

/**
 * Handles a resize event.
 */
void HexEditor::resizeEvent(QResizeEvent*) {
    adjust();
}

/**
 *
 */
HexEditor::HexEditor(QWidget* parent) : QAbstractScrollArea(parent) {

    /* Connect signals to slots */
    connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(adjust()));
    connect(horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(adjust()));

    /* Initialize variables */
    init();
}

/**
 * Determines if point is inside widget.
 */
bool HexEditorWidget::isMouseInside(const QPoint& point) {
    return (point.x() >= start && point.y() <= start+width);
}

/**
 *
 */
AddressView::AddressView(HexEditor* editor) {
    this->editor = editor;
    this->num_max = QString("deadbeef").size();
    this->width = ((num_max+1)*editor->font_cwidth) + (2*editor->widget_text_offset);
}

/**
 *
 */
void AddressView::render(QPainter& painter) {

    start = editor->widget_start;

    /* Draw bar part */
    painter.fillRect(
        QRect(start, 0, width, editor->viewport()->height()),
        editor->palette().alternateBase().color());

    /* Draw address part */
    for (int row=1, addr=editor->getCursorPos();
        row <= editor->rows_shown && addr < file_size(editor->curfile);
        row++, addr += QMC_HEXEDIT_BYTESPERROW) {

        painter.drawText(start+editor->widget_text_offset,
            row*(editor->font_cheight+editor->row_offset),
            QString("%1h").arg(addr, num_max, 16, QChar('0')).toUpper());
    }

    editor->widget_start += width + editor->widget_gap;
}

void AddressView::mouseMoveEvent(QMouseEvent* event) {

}

void AddressView::mousePressEvent(QMouseEvent* event) {

}

/**
 *
 */
HexView::HexView(HexEditor* editor) {
    this->editor = editor;
    this->byte_gap = 7;
    this->width = QMC_HEXEDIT_BYTESPERROW * (2*editor->font_cwidth)
        + (QMC_HEXEDIT_BYTESPERROW-1) * byte_gap
        + (2*editor->widget_text_offset);
}

int HexView::byteToPxStart(int byte) {
    int ret = start + editor->widget_text_offset;
    ret += byte * (editor->font_cwidth*2);
    ret += byte * byte_gap;
    return ret;
}

int HexView::byteToPxEnd(int byte) {
    int ret = byteToPxStart(byte);
    ret += editor->font_cwidth*2;
    ret += 1;
    return ret;
}

bool HexView::getPos(const QPoint& point, int* byte, int* nybble) {

    int s = start + editor->widget_text_offset;
    int e = (start+width) - (editor->widget_text_offset*2);
    int x = point.x();
    int y = point.y();

    if (x >= s && x <= e) {
        int sep = (editor->font_cwidth*2) + byte_gap;
        if (byte != NULL) {
            int vert_mul = 0;
            if (y >= 0) {
                vert_mul = QMC_HEXEDIT_BYTESPERROW*(y/(editor->font_cheight+editor->row_offset));
            }

            *byte = vert_mul + ((x-s) / sep);
        }

        if (nybble != NULL) {
            int space = (x-s) % sep;
            if (space <= editor->font_cwidth) {
                *nybble = 0;
            }
            else if (space > editor->font_cwidth && space <= editor->font_cwidth*2) {
                *nybble = 1;
            }
            else {
                *nybble = 2;
            }
        }

        return true;
    }

    return false;
}

/**
 *
 */
void HexView::renderHighlight(HighlightArea* area, QPainter& painter) {

    int selection_a = area->getA();
    int selection_b = area->getB();

    /* Swap the two points if the "front" one is bigger than the "rear" */
    if (selection_a > selection_b) {
        selection_b = area->getA();
        selection_a = area->getB();
    }

    /* Does the selection start in range of the file? */
    if (selection_a > file_size(editor->curfile)) {
        return;
    }

    /* If the selection extends past the end of the file, trim it */
    if (selection_b > file_size(editor->curfile)) {
        selection_b = file_size(editor->curfile);
    }

    /* Figure out the bytes and rows the selection will be on */
    const int startpos = selection_a % QMC_HEXEDIT_BYTESPERROW;
    const int startline = selection_a / QMC_HEXEDIT_BYTESPERROW;
    const int endpos = selection_b % QMC_HEXEDIT_BYTESPERROW;
    const int endline = selection_b / QMC_HEXEDIT_BYTESPERROW;

    /* If the endline is past the bounds of the bottom screen, trim it */
    //if (endline > rows_shown) {
    //    endline = startline + rows_shown;
    //}

    for (int curline = startline; curline <= endline; curline++) {
        int rel_line = curline - editor->row_top;

        /* Figure out where to start and end the selection line */
        int text_start = 0;
        int text_end = QMC_HEXEDIT_BYTESPERROW-1;
        if (curline == startline) {
            text_start = startpos;
        }
        if (curline == endline) {
            text_end = endpos;
        }

        /* Turn the byte positions into px positions */
        int px_text_start = byteToPxStart(text_start);
        int px_text_end = byteToPxEnd(text_end);
        px_text_end -= px_text_start;

        /* Offset the start and end by a fixed val for visual whimsey! */
        const int highlight_trail = 1;
        px_text_start -= highlight_trail;
        px_text_end += highlight_trail * 2;

        /* Determine where to place the highlight vertically */
        const int vert_space = editor->row_offset / 2;
        const int px_vstart = (editor->row_offset*(rel_line+1)) + (editor->font_cheight*rel_line) - (editor->row_offset/2);
        const int px_vend = editor->font_cheight + editor->row_offset;

        /* Draw the selection line */
        painter.fillRect(
            QRect(px_text_start, px_vstart, px_text_end, px_vend),
            area->getColor());
    }
}

/**
 *
 */
void HexView::render(QPainter& painter) {

    start = editor->widget_start;

    /* Draw background part */
    painter.fillRect(
        QRect(start, 0, width, editor->viewport()->height()),
        //editor->palette().color(QPalette::Base));
        editor->palette().alternateBase().color());

    /* Draw selection (if there is one) */
    for (HighlightArea* area : editor->highlights) {
        renderHighlight(area, painter);
    }

    /* Draw selector */
    if (editor->selector_shouldrender) {
        renderHighlight(editor->selector, painter);
    }

    /* Draw bytes */
    int cursor_prev = file_get_cursor(editor->curfile);
    for (int row=1; row <= editor->rows_shown; row++) {

        /* Read file bytes */
        uint8_t bytes[QMC_HEXEDIT_BYTESPERROW];

        //TODO: maintain a single cache/file buffer?
        int amnt = file_read(editor->curfile, QMC_HEXEDIT_BYTESPERROW, bytes);
        if (amnt == 0) {
            break;
        }
        else if (amnt < 0) {
            return;
        }

        /* Display bytes */
        int offset = editor->widget_text_offset;
        for (int j=0; j < amnt; j++) {
            painter.drawText(start+offset+(j*editor->font_cwidth*2),
                row*(editor->font_cheight+editor->row_offset),
                QString("%1").arg(bytes[j], 2, 16, QChar('0')).toUpper());
            offset += byte_gap;
        }
    }
    file_set_cursor(editor->curfile, cursor_prev);

    editor->widget_start += width + editor->widget_gap;
}

void HexView::mouseMoveEvent(QMouseEvent* event) {

    /* Some constants to shorten code */
    HighlightArea* s = editor->selector;
    const int a = s->getA();
    const int b = s->getB();
    const bool sr = editor->selector_shouldrender;

    int byte, nybble;
    if (getPos(event->pos(), &byte, &nybble)) {

        /* If this is the first movement after a mouse press, reset selection */
        editor->selector->setB(byte);
        //printf("byte:%i  a:%i  b:%i\n", byte, a, b);
        if (byte == a) {
            if (nybble >= 2) {
                editor->selector_shouldrender = true;
            }
            else if (nybble == 0 && editor->selector_shouldrender){
                editor->selector_shouldrender = false;
            }
        }
        else {
            editor->selector_shouldrender = true;
        }

        editor->adjust();
        editor->viewport()->update();
    }
}

void HexView::mousePressEvent(QMouseEvent* event) {
    int byte, nybble;
    if (getPos(event->pos(), &byte, &nybble)) {
        editor->selector->setA(byte);
        editor->selector->setB(byte);
    }
    editor->selector_shouldrender = false;
    editor->adjust();
    editor->viewport()->update();
}

/**
 * 
 */
AsciiView::AsciiView(HexEditor* editor) {
    this->editor = editor;
    this->width = (QMC_HEXEDIT_BYTESPERROW*editor->font_cwidth)
        + (2*editor->widget_text_offset);
}

/**
 * 
 */
void AsciiView::renderHighlight(HighlightArea* area, QPainter& painter) {

}

/**
 * 
 */
void AsciiView::render(QPainter& painter) {

    start = editor->widget_start;

    /* Draw bar part */
    painter.fillRect(
        QRect(start, 0, width, editor->viewport()->height()),
        editor->palette().alternateBase().color());

    /* Draw bytes */
    int cursor_prev = file_get_cursor(editor->curfile);
    for (int row=1; row <= editor->rows_shown; row++) {

        /* Read file bytes */
        uint8_t bytes[QMC_HEXEDIT_BYTESPERROW];
        int amnt = file_read(editor->curfile, QMC_HEXEDIT_BYTESPERROW, bytes);
        if (amnt == 0) {
            break;
        }
        else if (amnt < 0) {
            return;
        }

        /* Display bytes */
        for (int j=0; j < amnt; j++) {

            /* Figure out if byte isn't printable */
            QChar c = QChar(bytes[j]);
            if (!c.isPrint()) {
                c = QChar('.');
            }

            /* Print char */
            painter.drawText(editor->widget_text_offset+start+(j*editor->font_cwidth),
                row*(editor->font_cheight+editor->row_offset),
                QString("%1").arg(c));
        }
    }
    file_set_cursor(editor->curfile, cursor_prev);

    editor->widget_start += width + editor->widget_gap;
}

void AsciiView::mouseMoveEvent(QMouseEvent* event) {

}

void AsciiView::mousePressEvent(QMouseEvent* event) {

}

