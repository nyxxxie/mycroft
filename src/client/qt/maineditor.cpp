#include "maineditor.h"

MainEditor::MainEditor(QWidget* parent)
{
    createHexEditor();
}

MainEditor::~MainEditor()
{

}

bool MainEditor::createHexEditor()
{
    hexeditor = new HexEditor(this);
    connect(parent(), SIGNAL(fileFocused(mc_file_t*)),
            hexeditor, SLOT(setFile(mc_file_t*)));
}

bool MainEditor::destroyHexEditor()
{

}
