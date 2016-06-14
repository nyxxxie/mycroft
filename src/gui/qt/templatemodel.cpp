#include <QFile>
#include "templatemodel.h"

TemplateModel::TemplateModel()
    : QAbstractItemModel() {

    QVector<QVariant> headers;
    headers << "Variable";
    headers << "Value";
    headers << "Comment";

    m_root = new TemplateNode(headers);

    QFile file("default.txt");
    file.open(QIODevice::ReadOnly);
    setupModelData(((QString)file.readAll()).split('\n'), m_root);
    file.close();
}

TemplateModel::~TemplateModel() {
    delete m_root;
}

int TemplateModel::columnCount(const QModelIndex & /* parent */) const {
    return m_root->columnCount();
}

QVariant TemplateModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    TemplateNode *item = getItem(index);

    return item->data(index.column());
}

Qt::ItemFlags TemplateModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

TemplateNode *TemplateModel::getItem(const QModelIndex &index) const {
    if (index.isValid()) {
        TemplateNode *item = static_cast<TemplateNode*>(index.internalPointer());
        if (item)
            return item;
    }
    return m_root;
}

QVariant TemplateModel::headerData(int section,
    Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return m_root->data(section);

    return QVariant();
}

QModelIndex TemplateModel::index(int row, int column,
    const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    TemplateNode *parentItem = getItem(parent);

    TemplateNode *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

bool TemplateModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = m_root->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

bool TemplateModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    TemplateNode *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, m_root->columnCount());
    endInsertRows();

    return success;
}

QModelIndex TemplateModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TemplateNode *childItem = getItem(index);
    TemplateNode *parentItem = childItem->parent();

    if (parentItem == m_root)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool TemplateModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = m_root->removeColumns(position, columns);
    endRemoveColumns();

    if (m_root->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

bool TemplateModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    TemplateNode *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

int TemplateModel::rowCount(const QModelIndex &parent) const
{
    TemplateNode *parentItem = getItem(parent);

    return parentItem->childCount();
}

bool TemplateModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    TemplateNode *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result)
        emit dataChanged(index, index);

    return result;
}

bool TemplateModel::setHeaderData(int section, Qt::Orientation orientation,
    const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = m_root->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

void TemplateModel::setupModelData(const QStringList &lines, TemplateNode *parent)
{
    QList<TemplateNode*> parents;
    QList<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;

    while (number < lines.count()) {
        int position = 0;
        while (position < lines[number].length()) {
            if (lines[number].at(position) != ' ')
                break;
            ++position;
        }

        QString lineData = lines[number].mid(position).trimmed();

        if (!lineData.isEmpty()) {
            // Read the column data from the rest of the line.
            QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
            QVector<QVariant> columnData;
            for (int column = 0; column < columnStrings.count(); ++column)
                columnData << columnStrings[column];

            if (position > indentations.last()) {
                // The last child of the current parent is now the new parent
                // unless the current parent has no children.

                if (parents.last()->childCount() > 0) {
                    parents << parents.last()->child(parents.last()->childCount()-1);
                    indentations << position;
                }
            } else {
                while (position < indentations.last() && parents.count() > 0) {
                    parents.pop_back();
                    indentations.pop_back();
                }
            }

            // Append a new item to the current parent's list of children.
            TemplateNode *parent = parents.last();
            parent->insertChildren(parent->childCount(), 1, m_root->columnCount());
            for (int column = 0; column < columnData.size(); ++column)
                parent->child(parent->childCount() - 1)->setData(column, columnData[column]);
        }

        ++number;
    }
}
