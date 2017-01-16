#ifndef PROJECTVIEW_H
#define PROJECTVIEW_H

#include <QVariant>
#include <QTreeView>
#include <QModelIndex>
#include <QMenu>
#include <QAbstractItemModel>
#include <mycroft/project.h>

class ProjectView;

class ProjectModel : public QAbstractItemModel
{
    Q_OBJECT

private:
    ProjectView* treeview;

    bool shouldRender() const;
    void* getParent(const QModelIndex& parent) const;

public:
    explicit ProjectModel(ProjectView* parent);
    ~ProjectModel();

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

class ProjectView : public QTreeView
{
    friend class ProjectModel;
    Q_OBJECT

protected:
    ProjectModel* model;

public:
    ProjectView(QWidget* parent);
    ~ProjectView();

signals:
    void focusFile(mc_project_t* project, mc_file_t* file);
    void focusProject(mc_project_t* project);

private slots:
    void indexDoubleClicked(const QModelIndex& index);
    void onContextMenuRequested(const QPoint& point);
    void fileAdded(mc_project_t* proj, mc_file_t* file);
    void projectAdded(mc_project_t* proj);
};

#endif // PROJECTVIEW_H
