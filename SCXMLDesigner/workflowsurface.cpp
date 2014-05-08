#include "workflowsurface.h"

WorkflowSurface::WorkflowSurface(QWidget *parent) :
    QWidget(parent)
{
    CreateWidgets();

    QList<int> sizes;
    sizes.append(300);
    sizes.append(70);
    sizes.append(30);
    splitter->setSizes(sizes);
}

void WorkflowSurface::CreateWidgets()
{
    this->resize(400, 339);
    verticalLayout = new QVBoxLayout(this);
    splitter = new QSplitter(this);
    splitter->setFrameShape(QFrame::NoFrame);
    splitter->setLineWidth(1);
    splitter->setMidLineWidth(2);
    splitter->setOrientation(Qt::Vertical);
    graphicsView = new WorkflowGraphicsView(splitter);
    splitter->addWidget(graphicsView);
    textSCXML = new QTextEdit(splitter);
    textSCXML->setMinimumSize(QSize(0, 0));
    textSCXML->setBaseSize(QSize(0, 0));
    splitter->addWidget(textSCXML);
    textOutput = new QTextEdit(splitter);
    textOutput->setMinimumSize(QSize(0, 0));
    textOutput->setBaseSize(QSize(0, 0));
    splitter->addWidget(textOutput);

    verticalLayout->addWidget(splitter);
}

WorkflowSurface::~WorkflowSurface()
{
}

void WorkflowSurface::SetSCMLText(QString scxml)
{
    this->textSCXML->setText(scxml);
}

void WorkflowSurface::AppendToTextOutput(QString txt)
{
    this->textOutput->append(txt);
}

QGraphicsView *WorkflowSurface::GetSurface()
{
    return this->graphicsView;
}

void WorkflowSurface::WriteToOutput(QString msg)
{
    this->AppendToTextOutput(msg);
}
