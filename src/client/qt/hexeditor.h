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
class HexEditorWidget;
class AddressView;
class HexView;
class AsciiView;

class HexEditor : public QAbstractScrollArea {
    friend class HighlightArea;
    friend class HexEditorWidget;
    friend class AddressView;
    friend class HexView;
    friend class AsciiView;

    Q_OBJECT

    /* Main internal funcs */
    void init();

    /* File related */
    mc_file_t* curfile;

    /* Font related */
    void setFont(const QFont& font);

    /* Section drawing */
    void drawNoFile(QPainter& painter);

protected:

    /* Highlights */
    QList<HighlightArea*> highlights; // TODO: replace with template rendering, leave this for testing
    QList<HighlightArea*> search_results;
    HighlightArea* selector;
    bool selector_shouldrender;

    /* Widgets */
    QList<HexEditorWidget*> widgets;
    int widgets_width;      /** Total width of all widgets */
    int widget_start;       /** Variable that should be used to determine start of current widget. */
    int widget_gap;         /** Gap between widgets */
    int widget_text_offset; /** Offset between widget start/end and text */

    /* Row stuff */
    int rows_total;
    int rows_shown;
    int row_top;
    int row_offset;  /** Offset beween rows. */

    /* Cursor stuff */
    int cursor;

    /* Font spacing info */
    int font_cwidth;
    int font_cheight;


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

    int a;
    int b;
    QColor color;

public:

    HighlightArea();
    HighlightArea(int start, int end);
    HighlightArea(int start, int end, const QColor& color);

    void init(int start, int end);
    void init(int start, int end, const QColor& color);
    void update(int start, int end);
    void update(int start, int end, const QColor& color);

    void setA(int start);
    void setB(int end);
    void setColor(const QColor& color);
    int getA();
    int getB();
    QColor getColor();

    void render(HexEditor* editor, QPainter& painter);
};


// TODO: implement these

/**
 * 
 */
class HexEditorWidget {
    friend class HexEditor;

protected:

    HexEditor* editor;
    int start;
    int width;

public:

    int getWidth() { return width; }
    bool isMouseInside(const QPoint& point);

    virtual void render(QPainter& painter) = 0;
    virtual void mouseMoveEvent(QMouseEvent* event) = 0;
    virtual void mousePressEvent(QMouseEvent* event) = 0;
};

/**
 * 
 */
class AddressView : public HexEditorWidget {

    int num_max;

public:

    AddressView(HexEditor* editor);

    void render(QPainter& painter);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
};

/**
 * 
 */
class HexView : public HexEditorWidget {

    int byte_gap;

    void renderHighlight(HighlightArea* area, QPainter& painter);

public:

    HexView(HexEditor* editor);

    int byteToPxStart(int byte);
    int byteToPxEnd(int byte);
    bool getPos(const QPoint& point, int* byte=NULL, int* nybble=NULL);

    void render(QPainter& painter);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
};

/**
 * 
 */
class AsciiView : public HexEditorWidget {

    void renderHighlight(HighlightArea* area, QPainter& painter);

public:

    AsciiView(HexEditor* editor);

    void render(QPainter& painter);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
};

#endif // HEXEDITOR_H
