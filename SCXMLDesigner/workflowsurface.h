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
    void AppendToTextOutput(QString txt);
    QGraphicsView *GetSurface();

    void CreateWidgets();

public slots:
    void WriteToOutput(QString msg);

private:
    QVBoxLayout* verticalLayout;
    QSplitter* splitter;
    WorkflowGraphicsView* graphicsView;
    QTextEdit* textSCXML;
    QTextEdit* textOutput;
};

#endif // WORKFLOWSURFACE_H
