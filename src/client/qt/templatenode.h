#ifndef TEMPLATENODE_H
#define TEMPLATENODE_H

#include <QList>
#include <QVariant>

class TemplateNode {
public:
    explicit TemplateNode(const QList<QVariant> &data, TemplateNode *parentItem = 0);
    ~TemplateNode();

    void appendChild(TemplateNode *child);

    TemplateNode *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    TemplateNode *parentItem();

private:
    QList<TemplateNode*> m_childItems;
    QList<QVariant> m_itemData;
    TemplateNode *m_parentItem;
};

#endif // TEMPLATENODE_H
