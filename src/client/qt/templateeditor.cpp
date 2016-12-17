#include <QFile>
#include <QColor>
#include <mycroft/mycroft.h>
#include "templateeditor.h"

/**
 * Starts up the template viewer with the default struct.
 */
void TemplateEditor::init()
{
    this->t = NULL;
    TemplateModel* model = new TemplateModel(this);
    setModel(model);
}

TemplateEditor::TemplateEditor(QWidget* parent)
    : QTreeView(parent)
{
    init();
}

void TemplateEditor::setFile(mc_file_t* file)
{
    if (file == NULL) {
        this->t = NULL;
    }
    else {
        this->t = mc_file_get_template(file);
    }
}

bool TemplateModel::shouldRender() const
{
    return (p->t != NULL);
}

ast_struct_t* TemplateModel::getParent(const QModelIndex& parent) const
{
    ast_struct_t* ret = NULL;

    if (!parent.isValid())
        ret = p->t->root->entry;
    else
        ret = (ast_struct_t*)(parent.internalPointer());

    return ret;
}

TemplateModel::TemplateModel(TemplateEditor* parent)
{
    p = parent;
}

TemplateModel::~TemplateModel()
{
}

int TemplateModel::columnCount(const QModelIndex& parent) const
{
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

QVariant TemplateModel::data(const QModelIndex& index, int role) const
{
    if (!shouldRender()) {
        return QVariant();
    }

    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        /* Get node */
        ast_node_t* node = (ast_node_t*)(index.internalPointer());
        if (node == NULL) {
            fprintf(stderr, "%s: Node was null.\n", __func__);
            return QVariant();
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

Qt::ItemFlags TemplateModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant TemplateModel::headerData(int section, Qt::Orientation orientation, int role) const
{
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

QModelIndex TemplateModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!shouldRender()) {
        return QModelIndex();
    }

    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    ast_struct_t* strct = getParent(parent);
    if (strct == NULL) {
        return QModelIndex();
    }

    ast_node_t* node = strct->nodes[row];
    if (node == NULL) {
        return QModelIndex();
    }

    return createIndex(row, column, node);
}

QModelIndex TemplateModel::parent(const QModelIndex& index) const
{
    if (!shouldRender()) {
        return QModelIndex();
    }

    if (!index.isValid()) {
        return QModelIndex();
    }

    ast_var_t* child = (ast_var_t*)(index.internalPointer());
    ast_struct_t* parent = (ast_struct_t*)child->parent;

    if (((void*)parent) == ((void*)p->t->root)) {
        return QModelIndex();
    }

    return createIndex(parent->index, 0, parent);
}

int TemplateModel::rowCount(const QModelIndex& parent) const
{
    if (!shouldRender()) {
        return 0;
    }

    if (parent.column() > 0) {
        return 0;
    }

    ast_struct_t* strct = getParent(parent);
    if (strct == NULL) {
        return 0;
    }

    return strct->node_amt;
}
