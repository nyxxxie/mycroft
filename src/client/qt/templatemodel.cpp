#include <QColor>
#include "templateeditor.h"
#include "templatemodel.h"

template_t* TemplateModel::getCurTemplate() const {
    return p->getCurTemplate();
}

TemplateModel::TemplateModel(TemplateEditor* parent) {
    p = parent;
}

TemplateModel::~TemplateModel() {
}

int TemplateModel::columnCount(const QModelIndex& parent) const {
    if (parent.isValid()) {
        ast_node_t* node = (ast_node_t*)(parent.internalPointer());
        if (node == NULL) {
            fprintf(stderr, "%s: Node was null.\n", __func__);
            return 0;
        }


        /* Return data */
        switch (node->type) {
        case AST_TYPE_STRUCT:
            return ((ast_struct_t*)node)->node_amt;
        case AST_TYPE_VAR:
            return 0;
        default:
            printf("%s: node type %i was not handled.\n", __func__, node->type);
        }
    }
    else {
        return 1;
    }
}

QVariant TemplateModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        /* Get node */
        ast_node_t* node = (ast_node_t*)(index.internalPointer());
        if (node == NULL) {
            fprintf(stderr, "%s: Node was null.\n", __func__);
            return 0;
        }

        /* Return data */
        switch (node->type) {
        case AST_TYPE_STRUCT:
            return ((ast_struct_t*)node)->name;
        case AST_TYPE_VAR:
            return ((ast_var_t*)node)->name;
        default:
            printf("%s: node type %i was not handled.\n", __func__, node->type);
        }
    }
    else if (role == Qt::BackgroundRole) {
        if (index.row() % 2 == 0) {
            return QVariant(QColor(226, 237, 253));
        }
        else {
            return QVariant(QColor(Qt::white));
        }
    }

    return QVariant();
}

Qt::ItemFlags TemplateModel::flags(const QModelIndex& index) const {
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant TemplateModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0:
            return QString("Name");
        case 1:
            return QString("Value");
        }
    }

    return QVariant();
}

QModelIndex TemplateModel::index(int row, int column, const QModelIndex& parent) const {
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    ast_struct_t* strct = NULL;
    if (!parent.isValid())
        strct = getCurTemplate()->root->entry;
    else
        strct = (ast_struct_t*)(parent.internalPointer());

    ast_node_t* node = strct->nodes[row];
    if (node == NULL)
        return QModelIndex();

    return createIndex(row, column, node);
}

QModelIndex TemplateModel::parent(const QModelIndex& index) const {
    if (!index.isValid())
        return QModelIndex();

    ast_var_t* child = (ast_var_t*)(index.internalPointer());
    ast_struct_t* parent = (ast_struct_t*)child->parent;

    if (((void*)parent) == ((void*)getCurTemplate()->root)) //TODO: this will likely fail, modify template code
        return QModelIndex();

    return createIndex(parent->index, 0, parent);
}

int TemplateModel::rowCount(const QModelIndex& parent) const {
    if (parent.column() > 0)
        return 0;

    ast_struct_t* strct = NULL;
    if (!parent.isValid())
        strct = getCurTemplate()->root->entry;
    else
        strct = (ast_struct_t*)(parent.internalPointer());

    return strct->node_amt;
}
