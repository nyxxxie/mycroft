#include <mycroft/context.h>
#include <mycroft/project.h>
#include "projectview.h"


//TODO: use this along with QModelIndex::column to determine what kind of thing a column is


enum {
    COLTYPE_PROJECT = 0,
    COLTYPE_FILE = 1
};

ProjectView::ProjectView(QWidget* parent)
    : QTreeView(parent)
{
    this->model = new ProjectModel(this);
    setModel(this->model);
}

ProjectView::~ProjectView()
{
    delete this->model;
}

void ProjectView::setContext(mc_ctx_t* ctx)
{
    this->ctx = ctx;
}

bool ProjectModel::shouldRender() const
{
    return (getContext() != NULL);
}

void* ProjectModel::getParent(const QModelIndex& parent) const
{
}

mc_ctx_t* ProjectModel::getContext() const
{
    return treeview->ctx;
}

bool ProjectModel::isContext(const QModelIndex& index) const
{
    return (!index.parent().isValid() && index.internalPointer() == getContext());
}

bool ProjectModel::isProject(const QModelIndex& index) const
{
    return (index.parent().isValid() && index.parent().internalPointer() == getContext());
}

bool ProjectModel::isFile(const QModelIndex& index) const
{
    return (index.parent().isValid() && isProject(index.parent()));
}

ProjectModel::ProjectModel(ProjectView* parent)
    : QAbstractItemModel(parent)
{
    treeview = parent;
}

ProjectModel::~ProjectModel()
{
}

int ProjectModel::columnCount(const QModelIndex& parent) const
{
    if (isContext(parent)) {
        return mc_ctx_get_project_amount((mc_ctx_t*)parent.internalPointer());
    }
    else if (isProject(parent)) {
        return mc_project_get_file_amount((mc_project_t*)parent.internalPointer());
    }
    else if (isFile(parent)) {
        return 0;
    }
    else {
        return 0;
    }
}

QVariant ProjectModel::data(const QModelIndex& index, int role) const
{
    if (!shouldRender()) {
        return QVariant();
    }

    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {

        /* */
        if (isContext(index)) {
            return "ASDF";
        }
        else if (isProject(index)) {
            return "project";
        }
        else if (isFile(index)) {
            return mc_file_name((mc_file_t*)index.internalPointer());
        }
        else {
            return 0;
        }
    }
}

Qt::ItemFlags ProjectModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant ProjectModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0:
            return QString("Name");
        }
    }

    return QVariant();
}

QModelIndex ProjectModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!shouldRender()) {
        return QModelIndex();
    }

    if (!parent.isValid()) {
        return QModelIndex();
    }

    void* tmp = (void*)parent.internalPointer();
    if (tmp == NULL) {
        return QModelIndex();
    }

    /* */
    switch (column) {
    case 0:
        return createIndex(row, column, mc_ctx_get_project((mc_ctx_t*)tmp, row));
    case 1:
        return createIndex(row, column, mc_project_get_file((mc_project_t*)tmp, row));
    default:
        return QModelIndex();
    }
}

QModelIndex ProjectModel::parent(const QModelIndex& index) const
{
    if (!shouldRender()) {
        return QModelIndex();
    }

    if (!index.isValid()) {
        return QModelIndex();
    }

    if (isContext(index)) {
        return QModelIndex();
    }

    if (isProject(index)) {
        return QModelIndex();
    }

    /* Index points to a file, find the project it belongs to.  Hacky and
       inefficient I know, spare me your wrath. */
    for (int i=0; i < mc_ctx_get_project_amount(getContext()); i++) {

        /* Get project for this iteration */
        mc_project_t* project = mc_ctx_get_project(getContext(), i);
        if (project == NULL) {
            MC_ERROR("Encountered NULL project [i=%i]\n", i);
            return QModelIndex();
        }

        /* Loop through each file in the project */
        for (int j=0; j < mc_project_get_file_amount(project); j++) {

            /* Get file for this iteration */
            mc_file_t* file = mc_project_get_file(project, i);
            if (file == NULL) {
                MC_ERROR("Encountered NULL file in project [i=%i, j=%i]\n", i, j);
                return QModelIndex();
            }

            /*  */
            if (index.internalPointer() == file) {
                return createIndex(i, COLTYPE_PROJECT, file);
            }
        }
    }

    return QModelIndex();
}

int ProjectModel::rowCount(const QModelIndex& parent) const
{
    if (!shouldRender()) {
        return 0;
    }

    return 0;
}
