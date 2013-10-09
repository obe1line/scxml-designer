#ifndef WORKFLOWGRAPHICSVIEW_H
#define WORKFLOWGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QSplitter>

class WorkflowGraphicsView : public QGraphicsView
{
public:
    //WorkflowGraphicsView();
    WorkflowGraphicsView(QSplitter*&);

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // WORKFLOWGRAPHICSVIEW_H
