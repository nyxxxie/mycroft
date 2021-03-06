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

#ifndef TEMPLATEEDITOR_H
#define TEMPLATEEDITOR_H

#include <QVariant>
#include <QTreeView>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <mycroft/template.h>

class TemplateEditor;

class TemplateModel : public QAbstractItemModel
{
    Q_OBJECT

    TemplateEditor* treeview;
    bool shouldRender() const;
    ast_struct_t* getParent(const QModelIndex& parent) const;
    template_t* getTemplate() const;

public:
    explicit TemplateModel(TemplateEditor* parent);
    ~TemplateModel();

    QVariant data(const QModelIndex& index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex& index) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column,
                      const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex& index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
};

class TemplateEditor : public QTreeView
{
    friend class TemplateModel;

    Q_OBJECT

protected:
    template_t* t;
    TemplateModel* model;

public:
    TemplateEditor(QWidget* parent);
    ~TemplateEditor();

public slots:
    void setFile(mc_file_t* file);
};

#endif // TEMPLATEEDITOR_H
