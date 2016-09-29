#ifndef TEMPLATEMODEL_H
#define TEMPLATEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <mycroft/template.h>

class TemplateEditor;

class TemplateModel : public QAbstractItemModel {

    Q_OBJECT

    TemplateEditor* p;

    template_t* getCurTemplate() const;

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

#endif // TEMPLATEMODEL_H
