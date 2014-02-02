#include "chaikincurve.h"
#include <QPainter>
#include <QtOpenGL/QGLFunctions>
#include <QDebug>
#include <QGraphicsSceneHoverEvent>
#include <QDrag>
#include <QMimeData>
#include <QGraphicsWidget>

void ChaikinCurve::InitializeCurvePoints()
{
    mCurvePoints.clear();
    foreach (QVector3D point, mOriginalCurvePoints) {
        mCurvePoints.append(point);
    }
}

ChaikinCurve::ChaikinCurve()
{
    // create brushes and pens
    mYellowBrush = new QBrush(Qt::GlobalColor::yellow, Qt::SolidPattern);
    mControlPointPen = new QPen(Qt::GlobalColor::black);
    mControlPointPen->setWidth(2);

    // create the initial curve points
    mOriginalCurvePoints.push_back( QVector3D(10,10,0));
    mOriginalCurvePoints.push_back( QVector3D(100,50,0));
    mOriginalCurvePoints.push_back( QVector3D(50,70,0));
    mOriginalCurvePoints.push_back( QVector3D(20,160,0));
    mOriginalCurvePoints.push_back( QVector3D(200,200,0));

    // create the starting points
    InitializeCurvePoints();

    this->setBoundingRegionGranularity(1);
    this->setAcceptDrops(true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setCursor(Qt::PointingHandCursor);

    mControlPointVisible = true;    //TODO: change to false after testing
}

void ChaikinCurve::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)

    painter->begin(widget);

    // draw the lines
    QPainterPath path = GetPathOfLines();
    painter->setPen(*mControlPointPen);
    painter->drawPath(path);

    // draw the moveable points
    if (mControlPointVisible) {
        painter->setPen(*mControlPointPen);
        painter->setBrush(*mYellowBrush);
        path = GetPathOfControlPoints();
        painter->drawPath(path);
    }

    painter->end();
}

QRectF ChaikinCurve::boundingRect() const
{
    QPainterPath path = GetPathOfLines();
    path.addPath(GetPathOfControlPoints());
    return path.boundingRect();
}

QPainterPath ChaikinCurve::GetPathOfLines() const
{
    QPainterPath path;
    QVector<QVector3D>::const_iterator it = mCurvePoints.constBegin();
    QVector3D lastPoint = *it;
    bool first = true;
    for(; it != mCurvePoints.constEnd(); ++it) {
        if (!first) {
            first = false;
            continue;
        }

        path.moveTo(QPoint(lastPoint.toPoint()));
        path.lineTo(QPoint(it->toPoint()));
        path.closeSubpath();

        lastPoint = *it;
    }

    return path;
}

QPainterPath ChaikinCurve::GetPathOfControlPoints() const
{
    QPainterPath path;
    foreach (QVector3D point, mOriginalCurvePoints) {
        path.addEllipse(point.toPoint(), 5, 5);
    }

    return path;
}

void ChaikinCurve::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    Q_UNUSED(event);
    //event->acceptProposedAction();
}

void ChaikinCurve::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    Q_UNUSED(event);
    //event->acceptProposedAction();
}

void ChaikinCurve::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    Q_UNUSED(event);
    // allow drop events outside of the bounding rect
    //event->accept();
}

void ChaikinCurve::SetNewPointPosition(QPointF dragStartPoint, QPointF dragDropPoint)
{
    Q_UNUSED(dragStartPoint);
    //TODO: find control point - for now just use first point
    this->prepareGeometryChange();
    mOriginalCurvePoints[0].setX(dragDropPoint.x());
    mOriginalCurvePoints[0].setY(dragDropPoint.y());
    InitializeCurvePoints();
    update();
}

void ChaikinCurve::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    Q_UNUSED(event);
    SetNewPointPosition(mControlDragStartPoint, event->pos());
}

QPainterPath ChaikinCurve::shape() const
{
    QPainterPath path;

    // add the lines
    path = GetPathOfLines();

    // add the control points
    path.addPath(GetPathOfControlPoints());
    return path;
}

// When we increase the LOD we will have to re-create the points
// array inserting the new intermediate points into it.
//
//	Basically the subdivision works like this. each line,
//
//            A  *------------*  B
//
//	will be split into 2 new points, Q and R.
//
//                   Q    R
//            A  *---|----|---*  B
//
//	Q and R are given by the equations :
//
// 		Q = 3/4*A + 1/4*B
// 		R = 3/4*B + 1/4*A
//
void ChaikinCurve::IncreaseLod()
{
    QVector<QVector3D> newPoints;

    // keep the first point
    newPoints.push_back(mCurvePoints[0]);
    for(int i=0; i<(mCurvePoints.size()-1); ++i) {

        // get 2 original points
        const QVector3D& p0 = mCurvePoints[i];
        const QVector3D& p1 = mCurvePoints[i+1];
        QVector3D Q;
        QVector3D R;

        // average the 2 original points to create 2 new points. For each
        // CV, another 2 verts are created.
        Q.setX(0.75f*p0.x() + 0.25f*p1.x());
        Q.setY(0.75f*p0.y() + 0.25f*p1.y());
        Q.setZ(0.75f*p0.z() + 0.25f*p1.z());

        R.setX(0.25f*p0.x() + 0.75f*p1.x());
        R.setY(0.25f*p0.y() + 0.75f*p1.y());
        R.setZ(0.25f*p0.z() + 0.75f*p1.z());

        newPoints.push_back(Q);
        newPoints.push_back(R);
    }
    // keep the last point
    newPoints.push_back(mCurvePoints[mCurvePoints.size()-1]);

    // update the points array
    mCurvePoints = newPoints;
}

//------------------------------------------------------------
// When we decrease the LOD, we can rather niftily get back
// to exactly what we had before. Since the original subdivision
// simply required a basic ratio of both points, we can simply
// reverse the ratio's to get the previous point...
//
void ChaikinCurve::DecreaseLod()
{
    // need at lease two points for a line
    if (mCurvePoints.size() <= 2)
        return;

    QVector<QVector3D> newPoints;

    // keep the first point
    newPoints.push_back(mCurvePoints[0]);

    // step over every 2 points
    for(int i=1; i<(mCurvePoints.size()-1); i+=2) {

        // get last point found in reduced array
        const QVector3D& pLast = newPoints[newPoints.size()-1];

        // get 2 original point
        const QVector3D& p0 = mCurvePoints[i];
        QVector3D Q;

        // calculate the original point
        Q.setX(4.0f * (p0.x() - 0.75f*pLast.x()));
        Q.setY(4.0f * (p0.y() - 0.75f*pLast.y()));
        Q.setZ(4.0f * (p0.z() - 0.75f*pLast.z()));

        // add to new curve
        newPoints.push_back(Q);
    }

    // keep the last point
    newPoints.push_back(mCurvePoints[mCurvePoints.size()-1]);

    // copy over points
    mCurvePoints = newPoints;
}

void ChaikinCurve::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    QPainterPath path = GetPathOfControlPoints();
    if (path.contains(event->pos())) {
        // start control point drag
        mControlDragStartPoint = event->pos();
        QDrag *drag = new QDrag(event->widget());
        QMimeData *data = new QMimeData();
        drag->setMimeData(data);
        drag->start(Qt::MoveAction);
    }
    else {
        mControlPointVisible = !mControlPointVisible;
        update();
        event->accept();
    }
}

void ChaikinCurve::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    QGraphicsItem::mouseMoveEvent(event);
}

void ChaikinCurve::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    QGraphicsItem::mouseReleaseEvent(event);
}
