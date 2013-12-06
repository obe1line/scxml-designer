#include <QLCDNumber>
#include <QDebug>
#include <QDockWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QAbstractTransition>
#include <QHBoxLayout>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "workflowtab.h"

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

    // create a dockable area for the properies
    QDockWidget *dock = new QDockWidget(mCentralWidget);
    dock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    QDockWidget::DockWidgetFeatures dockFeatures = QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetVerticalTitleBar;
    dock->setFeatures(dockFeatures);
    dock->setWindowTitle("Worfklow properties and settings");

    mHorizontalLayout = new QHBoxLayout(mCentralWidget);
    mHorizontalLayout->setSpacing(6);
    mHorizontalLayout->setContentsMargins(11, 11, 11, 11);

    mTabWidget = new QTabWidget(dock);
    mTabWidget->setTabsClosable(true);
    mTabWidget->setCurrentIndex(-1);

    mHorizontalLayout->addWidget(mTabWidget);

    addDockWidget(Qt::RightDockWidgetArea, dock);
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
    QObject::connect(mActionNew, SIGNAL(triggered()), this, SLOT(createWorkflow()));

    mActionOpen = new QAction(tr("&Open"), this);
    mActionOpen->setIcon(QIcon("://images/open.png"));
    mActionOpen->setStatusTip(tr("Open a layout"));
    QObject::connect(mActionOpen, SIGNAL(triggered()), this, SLOT(loadWorkflow()));

    mActionSave = new QAction(tr("&Save"), this);
    mActionSave->setIcon(QIcon("://images/save.png"));
    mActionSave->setStatusTip(tr("Save the current layout"));
    QObject::connect(mActionSave, SIGNAL(triggered()), this, SLOT(saveCurrentWorkflow()));

    mActionState = new QAction(tr("&State"), this);
    mActionState->setIcon(QIcon("://images/new.png"));
    mActionState->setStatusTip(tr("Add a new state"));
    QObject::connect(mActionState, SIGNAL(triggered()), this, SLOT(insertState()));

    mActionTransition = new QAction(tr("&Transition"), this);

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

    statusBar()->showMessage("Status here");
}

//!
//! \brief MainWindow::insertState
//! Add a new state to the workflow with a default metadata describing the dimensions
//!
void MainWindow::insertState()
{
    WorkflowTab* activeTab = getActiveWorkflowTab();
    if (activeTab == NULL) return;
    Workflow* activeWorkflow = activeTab->GetWorkflow();
    int nodeCount = activeWorkflow->children().length();
    SCXMLState *newState = new SCXMLState(QString("state_%1").arg(nodeCount));
    QMap<QString,QString> metaData;
    metaData["x"] = "10";
    metaData["y"] = "10";
    metaData["height"] = "50";
    metaData["width"] = "100";
    metaData["description"] = "new state";
    newState->ApplyMetaData(&metaData);
    activeWorkflow->addState(newState);
    activeWorkflow->setInitialState(newState);
    activeTab->AddStateToScene(newState);
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
    }
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    Q_UNUSED(index)
    //TODO: close the tab after save check
    mTabWidget->removeTab(index);
}

//!
//! \brief Create a new blank workflow
//!
WorkflowTab* MainWindow::createWorkflow()
{
    WorkflowTab* tab = new WorkflowTab(mTabWidget, "");
    int index = mTabWidget->addTab(tab, "Unnamed");
    mTabWidget->setCurrentIndex(index);
    return tab;
}

WorkflowTab *MainWindow::getActiveWorkflowTab()
{
    QWidget* activeWidget = mTabWidget->currentWidget();
    if (activeWidget == NULL) return NULL;
    return dynamic_cast<WorkflowTab*>(activeWidget);
}

