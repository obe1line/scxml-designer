#ifndef WORKFLOWTAB_H
#define WORKFLOWTAB_H

#include <QTabWidget>
#include "workflow.h"

class WorkflowTab : public QWidget
{
    Q_OBJECT
public:
    explicit WorkflowTab(QWidget *parent = 0, QString filename = "");
    Workflow* GetWorkflow() { return &mWorkflow; }
    QString GetFilename() { return mFilename; }
    void SetFilename(QString filename);

private:
    inline QTabWidget* GetParentTabWidget() { return dynamic_cast<QTabWidget*>(parent()); }

signals:
    
public slots:
    
private:
    Workflow mWorkflow;
    QTabWidget *mTabWidget;
    QString mFilename;
};

#endif // WORKFLOWTAB_H
