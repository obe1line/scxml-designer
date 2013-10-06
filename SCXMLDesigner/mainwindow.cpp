#include <QLCDNumber>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QAbstractTransition>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "workflowtab.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    mUI(new Ui::MainWindow)
{
    mUI->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete mUI;
}

void MainWindow::insertState()
{
    WorkflowTab* activeTab = getActiveWorkflowTab();
    if (activeTab == NULL) return;
    Workflow* activeWorkflow = activeTab->GetWorkflow();
    int nodeCount = activeWorkflow->children().length();
    SCXMLState *newState = new SCXMLState(QString("state_%1").arg(nodeCount));
    activeWorkflow->addState(newState);
    activeWorkflow->setInitialState(newState);

    //TODO: remove this... temporary code
    setLCDValue(activeWorkflow->children().length());
    showStates();
}

//FIXME: remove this... temporary code
void MainWindow::showStates()
{
    WorkflowTab* activeTab = getActiveWorkflowTab();
    if (activeTab == NULL) return;
    Workflow* activeWorkflow = activeTab->GetWorkflow();
    mUI->listOfStates->clear();
    foreach(QObject* child, activeWorkflow->children()) {
        SCXMLState* state = dynamic_cast<SCXMLState*>(child);
        qDebug() << state->GetId();
        mUI->listOfStates->addItem(state->GetId());

        foreach(QAbstractTransition* trans, state->transitions()) {
            mUI->listOfStates->addItem(QString(" (T) %1").arg("trans"));
            foreach(QAbstractState* targetTrans, trans->targetStates()) {
                mUI->listOfStates->addItem(QString(" (T+) %1").arg("target"));
            }
        }
    }
}

//!
//! \brief Saves the current workflow to a file in SCXML format
//!
void MainWindow::saveCurrentWorkflow()
{
    QFileDialog fileSelector(this);
    fileSelector.setWindowTitle(tr("Save SCXML workflow"));
    fileSelector.setNameFilter(tr("SCXML Files (*.scxml);;All Files (*.*)"));
    fileSelector.setFileMode(QFileDialog::AnyFile);
    fileSelector.setViewMode(QFileDialog::Detail);
    fileSelector.setAcceptMode(QFileDialog::AcceptSave);
    fileSelector.setDefaultSuffix(tr("scxml"));
    if (fileSelector.exec() && !fileSelector.selectedFiles().isEmpty()) {
        QString workflowFilename = fileSelector.selectedFiles().first();
        //FIXME: get active tab and call save funtion on it
        // for now, just write a test file
        QDomDocument doc;
        WorkflowTab* activeTab = getActiveWorkflowTab();
        activeTab->GetWorkflow()->ConstructSCXMLFromStateMachine(doc);

        QFile scxmlFile(workflowFilename);
        if (!scxmlFile.open(QIODevice::Truncate | QIODevice::WriteOnly)) {
            Utilities::ShowWarning("SCXML file cannot be written");
            return;
        }
        QByteArray xml = doc.toByteArray();
        scxmlFile.write(xml);
        scxmlFile.close();
    }
}

//!
//! \brief Loads an SCXML file as a new workflow
//!
void MainWindow::loadWorkflow()
{
    QFileDialog fileSelector(this);
    fileSelector.setWindowTitle(tr("Open SCXML workflow"));
    fileSelector.setNameFilter(tr("SCXML Files (*.scxml);;All Files (*.*)"));
    fileSelector.setFileMode(QFileDialog::AnyFile);
    fileSelector.setViewMode(QFileDialog::Detail);
    fileSelector.setAcceptMode(QFileDialog::AcceptOpen);
    fileSelector.setDefaultSuffix(tr("scxml"));
    if (fileSelector.exec()) {
        QString workflowFilename = fileSelector.selectedFiles().first();
        QDomDocument doc;
        QFile scxmlFile(workflowFilename);
        if (!scxmlFile.open(QIODevice::ReadOnly)) {
            Utilities::ShowWarning("SCXML file cannot be read");
            return;
        }
        if (!doc.setContent(&scxmlFile)) {
            qDebug() << doc.lineNumber();
            qDebug() << doc.columnNumber();
            Utilities::ShowWarning("SCXML file cannot be parsed");
            scxmlFile.close();
            return;
        }
        scxmlFile.close();


        // create a new tab and add the workflow to it
        WorkflowTab* newTab = createWorkflow();
        newTab->SetFilename(workflowFilename);
        newTab->GetWorkflow()->ConstructStateMachineFromSCXML(doc);
        newTab->SetWorkflowName(newTab->GetWorkflow()->GetWorkflowName());
        newTab->Update();

        //TODO: remove this when no longer needed
        setLCDValue(newTab->GetWorkflow()->children().length());
    }
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    Q_UNUSED(index)
    //TODO: close the tab after save check
    mUI->tabWidget->removeTab(index);
}

//!
//! \brief Create a new blank workflow
//!
WorkflowTab* MainWindow::createWorkflow()
{
    WorkflowTab* tab = new WorkflowTab(mUI->tabWidget, "");
    int index = mUI->tabWidget->addTab(tab, "Unnamed");
    mUI->tabWidget->setCurrentIndex(index);
    return tab;
}

WorkflowTab *MainWindow::getActiveWorkflowTab()
{
    QWidget* activeWidget = mUI->tabWidget->currentWidget();
    if (activeWidget == NULL) return NULL;
    return dynamic_cast<WorkflowTab*>(activeWidget);
}

