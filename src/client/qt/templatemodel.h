#ifndef TEMPLATEMODEL_H
#define TEMPLATEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <mycroft/template.h>
#include "templatenode.h"


class TemplateModel : public QAbstractItemModel {

    Q_OBJECT

    template_t* templ;

    ast_node_t* getItem(const QModelIndex& index) const;

public:

    explicit TemplateModel(QObject* parent = 0);
    ~TemplateModel();

    QVariant data(const QModelIndex& index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex& index) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex& child) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
};

#endif // TEMPLATEMODEL_H
