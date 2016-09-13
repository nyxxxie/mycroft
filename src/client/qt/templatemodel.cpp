#include <QColor>
#include "templatemodel.h"

TemplateModel::TemplateModel(const QString &data, QObject *parent)
    : QAbstractItemModel(parent)
{
    QList<QVariant> rootData;
    rootData << "Title" << "Summary";
    rootItem = new TemplateNode(rootData);
    setupModelData(data.split(QString("\n")), rootItem);
}

TemplateModel::~TemplateModel()
{
    delete rootItem;
}

int TemplateModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TemplateNode*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QVariant TemplateModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        TemplateNode *item = static_cast<TemplateNode*>(index.internalPointer());
        return item->data(index.column());
    }
    else if (role == Qt::BackgroundRole) {
        if (index.row() % 2 == 0) {
            return QVariant(QColor(226, 237, 253));
        }
        else {
            return QVariant(QColor(Qt::white));
        }
    }
    else {
        return QVariant();
    }
}

Qt::ItemFlags TemplateModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant TemplateModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex TemplateModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TemplateNode *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TemplateNode*>(parent.internalPointer());

    TemplateNode *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex TemplateModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TemplateNode *childItem = static_cast<TemplateNode*>(index.internalPointer());
    TemplateNode *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int TemplateModel::rowCount(const QModelIndex &parent) const
{
    TemplateNode *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TemplateNode*>(parent.internalPointer());

    return parentItem->childCount();
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
            position++;
        }

        QString lineData = lines[number].mid(position).trimmed();

        if (!lineData.isEmpty()) {
            // Read the column data from the rest of the line.
            QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
            QList<QVariant> columnData;
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
            parents.last()->appendChild(new TemplateNode(columnData, parents.last()));
        }

        ++number;
    }
}
