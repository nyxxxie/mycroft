#include <QStringList>
#include "templatenode.h"

TemplateNode::TemplateNode(const QList<QVariant> &data, TemplateNode *parent)
{
    m_parentItem = parent;
    m_itemData = data;
}

TemplateNode::~TemplateNode()
{
    qDeleteAll(m_childItems);
}

void TemplateNode::appendChild(TemplateNode *item)
{
    m_childItems.append(item);
}

TemplateNode *TemplateNode::child(int row)
{
    return m_childItems.value(row);
}

int TemplateNode::childCount() const
{
    return m_childItems.count();
}

int TemplateNode::columnCount() const
{
    return m_itemData.count();
}

QVariant TemplateNode::data(int column) const
{
    return m_itemData.value(column);
}

TemplateNode *TemplateNode::parentItem()
{
    return m_parentItem;
}

int TemplateNode::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<TemplateNode*>(this));

    return 0;
}
