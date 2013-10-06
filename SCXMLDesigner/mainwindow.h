#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStateMachine>
#include <QDomDocument>
#include "scxmlstate.h"
#include "workflow.h"
#include "workflowtab.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void ShowWarning(QString msg);

signals:
    void setLCDValue(int value);

private slots:
    void on_tabWidget_tabCloseRequested(int index);
    void insertState();
    void showStates();
    void saveCurrentWorkflow();
    void loadWorkflow();
    WorkflowTab* createWorkflow();
    WorkflowTab* getActiveWorkflowTab();

private:
    Ui::MainWindow *mUI;
};

#endif // MAINWINDOW_H
