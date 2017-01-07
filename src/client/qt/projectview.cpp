#include <mycroft/mycroft.h>
#include <mycroft/project.h>
#include <mycroft/file.h>
#include "projectview.h"

ProjectView::ProjectView(QWidget* parent)
    : QTreeView(parent)
{
    MC_DEBUG("asfd: 0x%p\n", parent);

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->model = new ProjectModel(this);
    setModel(this->model);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(onContextMenuRequested(const QPoint&)));
    connect(this, SIGNAL(doubleClicked(const QModelIndex&)),
            this, SLOT(indexDoubleClicked(const QModelIndex&)));
}

ProjectView::~ProjectView()
{
    delete this->model;
}

void ProjectView::indexDoubleClicked(const QModelIndex& index)
{
    if (model->isProject(index)) {
        emit focusProject((mc_project_t*)index.internalPointer());
    }
    else if (model->isFile(index)) {
        emit focusFile((mc_project_t*)index.parent().internalPointer(), (mc_file_t*)index.internalPointer());
        emit focusProject((mc_project_t*)index.parent().internalPointer());
    }

    viewport()->update();
}

void ProjectView::onContextMenuRequested(const QPoint& point)
{
    MC_DEBUG("right click menuu\n");
    QModelIndex index = indexAt(point);
    if (index.isValid()) {
        if (model->isProject(index)) {
            QMenu menu;
            menu.addMenu("Add");
            menu.exec(mapToGlobal(point));
        }
    }
}

bool ProjectModel::shouldRender() const
{
    return true;
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
    return 1; // There's always going to be one column, so we always return 1
}

QVariant ProjectModel::data(const QModelIndex& index, int role) const
{
    if (!shouldRender()) {
        return QVariant();
    }

    if (!index.isValid()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        if (isContext(index)) {
            return QVariant();
        }
        else if (isProject(index)) {

            /* */
            mc_project_t* project = (mc_project_t*)index.internalPointer();

            /* */
            QString ret = mc_project_get_name(project);
            if (project == mc_get_focused_project()) {
                ret += " (focused)";
            }

            /* */
            return ret;
        }
        else if (isFile(index)) {

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
    if (!shouldRender()) {
        return QModelIndex();
    }

    if (!parent.isValid()) {
        return QModelIndex();
    }

    if (isContext(parent)) {
        return QModelIndex();
    }
    else if (isProject(parent)) {

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
    if (!shouldRender()) {
        return QModelIndex();
    }

    if (!index.isValid()) {
        return QModelIndex();
    }

    /* From this point on, the index is either a project, file, or there
       is a bug.  Since we don't have a good way of indicating what an
       index actually contains nor what row that index belongs to on its
       parent, we search for it here.  There is probably a better way to do
       this, and I'll change it when I figure out that way. */
    for (int i=0; i < mc_get_project_amount(); i++) {

        /* Get project for this iteration */
        mc_project_t* project = mc_get_project(i);
        if (project == NULL) {
            MC_ERROR("Encountered NULL project [i=%i]\n", i);
            return QModelIndex();
        }

        /* Check to see if current project is what the index points to */
        if (index.internalPointer() == project) {
            return QModelIndex();
        }

        /* Loop through each file in the project */
        for (int j=0; j < mc_project_get_file_amount(project); j++) {

            /* Get file for this iteration */
            mc_file_t* file = mc_project_get_file(project, j);
            if (file == NULL) {
                MC_ERROR("Encountered NULL file in project [i=%i, j=%i]\n", i, j);
                return QModelIndex();
            }

            /* Check to see if current file is what the index points to */
            if (index.internalPointer() == file) {
                return createIndex(j, 0, project);
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

    if (parent.column() > 0) {
        return 0;
    }

    if (!parent.isValid()) {
        return 0;
    }

    if (isContext(parent)) {
        return mc_get_project_amount();
    }
    else if (isProject(parent)) {

        /* */
        mc_project_t* project = (mc_project_t*)parent.internalPointer();

        /* */
        return mc_project_get_file_amount(project);
    }
    else if (isFile(parent)) {
        return 0;
    }
    else {
        MC_ERROR("No idea what the parent is.\n");
        return 0;
    }
}
