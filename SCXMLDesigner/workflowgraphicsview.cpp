#include <QMouseEvent>
#include "workflowgraphicsview.h"

//WorkflowGraphicsView::WorkflowGraphicsView()
//{
//}

WorkflowGraphicsView::WorkflowGraphicsView(QSplitter *&)
{
    setDragMode(QGraphicsView::RubberBandDrag);
}

void WorkflowGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (event->modifiers() & Qt::ShiftModifier )
        event->setModifiers(Qt::ControlModifier);

    QGraphicsView::mousePressEvent(event);
}

void WorkflowGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->modifiers() & Qt::ShiftModifier )
        event->setModifiers(Qt::ControlModifier);

    QGraphicsView::mouseReleaseEvent(event);
}
