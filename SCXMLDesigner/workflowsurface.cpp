#include "workflowsurface.h"
#include "ui_workflowsurface.h"

WorkflowSurface::WorkflowSurface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WorkflowSurface)
{
    ui->setupUi(this);
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
