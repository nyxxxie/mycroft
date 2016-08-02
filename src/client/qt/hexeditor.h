#ifndef HEXEDITOR_H
#define HEXEDITOR_H

#include <QAbstractScrollArea>
#include <QColor>
#include <mycroft/mdb.h>
#include <mycroft/file.h>
#include "hexeditor.h"

//TODO: make these config variables
#define QMC_HEXEDIT_ELEMENT_GAP 6
#define QMC_HEXEDIT_BYTE_GAP 8
#define QMC_HEXEDIT_BYTESPERROW 16

class HighlightArea; // forward decl

class HexEditor : public QAbstractScrollArea {
    friend class HighlightArea; // needed so render func can access privates

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

    /* Highlights */
    QList<HighlightArea*> highlights; // TODO: replace with template rendering, leave this for testing
    QList<HighlightArea*> search_results;
    HighlightArea* selector;

    /* Row stuff */
    int rows_total;
    int rows_shown;
    int row_top;

    /* Cursor stuff */
    int cursor;

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

class HighlightArea {

    int start;
    int end;
    QColor color;

public:

    HighlightArea();
    HighlightArea(int start, int end);
    HighlightArea(int start, int end, const QColor& color);

    void init(int start, int end);
    void init(int start, int end, const QColor& color);
    void update(int start, int end);
    void update(int start, int end, const QColor& color);

    void setStart(int start);
    void setEnd(int end);
    void setColor(const QColor& color);
    int getStart();
    int getEnd();
    QColor getColor();

    void render(HexEditor* editor, QPainter& painter);
};

#endif // HEXEDITOR_H
