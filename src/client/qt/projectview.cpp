/*****************************************************************************\
Copyright (C) 2016-2017 Nyxxie <github.com/nyxxxie>

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.
This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 51 Franklin
Street, Fifth Floor, Boston, MA  02110-1301, USA.
\*****************************************************************************/

#include <mycroft/mycroft.h>
#include <mycroft/project.h>
#include <mycroft/file.h>
#include "projectview.h"

ProjectView::ProjectView(QWidget* parent)
    : QTreeView(parent)
{
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
    MC_DEBUG("double click\n");
    if (index.isValid()) {
        if (!index.parent().isValid()) {
            emit focusProject((mc_project_t*)index.internalPointer());
        }
        else if (index.parent().isValid()) {
            emit focusFile((mc_project_t*)index.parent().internalPointer(), (mc_file_t*)index.internalPointer());
            emit focusProject((mc_project_t*)index.parent().internalPointer());
        }
    }

    viewport()->update();
}

void ProjectView::onContextMenuRequested(const QPoint& point)
{
    MC_DEBUG("right click menuu\n");
    QModelIndex index = indexAt(point);
    if (!index.parent().isValid()) {
        QMenu menu;
        menu.addMenu("Add");
        menu.exec(mapToGlobal(point));
    }
    else {
        QMenu menu;
        menu.addMenu("Open");
        menu.exec(mapToGlobal(point));
    }
}

void ProjectView::fileAdded(mc_project_t* proj, mc_file_t* file)
{
    /* When a new file is added, we should show that immediately in the view */
    MC_DEBUG("File added, updating projectview...\n");
    viewport()->update();
}

void ProjectView::projectAdded(mc_project_t* proj)
{
    /* When a new project is added, we should show that immediately in the view */
    MC_DEBUG("Project added, updating projectview...\n");
    viewport()->update();
}

bool ProjectModel::shouldRender() const
{
    return true;
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
        if (index.parent().isValid()) {
            /* */
            mc_file_t* file = (mc_file_t*)index.internalPointer();

            /* */
            return mc_file_name(file);
        }
        else {

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
        return createIndex(row, column, mc_get_project(row));
    }
    else {
        mc_project_t* project = (mc_project_t*)parent.internalPointer();
        return createIndex(row, column, mc_project_get_file(project, row));
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
        return mc_get_project_amount();
    }
    else if (!parent.parent().isValid()) {
        mc_project_t* project = (mc_project_t*)parent.internalPointer();
        return mc_project_get_file_amount(project);
    }
}
