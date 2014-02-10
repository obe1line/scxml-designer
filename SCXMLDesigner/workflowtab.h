#ifndef WORKFLOWTAB_H
#define WORKFLOWTAB_H

#include <QTabWidget>
#include <QTableWidget>

#include "workflow.h"
#include "workflowsurface.h"

class WorkflowTab : public WorkflowSurface
{
    Q_OBJECT
public:
    explicit WorkflowTab(QWidget *parent = 0, QString filename = "");
    Workflow* GetWorkflow() { return &mWorkflow; }
    QString GetFilename() { return mFilename; }
    void SetFilename(QString filename);
    void SetWorkflowName(QString workflowName);
    void Update();
    void AddItemToScene(QGraphicsItem *item);

    void TestDataModel(QTableWidget *dataView);
private:
    inline QTabWidget* GetParentTabWidget() { return dynamic_cast<QTabWidget*>(parent()); }

signals:
    
public slots:
    
private:
    Workflow mWorkflow;
    QTabWidget *mTabWidget;
    QString mFilename;
    QString mWorkflowName;
    QGraphicsScene* mScene ;
};

#endif // WORKFLOWTAB_H
