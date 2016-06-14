#ifndef TEMPLATENODE_H
#define TEMPLATENODE_H

#include <QList>
#include <QVariant>
#include <QVector>

class TemplateNode {

    QList<TemplateNode*> m_childitems;
    QVector<QVariant> m_itemdata;
    TemplateNode* m_parentitem;

public:

    TemplateNode(const QVector<QVariant>& data, TemplateNode* parent = NULL);
    ~TemplateNode();

    TemplateNode* child(int number);
    int childCount();
    int columnCount();
    QVariant data(int column);
    bool insertChildren(int position, int count, int columns);
    bool insertColumns(int position, int columns);
    TemplateNode* parent();
    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);
    int childNumber();
    bool setData(int column, const QVariant &value);
};

#endif // TEMPLATENODE_H
