#include <QtGui>
#include "templateeditor.h"

void TemplateEditor::ShowDirectory(QTreeWidgetItem* item, int column) {
    QDir* rootDir = new QDir(item->text(2));
    QFileInfoList filesList = rootDir->entryInfoList();

    foreach(QFileInfo fileInfo, filesList) {

        QTreeWidgetItem* child = new QTreeWidgetItem();
        child->setText(0,fileInfo.fileName());

        if(fileInfo.isFile()) {
          child->setText(1,QString::number(fileInfo.size()));
          child->setIcon(0,*(new QIcon("file.jpg")));
        }

        if(fileInfo.isDir()) {
          child->setIcon(0,*(new QIcon("folder.jpg")));
          child->setText(2,fileInfo.filePath());
        }

        item->addChild(child);

        resizeColumnToContents(0);
    }
}

void AddChildren(QTreeWidgetItem* item,QString filePath) {

    QDir* rootDir = new QDir(filePath);
    QFileInfoList filesList = rootDir->entryInfoList();

    foreach(QFileInfo fileInfo, filesList) {

        QTreeWidgetItem* child = new QTreeWidgetItem();
        child->setText(0,fileInfo.fileName());

        if(fileInfo.isFile()) {
            child->setText(1,QString::number(fileInfo.size()));
        }

        if(fileInfo.isDir()) {
            child->setIcon(0,*(new QIcon("folder.jpg")));
            child->setText(2,fileInfo.filePath());
        }

        item->addChild(child);
    }
}
void TemplateEditor::Init() {

    QTreeWidgetItem* headeritem = new QTreeWidgetItem();
    headeritem->setText(0, QString("File Name"));
    headeritem->setText(1, QString("Size"));
    headeritem->setText(2, QString("Path"));
    setHeaderItem(headeritem);

    QDir* rootdir = new QDir("/");
    QFileInfoList filelist = rootdir->entryInfoList();

    foreach(QFileInfo fileinfo, filelist) {
        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setText(0, fileinfo.fileName());

        if (fileinfo.isFile()) {
            item->setText(1, QString::number(fileinfo.size()));
            //item->setIcon(0, QIcon("file.jpg));
        }
        else if (fileinfo.isDir()) {
            AddChildren(item, fileinfo.filePath());
        }

        item->setText(2, fileinfo.filePath());
        addTopLevelItem(item);
    }
}

TemplateEditor::TemplateEditor() : QTreeWidget() {
    connect(this , SIGNAL(itemClicked(QTreeWidgetItem*,int)),
            this,  SLOT(ShowDirectory(QTreeWidgetItem*,int)));

    Init();
}
