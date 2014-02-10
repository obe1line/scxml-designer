#include <QModelIndex>
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

// test only
void WorkflowTab::TestDataModel(QTableWidget *dataView)
{
    SCXMLDataModel* dataModel = GetWorkflow()->GetDataModel();

    foreach (SCXMLDataItem* dataItem, dataModel->GetDataItemList()) {
        dataView->insertRow(0);
        dataView->setItem(0, 0, new QTableWidgetItem(dataItem->GetId()));
        dataView->setItem(0, 1, new QTableWidgetItem(dataItem->GetExpr()));
        dataView->setItem(0, 2, new QTableWidgetItem(dataItem->GetSrc()));
    }
}

void WorkflowTab::AddItemToScene(QGraphicsItem *item)
{
    mScene->addItem(item);
}
