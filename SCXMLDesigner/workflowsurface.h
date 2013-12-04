#ifndef WORKFLOWSURFACE_H
#define WORKFLOWSURFACE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QGraphicsView>
#include "workflowgraphicsview.h"


class WorkflowSurface : public QWidget
{
    Q_OBJECT
    
public:
    explicit WorkflowSurface(QWidget *parent = 0);
    ~WorkflowSurface();

    void SetSCMLText(QString scxml);
    QGraphicsView *GetSurface();

    void CreateWidgets();

private:
    QVBoxLayout* verticalLayout;
    QSplitter* splitter;
    WorkflowGraphicsView* graphicsView;
    QTextEdit* textSCXML;};

#endif // WORKFLOWSURFACE_H
