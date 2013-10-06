#include "workflowtab.h"

WorkflowTab::WorkflowTab(QWidget *parent, QString filename) :
    WorkflowSurface(parent), mFilename(filename)
{
    mTabWidget = dynamic_cast<QTabWidget*>(parent);
    mScene = new QGraphicsScene();

    // create a scene for the view
    GetSurface()->setScene(mScene);
}

void WorkflowTab::SetFilename(QString filename)
{
    mFilename = filename;

    // set the tooltip to show the filename
    int tabIndex = mTabWidget->indexOf(this);
    mTabWidget->setTabToolTip(tabIndex, filename);
}

void WorkflowTab::SetWorkflowName(QString workflowName)
{
    mWorkflowName = workflowName;

    // set the tab title to show the workflow name
    int tabIndex = mTabWidget->indexOf(this);
    mTabWidget->setTabText(tabIndex, workflowName);
}

void WorkflowTab::Update()
{
    QString scxml = mWorkflow.GetRawSCXML();
    SetSCMLText(scxml);
    GetWorkflow()->CreateSceneObjects(mScene);
}
