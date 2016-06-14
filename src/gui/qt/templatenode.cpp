#include <templatenode.h>

#include <QStringList>

TemplateNode::TemplateNode(const QVector<QVariant> &data, TemplateNode *parent) {
    m_parentitem = parent;
    m_itemdata = data;
}

TemplateNode::~TemplateNode() {
    qDeleteAll(m_childitems);
}

TemplateNode *TemplateNode::child(int number) {
    return m_childitems.value(number);
}

int TemplateNode::childCount() {
    return m_childitems.count();
}

int TemplateNode::childNumber() {

    if (m_parentitem)
        return m_parentitem->m_childitems.indexOf(const_cast<TemplateNode*>(this));

    return 0;
}

int TemplateNode::columnCount() {
    return m_itemdata.count();
}

QVariant TemplateNode::data(int column) {
    return m_itemdata.value(column);
}

bool TemplateNode::insertChildren(int position, int count, int columns) {

    if (position < 0 || position > m_childitems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data(columns);
        TemplateNode *item = new TemplateNode(data, this);
        m_childitems.insert(position, item);
    }

    return true;
}

bool TemplateNode::insertColumns(int position, int columns) {

    if (position < 0 || position > m_itemdata.size())
        return false;

    for (int column = 0; column < columns; ++column)
        m_itemdata.insert(position, QVariant());

    foreach (TemplateNode *child, m_childitems)
        child->insertColumns(position, columns);

    return true;
}

TemplateNode *TemplateNode::parent() {
    return m_parentitem;
}

bool TemplateNode::removeChildren(int position, int count) {

    if (position < 0 || position + count > m_childitems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete m_childitems.takeAt(position);

    return true;
}

bool TemplateNode::removeColumns(int position, int columns) {

    if (position < 0 || position + columns > m_itemdata.size())
        return false;

    for (int column = 0; column < columns; ++column)
        m_itemdata.remove(position);

    foreach (TemplateNode *child, m_childitems)
        child->removeColumns(position, columns);

    return true;
}

bool TemplateNode::setData(int column, const QVariant &value) {

    if (column < 0 || column >= m_itemdata.size())
        return false;

    m_itemdata[column] = value;
    return true;
}
