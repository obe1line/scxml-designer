#include "workflowtab.h"

WorkflowTab::WorkflowTab(QWidget *parent, QString filename) :
    QWidget(parent), mFilename(filename)
{
    mTabWidget = dynamic_cast<QTabWidget*>(parent);
}

void WorkflowTab::SetFilename(QString filename)
{
    mFilename = filename;

    // set the tooltip to show the filename
    int tabIndex = mTabWidget->indexOf(this);
    mTabWidget->setTabToolTip(tabIndex, filename);
}
