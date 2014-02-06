#include <QDebug>
#include <QDockWidget>
#include <QFileDialog>

#include "mainwindow.h"
#include "workflowtab.h"
#include "scxmltransition.h"

//!
//! \brief MainWindow::MainWindow
//! \param parent
//! Construct the main window and child widgets
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    CreateWidgets();
    CreateActions();
    CreateMenus();
    CreateToolbars();

    // while testing
    LoadWorkflowFromFile("../SCXMLDesigner/Examples/TestLog.scxml");
}

MainWindow::~MainWindow()
{
}

//!
//! \brief MainWindow::CreateWidgets
//!
void MainWindow::CreateWidgets()
{
    setObjectName(QStringLiteral("MainWindow"));
    resize(664, 415);

    mCentralWidget = new QWidget(this);
    setCentralWidget(mCentralWidget);

    QDockWidget *propertyDock = new QDockWidget(this);
    propertyDock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    QDockWidget::DockWidgetFeatures dockPropertyFeatures = QDockWidget::DockWidgetMovable;
    propertyDock->setFeatures(dockPropertyFeatures);
    propertyDock->setWindowTitle("DataModel");

    // data model properties
    mDataModelTable = new QTableWidget(0, 3);
    mDataModelTable->setStyleSheet("QTableView {selection-background-color: green;}");
    mDataModelTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    mDataModelTable->setSelectionMode(QAbstractItemView::SingleSelection);
    mDataModelTable->setColumnWidth(1, 50);
    QStringList tableHeader;
    tableHeader << "Id" << "Expr" << "Src";
    mDataModelTable->setHorizontalHeaderLabels(tableHeader);
    propertyDock->setWidget(mDataModelTable);

    mHorizontalLayout = new QHBoxLayout(mCentralWidget);
    mHorizontalLayout->setSpacing(6);
    mHorizontalLayout->setContentsMargins(11, 11, 11, 11);

    mTabWidget = new QTabWidget();
    mTabWidget->setTabsClosable(true);
    mTabWidget->setCurrentIndex(-1);

    mHorizontalLayout->addWidget(mTabWidget);

    addDockWidget(Qt::RightDockWidgetArea, propertyDock);
}

//!
//! \brief MainWindow::CreateActions
//!
void MainWindow::CreateActions()
{
    mActionExit = new QAction(tr("E&xit"), this);
    mActionExit->setIcon(QIcon("://images/exit.png"));
    mActionExit->setStatusTip(tr("Exit the application"));
    QObject::connect(mActionExit, SIGNAL(triggered()), this, SLOT(close()));

    mActionNew = new QAction(tr("&New"), this);
    mActionNew->setIcon(QIcon("://images/new.png"));
    mActionNew->setStatusTip(tr("New layout"));
    QObject::connect(mActionNew, SIGNAL(triggered()), this, SLOT(CreateWorkflow()));

    mActionOpen = new QAction(tr("&Open"), this);
    mActionOpen->setIcon(QIcon("://images/open.png"));
    mActionOpen->setStatusTip(tr("Open a layout"));
    QObject::connect(mActionOpen, SIGNAL(triggered()), this, SLOT(LoadWorkflowFromDialog()));

    mActionSave = new QAction(tr("&Save"), this);
    mActionSave->setIcon(QIcon("://images/save.png"));
    mActionSave->setStatusTip(tr("Save the current layout"));
    QObject::connect(mActionSave, SIGNAL(triggered()), this, SLOT(SaveCurrentWorkflow()));

    mActionState = new QAction(tr("&State"), this);
    mActionState->setIcon(QIcon("://images/new.png"));
    mActionState->setStatusTip(tr("Add a new state"));
    QObject::connect(mActionState, SIGNAL(triggered()), this, SLOT(InsertState()));

    mActionTransition = new QAction(tr("&Transition"), this);
    //mActionTransition->setIcon(QIcon("://images/link.png"));
    mActionTransition->setStatusTip(tr("Add a new transition"));
    QObject::connect(mActionTransition, SIGNAL(triggered()), this, SLOT(InsertTransition()));

    mActionAnimate = new QAction(tr("&Animate"), this);
    mActionAnimate->setStatusTip(tr("Animate"));
    QObject::connect(mActionAnimate, SIGNAL(triggered()), this, SLOT(TestAnimation()));

    //TODO: connect these
    mActionAbout = new QAction(tr("&About"), this);
    mActionShowChildStates = new QAction(tr("&Show"), this);
}

//!
//! \brief MainWindow::CreateMenus
//!
void MainWindow::CreateMenus()
{
    mMenuFile = menuBar()->addMenu(tr("&File"));
    mMenuFile->addAction(mActionNew);
    mMenuFile->addAction(mActionOpen);
    mMenuFile->addAction(mActionSave);
    mMenuFile->addSeparator();
    mMenuFile->addAction(mActionExit);

    mMenuEdit = menuBar()->addMenu(tr("&Edit"));

    mMenuHelp = menuBar()->addMenu(tr("&Help"));
    mMenuHelp->addAction(mActionAbout);

    mMenuInsert = menuBar()->addMenu(tr("&Insert"));
    mMenuInsert->addAction(mActionState);
    mMenuInsert->addAction(mActionTransition);
    mMenuInsert->addAction(mActionAnimate);

    mMenuTest = menuBar()->addMenu(tr("&Test"));
    mMenuTest->addAction(mActionShowChildStates);
}

