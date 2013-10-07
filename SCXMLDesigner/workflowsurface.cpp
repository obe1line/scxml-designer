#include "workflowsurface.h"
#include "ui_workflowsurface.h"

WorkflowSurface::WorkflowSurface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WorkflowSurface)
{
    ui->setupUi(this);
    QList<int> sizes;
    sizes.append(300);
    sizes.append(100);
    ui->splitter->setSizes(sizes);
}

WorkflowSurface::~WorkflowSurface()
{
    delete ui;
}

void WorkflowSurface::SetSCMLText(QString scxml)
{
    ui->textSCXML->setText(scxml);
}

QGraphicsView *WorkflowSurface::GetSurface()
{
    return ui->graphicsView;
}
