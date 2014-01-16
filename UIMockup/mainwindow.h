#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "chaikincurve.h"

#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLCDNumber>
#include <QListWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QTabWidget>
#include <QToolBar>
#include <QToolBox>
#include <QWidget>
#include <QTableWidget>
#include <QMainWindow>
#include <QStateMachine>
#include <QDomDocument>
#include <QGraphicsView>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void CreateWidgets();
    void TempCreate();
    void CreateMenus();
    void CreateActions();
    void CreateToolbars();

public slots:
    void increase() { mChaikinCurve->IncreaseLod(); mChaikinCurve->update(); }
    void decrease() { mChaikinCurve->DecreaseLod(); mChaikinCurve->update(); }

private:
    QMenu *mMenuFile;

    QAction *mActionExit;
    QAction *mActionIncrease;
    QAction *mActionDecrease;

    QToolBar *mFileToolBar;

    QWidget *mCentralWidget;
    QHBoxLayout *mHorizontalLayout;

    QGraphicsScene *mScene;
    QGraphicsView *mMainView;

    ChaikinCurve *mChaikinCurve;
};

#endif // MAINWINDOW_H
