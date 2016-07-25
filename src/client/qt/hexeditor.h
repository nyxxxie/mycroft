#ifndef HEXEDITOR_H
#define HEXEDITOR_H

#include <QAbstractScrollArea>
#include <mycroft/mdb.h>
#include <mycroft/file.h>
#include "hexeditor.h"

//TODO: make these config variables
#define QMC_HEXEDIT_ELEMENT_GAP 6
#define QMC_HEXEDIT_BYTE_GAP 8
#define QMC_HEXEDIT_BYTESPERROW 16

class HexEditor : public QAbstractScrollArea {

    Q_OBJECT

    /* Main internal funcs */
    void init();

    /* File related */
    mc_file_t* curfile;

    /* Font related */
    void setFont(const QFont& font);

    /* Section drawing */
    void drawNoFile(QPainter& painter);
    void drawAddressBar(QPainter& painter);
    void drawHexContent(QPainter& painter);
    void drawAsciiContent(QPainter& painter);

    /* Row stuff */
    int rows_total;
    int rows_shown;
    int row_top;

    /* Cursor stuff */
    int cursor;
    int selection_start;
    int selection_direction; // cursor should be the end, this handles the situation where we make a reverse direction.  This should be 1 for forward, -1 for backward, and 0 for no selection.

    /* Spacing stuff */
    int element_offset;
    int element_gap;

    int font_cwidth;
    int font_cheight;

    int text_offset; /** Offset between background start/end and text start/end. */
    int row_offset;  /** Offset beween rows. */

    int addrbar_offset;
    int addrbar_width;
    int addrbar_num_max;

    int hexarea_offset;
    int hexarea_width;
    int hexarea_text_gap;

    int asciiarea_offset;
    int asciiarea_width;

    int textarea_width;

public:

    HexEditor(QWidget* parent = NULL);

    int getNumLines();
    void setCurLine(int line);
    int getCurLine();
    void setCursorPos(int pos);
    int getCursorPos();

    void setCurrentFile(mc_file_t* file);

protected:

    void keyPressEvent(QKeyEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void paintEvent(QPaintEvent* event);
    void resizeEvent(QResizeEvent* event);

private slots:

    void adjust();
};

#endif // HEXEDITOR_H
