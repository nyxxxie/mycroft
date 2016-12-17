#ifndef TEMPLATEEDITOR_H
#define TEMPLATEEDITOR_H

#include <QString>
#include <QVariant>
#include <QTreeView>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <mycroft/template.h>

class TemplateEditor;

class TemplateModel : public QAbstractItemModel
{
    Q_OBJECT

    TemplateEditor* p;
    bool shouldRender() const;
    ast_struct_t* getParent(const QModelIndex& parent) const;

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

    void init();

protected:
    template_t* t;

public:
    TemplateEditor(QWidget* parent);

public slots:
    void setFile(mc_file_t* file);
};

#endif // TEMPLATEEDITOR_H
