#include <mycroft/context.h>
#include <mycroft/project.h>
#include "projectview.h"

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
    MC_DEBUG("Called setContext.\n");
    MC_DEBUG("\tSet context. [ctx=0x%p]\n", ctx);
    this->ctx = ctx;
}

bool ProjectModel::shouldRender() const
{
    return (getContext() != NULL);
}

mc_ctx_t* ProjectModel::getContext() const
{
    return treeview->ctx;
}

bool ProjectModel::isContext(const QModelIndex& index) const
{
    return (!index.parent().isValid());
}

bool ProjectModel::isProject(const QModelIndex& index) const
{
    return (index.parent().isValid() && !index.parent().parent().isValid());
}

bool ProjectModel::isFile(const QModelIndex& index) const
{
    return (index.parent().isValid() && index.parent().parent().isValid());
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
    MC_DEBUG("Called columnCount.\n");

    /* */
    return 1;
}

QVariant ProjectModel::data(const QModelIndex& index, int role) const
{
    MC_DEBUG("Called data.\n");
    if (!shouldRender()) {
        return QVariant();
    }

    if (!index.isValid()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        if (isContext(index)) {
            MC_DEBUG("\tIs project.\n");

            /* */
            return "CONTEXT";
        }
        else if (isProject(index)) {
            MC_ERROR("\tIs project.\n");

            /* */
            mc_project_t* project = (mc_project_t*)index.internalPointer();

            /* */
            return mc_project_get_name(project);
        }
        else if (isFile(index)) {
            MC_ERROR("Parent is a file, which is a lie.\n");

            /* */
            mc_file_t* file = (mc_file_t*)index.internalPointer();

            /* */
            return mc_file_name(file);
        }
        else {
            MC_ERROR("No idea what the parent is.\n");
            return QModelIndex();
        }
    }

    return QVariant();
}

Qt::ItemFlags ProjectModel::flags(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return 0;
    }

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
    MC_DEBUG("Called index [r=%i, c=%i].\n", row, column);
    if (!shouldRender()) {
        return QModelIndex();
    }

    if (!parent.isValid()) {
        MC_DEBUG("\tIs root.\n");

        /* */
        mc_ctx_t* ctx = getContext();

        /* */
        return createIndex(row, column, ctx);
    }

    if (isContext(parent)) {
        MC_DEBUG("\tIs project.\n");

        /* */
        mc_ctx_t* ctx = (mc_ctx_t*)parent.internalPointer();

        /* */
        return createIndex(row, column, mc_ctx_get_project(ctx, row));
    }
    else if (isProject(parent)) {
        MC_ERROR("\tIs project.\n");

        /* */
        mc_project_t* project = (mc_project_t*)parent.internalPointer();

        /* */
        return createIndex(row, column, mc_project_get_file(project, row));
    }
    else if (isFile(parent)) {
        MC_ERROR("Parent is a file, which is a lie.\n");
        return QModelIndex();
    }
    else {
        MC_ERROR("No idea what the parent is.\n");
        return QModelIndex();
    }
}

QModelIndex ProjectModel::parent(const QModelIndex& index) const
{
    MC_DEBUG("Called parent.\n");
    if (!shouldRender()) {
        return QModelIndex();
    }

    if (!index.isValid()) {
        return QModelIndex();
    }

    if (index.internalPointer() == getContext()) {
        return QModelIndex();
    }
    else {
        for (int i=0; i < mc_ctx_get_project_amount(getContext()); i++) {

            /* Get project for this iteration */
            mc_project_t* project = mc_ctx_get_project(getContext(), i);
            if (project == NULL) {
                MC_ERROR("Encountered NULL project [i=%i]\n", i);
                return QModelIndex();
            }

            /*  */
            if (index.internalPointer() == project) {
                MC_DEBUG("\tCreating index for row %i.\n", i);
                return createIndex(i, 0, getContext());
            }

            /* Loop through each file in the project */
            for (int j=0; j < mc_project_get_file_amount(project); j++) {

                /* Get file for this iteration */
                mc_file_t* file = mc_project_get_file(project, j);
                if (file == NULL) {
                    MC_ERROR("Encountered NULL file in project [i=%i, j=%i]\n", i, j);
                    return QModelIndex();
                }

                /*  */
                if (index.internalPointer() == file) {
                    return createIndex(j, 0, project);
                }
            }
        }
    }

    return QModelIndex();
}

int ProjectModel::rowCount(const QModelIndex& parent) const
{
    MC_DEBUG("Called rowCount.\n");
    if (!shouldRender()) {
        return 0;
    }

    if (parent.column() > 0) {
        return 0;
    }

    if (!parent.isValid()) {
        MC_DEBUG("parent appears to be invalid...\n");

        /* */
        mc_ctx_t* ctx = (mc_ctx_t*)getContext();

        /* */
        return 1;
    }

    if (isContext(parent)) {
        MC_DEBUG("parent appears to be context...\n");

        /* */
        mc_ctx_t* ctx = (mc_ctx_t*)parent.internalPointer();

        /* */
        return mc_ctx_get_project_amount(ctx);
    }
    else if (isProject(parent)) {
        MC_DEBUG("parent appears to be project...\n");

        /* */
        mc_project_t* project = (mc_project_t*)parent.internalPointer();

        /* */
        return mc_project_get_file_amount(project);
    }
    else if (isFile(parent)) {
        MC_ERROR("Parent is a file, which is a lie.\n");
        return 0;
    }
    else {
        MC_ERROR("No idea what the parent is.\n");
        return 0;
    }
}
