#include "templatemodel.h"

TemplateModel::TemplateModel(QObject* parent)
    : QAbstractItemModel(parent) {
    templ = template_create_from_file("testtemplate.mtf");
}

TemplateModel::~TemplateModel() {

}

ast_node_t* TemplateModel::getItem(const QModelIndex& index) const {
    if (index.isValid()) {
        ast_node_t* node = (ast_node_t*)index.internalPointer();
        if (node != NULL) {
            return node;
        }
    }

    return (ast_node_t*)templ->entry;
}

QVariant TemplateModel::data(const QModelIndex& index, int role) const {
    printf("Call on meeeeee.\n");
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    ast_node_t* node = (ast_node_t*)index.internalPointer();
    if (node == NULL) {
        printf("data was null!?!?!?!\n");
        return QVariant();
    }

    if (node->type == AST_TYPE_STRUCT) {
        ast_struct_t* strct = (ast_struct_t*)node;
        switch (index.column()) {
        case 0:
            return strct->name;
        case 1:
            return "";
        case 2:
            return "";
        case 3:
            return "";
        default:
            return QVariant();
        }
    }
    else if (node->type == AST_TYPE_VAR) {
        ast_var_t* var = (ast_var_t*)node;
        switch (index.column()) {
        case 0:
            return var->name;
        case 1:
            return "";
        case 2:
            return "";
        case 3:
            return "";
        default:
            return QVariant();
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
            return tr("Name");
        case 1:
            return tr("Value");
        case 2:
            return tr("Start");
        case 3:
            return tr("Size");
        default:
            return QVariant();
        }
    }

    return QVariant();
}

QModelIndex TemplateModel::index(int row, int column, const QModelIndex& parent) const {
    printf("Yikes [r:%i, c:%i]\n", row, column);
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    ast_struct_t* strct = (ast_struct_t*)getItem(parent);
    if (strct == NULL) {
        return QModelIndex();
    }

    ast_node_t* cnode = strct->nodes[row];
    if (cnode == NULL) {
        return QModelIndex();
    }

    return createIndex(row, column, cnode);
}

QModelIndex TemplateModel::parent(const QModelIndex& child) const {
    if (!child.isValid())
        return QModelIndex();

    ast_node_t* node = (ast_node_t*)child.internalPointer();
    if (node != NULL) {
        if (node->type == AST_TYPE_STRUCT) {
            ast_struct_t* strct = (ast_struct_t*)node;
            return createIndex(strct->index, 0, strct->parent);
        }
        else if (node->type == AST_TYPE_VAR) {
            ast_var_t* var = (ast_var_t*)node;
            return createIndex(var->index, 0, var->parent);
        }
    }

    return QModelIndex();
}

int TemplateModel::rowCount(const QModelIndex& parent) const {
    ast_struct_t* strct = (ast_struct_t*)getItem(parent);
    if (strct == NULL) {
        printf("%s: WTF NULL?", __func__);
        return 0;
    }

    return strct->node_amt;
}

int TemplateModel::columnCount(const QModelIndex& parent) const {
    return 4; // Change this if you add more headers to the itemmodel
}
