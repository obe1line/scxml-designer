#include <QDebug>

#include "mainwindow.h"

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

    mScene = new QGraphicsScene();
    mMainView = new QGraphicsView();
    mMainView->setScene(mScene);
    setCentralWidget(mMainView);

    mChaikinCurve = new ChaikinCurve(4);
    mMainView->scene()->addItem(mChaikinCurve);
}

//!
//! \brief MainWindow::CreateActions
//!
void MainWindow::CreateActions()
{
    mActionExit = new QAction(tr("E&xit"), this);
    mActionExit->setStatusTip(tr("Exit the application"));
    QObject::connect(mActionExit, SIGNAL(triggered()), this, SLOT(close()));

    mActionIncrease = new QAction(tr("Increase"), this);
    mActionIncrease->setStatusTip(tr("Increase curve points"));
    QObject::connect(mActionIncrease, SIGNAL(triggered()), this, SLOT(increase()));

    mActionDecrease = new QAction(tr("Decrease"), this);
    mActionDecrease->setStatusTip(tr("Decrease curve points"));
    QObject::connect(mActionDecrease, SIGNAL(triggered()), this, SLOT(decrease()));
}

//!
//! \brief MainWindow::CreateMenus
//!
void MainWindow::CreateMenus()
{
    mMenuFile = menuBar()->addMenu(tr("&File"));
    mMenuFile->addAction(mActionIncrease);
    mMenuFile->addAction(mActionDecrease);
    mMenuFile->addAction(mActionExit);
}

//!
//! \brief MainWindow::CreateToolbars
//!
void MainWindow::CreateToolbars()
{
    mFileToolBar = addToolBar(tr("File"));
}
