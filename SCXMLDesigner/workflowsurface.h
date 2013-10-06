#ifndef WORKFLOWSURFACE_H
#define WORKFLOWSURFACE_H

#include <QWidget>
#include <QGraphicsView>

namespace Ui {
class WorkflowSurface;
}

class WorkflowSurface : public QWidget
{
    Q_OBJECT
    
public:
    explicit WorkflowSurface(QWidget *parent = 0);
    ~WorkflowSurface();

    void SetSCMLText(QString scxml);
    QGraphicsView *GetSurface();
private:
    Ui::WorkflowSurface *ui;
};

#endif // WORKFLOWSURFACE_H