//!
//! \brief MainWindow::CreateToolbars
//!
void MainWindow::CreateToolbars()
{
    mFileToolBar = addToolBar(tr("File"));
    mFileToolBar->addAction(mActionNew);
    mFileToolBar->addAction(mActionOpen);
    mFileToolBar->addAction(mActionSave);

    mInsertToolBar = addToolBar(tr("Insert"));
    mInsertToolBar->addAction(mActionState);
    mInsertToolBar->addAction(mActionTransition);
    mInsertToolBar->addAction(mActionAnimate);

    statusBar()->showMessage(QString("Version: %1").arg(VERSION));
}

//!
//! \brief MainWindow::insertState
//! Add a new state to the workflow with a default metadata describing the dimensions
//!
void MainWindow::InsertState()
{
    WorkflowTab* activeTab = GetActiveWorkflowTab();
    if (activeTab == NULL) return;
    Workflow* activeWorkflow = activeTab->GetWorkflow();
    int nodeCount = activeWorkflow->children().length();
    QMap<QString,QString> metaData;
    metaData["x"] = "10";
    metaData["y"] = "10";
    metaData["height"] = "50";
    metaData["width"] = "100";
    metaData["description"] = "new state";
    SCXMLState *newState = new SCXMLState(QString("state_%1").arg(nodeCount), &metaData);
    activeWorkflow->addState(newState);
    activeWorkflow->setInitialState(newState);
    activeTab->AddItemToScene(newState);
}

void MainWindow::TestAnimation()
{
    WorkflowTab* activeTab = GetActiveWorkflowTab();
    if (activeTab == NULL) return;
    Workflow* activeWorkflow = activeTab->GetWorkflow();
    QObjectList nodes = activeWorkflow->children();
    foreach (QObject *obj, nodes) {
        SCXMLState* state = static_cast<SCXMLState*>(obj);
        QList<QAbstractTransition*> trans = state->transitions();
        foreach (QAbstractTransition *tran, trans) {
            SCXMLTransition* transition = static_cast<SCXMLTransition*>(tran);
            transition->AnimateEvent();
        }
    }
}

//!
//! \brief MainWindow::insertTransition
//! Add a new transition to the workflow between two selected states
//!
void MainWindow::InsertTransition()
{
    WorkflowTab* activeTab = GetActiveWorkflowTab();
    if (activeTab == NULL) return;
    Workflow* activeWorkflow = activeTab->GetWorkflow();
    QObjectList nodes = activeWorkflow->children();
    QList<SCXMLState*> nodesSelected;
    foreach (QObject *obj, nodes) {
        SCXMLState* state = static_cast<SCXMLState*>(obj);
        if (state->isSelected()) nodesSelected.append(state);
    }

    if (nodesSelected.count() == 2) {
        SCXMLState* stateFrom = nodesSelected.at(0);
        SCXMLState* stateTo = nodesSelected.at(1);
        SCXMLTransition* transition = new SCXMLTransition(stateFrom, stateTo, nullptr);

        // add the new transition to the scene
        QGraphicsItem* itemTran = dynamic_cast<QGraphicsItem*>(transition);
        activeTab->AddItemToScene(itemTran);
    }
}

//!
//! \brief Saves the current workflow to a file in SCXML format
//!
void MainWindow::SaveCurrentWorkflow()
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
        QDomDocument doc;
        WorkflowTab* activeTab = GetActiveWorkflowTab();
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
bool MainWindow::LoadWorkflowFromFile(QString workflowFilename) {
    QDomDocument doc;
    QFile scxmlFile(workflowFilename);
    if (!scxmlFile.open(QIODevice::ReadOnly)) {
        Utilities::ShowWarning("SCXML file cannot be read");
        return false;
    }
    if (!doc.setContent(&scxmlFile)) {
        qDebug() << doc.lineNumber();
        qDebug() << doc.columnNumber();
        Utilities::ShowWarning("SCXML file cannot be parsed");
        scxmlFile.close();
        return false;
    }
    scxmlFile.close();

    // create a new tab and add the workflow to it
    WorkflowTab* newTab = CreateWorkflow();
    newTab->SetFilename(workflowFilename);
    newTab->GetWorkflow()->ConstructStateMachineFromSCXML(doc);
    newTab->SetWorkflowName(newTab->GetWorkflow()->GetWorkflowName());
    newTab->Update();
    newTab->TestDataModel(mDataModelTable);

    return true;
}

//!
//! \brief Loads an SCXML file via the file selection dialog
//!
bool MainWindow::LoadWorkflowFromDialog()
{
    QFileDialog fileSelector(this);
    fileSelector.setWindowTitle(tr("Open SCXML workflow"));
    fileSelector.setNameFilter(tr("SCXML Files (*.scxml);;All Files (*.*)"));
    fileSelector.setFileMode(QFileDialog::AnyFile);
    fileSelector.setViewMode(QFileDialog::Detail);
    fileSelector.setAcceptMode(QFileDialog::AcceptOpen);
    fileSelector.setDefaultSuffix(tr("scxml"));
    if (fileSelector.exec()) {
        return LoadWorkflowFromFile(fileSelector.selectedFiles().first());
    }

    return false;
}

void MainWindow::CloseTabRequested(int index)
{
    Q_UNUSED(index)
    //TODO: close the tab after save check
    mTabWidget->removeTab(index);
}

//!
//! \brief Create a new blank workflow
//!
WorkflowTab* MainWindow::CreateWorkflow()
{
    WorkflowTab* tab = new WorkflowTab(mTabWidget, "");
    int index = mTabWidget->addTab(tab, "Unnamed");
    mTabWidget->setCurrentIndex(index);
    connect(mTabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(CloseTabRequested(int)));
    return tab;
}

WorkflowTab *MainWindow::GetActiveWorkflowTab()
{
    QWidget* activeWidget = mTabWidget->currentWidget();
    if (activeWidget == NULL) return NULL;
    return dynamic_cast<WorkflowTab*>(activeWidget);
}

