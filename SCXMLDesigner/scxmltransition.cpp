#include <QCursor>
#include <QVector2D>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include "scxmltransition.h"

SCXMLTransition::SCXMLTransition(SCXMLState *parent) :
    QAbstractTransition(), mParentState(parent), mStartPoint(0,0), mEndPoint(0,0), mDescription(""),
    mMovingControlPoint1(false), mNewControlPoint1StartX(0), mNewControlPoint1StartY(0),
    mMovingControlPoint2(false), mNewControlPoint2StartX(0), mNewControlPoint2StartY(0),
    mMovingStartPoint(false)
{
    mControlPoint1 = QPoint(100, 100);
    mControlPoint2 = QPoint(50, 50);

    // only the start, end and control points can be moved - not the full line
    setFlag(QGraphicsItem::ItemIsMovable, false);
    // can select to show property dialogs
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setCursor(Qt::ArrowCursor);
    setAcceptHoverEvents(true);

    UpdatePoints();
}

//TODO: add an onSizeChange dependency to the shape so when resized, sets the start point

void SCXMLTransition::UpdatePoints()
{
    SCXMLState* startState = dynamic_cast<SCXMLState*>(parent());
    SCXMLState* endState = dynamic_cast<SCXMLState*>(targetState());
    if (endState != nullptr) {
        mStartPoint = QPoint(startState->GetShapeX(), startState->GetShapeY());
        mEndPoint = QPoint(endState->GetShapeX(), endState->GetShapeY());
    }
}

void SCXMLTransition::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF mousePos = event->pos();

    if (mMovingControlPoint1) {
        prepareGeometryChange();
        mNewControlPoint1StartX = mousePos.x();
        mNewControlPoint1StartY = mousePos.y();
        mControlPoint1.setX(mNewControlPoint1StartX);
        mControlPoint1.setY(mNewControlPoint1StartY);

        update();
        event->accept();
        return;
    }

    if (mMovingControlPoint2) {
        prepareGeometryChange();

        mNewControlPoint2StartX = mousePos.x();
        mNewControlPoint2StartY = mousePos.y();
        mControlPoint2.setX(mNewControlPoint2StartX);
        mControlPoint2.setY(mNewControlPoint2StartY);

        update();
        event->accept();
        return;
    }

    if (mMovingStartPoint) {
        prepareGeometryChange();

        SetX1(mousePos.x());
        SetY1(mousePos.y());

        update();
        event->accept();
        return;
    }

    //QGraphicsItem::mouseMoveEvent(event);
    update();
}

void SCXMLTransition::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF mousePos = event->pos();

    // if mouse is over a control point, move it
    QPainterPath controlPointPath1;
    controlPointPath1.addEllipse(mControlPoint1, 5, 5);
    if (controlPointPath1.contains(mousePos)) {
        mMovingControlPoint1 = true;
        mNewControlPoint1StartX = mousePos.x();
        mNewControlPoint1StartY = mousePos.y();
        mControlPoint1.setX(mNewControlPoint1StartX);
        mControlPoint1.setY(mNewControlPoint1StartY);
        event->accept();
        update();
        return;
    }

    QPainterPath controlPointPath2;
    controlPointPath2.addEllipse(mControlPoint2, 5, 5);
    if (controlPointPath2.contains(mousePos)) {
        mMovingControlPoint2 = true;
        mNewControlPoint2StartX = mousePos.x();
        mNewControlPoint2StartY = mousePos.y();
        mControlPoint2.setX(mNewControlPoint2StartX);
        mControlPoint2.setY(mNewControlPoint2StartY);
        event->accept();
        return;
    }

    QPainterPath startPointPath;
    startPointPath.addEllipse(mStartPoint, 5, 5);
    if (startPointPath.contains(mousePos)) {
        mMovingStartPoint = true;
        SetX1(mousePos.x());
        SetY1(mousePos.y());
        event->accept();
        return;
    }

    QGraphicsItem::mousePressEvent(event);
    update();
}

void SCXMLTransition::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF mousePos = event->pos();

    if (mMovingControlPoint1) {
        mNewControlPoint1StartX = mousePos.x();
        mNewControlPoint1StartY = mousePos.y();
        mControlPoint1.setX(mNewControlPoint1StartX);
        mControlPoint1.setY(mNewControlPoint1StartY);
        mMovingControlPoint1 = false;
        QGraphicsItem::mouseReleaseEvent(event);
        return;
    }

    if (mMovingControlPoint2) {
        mNewControlPoint2StartX = mousePos.x();
        mNewControlPoint2StartY = mousePos.y();
        mControlPoint2.setX(mNewControlPoint2StartX);
        mControlPoint2.setY(mNewControlPoint2StartY);
        mMovingControlPoint2 = false;
        QGraphicsItem::mouseReleaseEvent(event);
        return;
    }

    //TODO:mMovingStartPoint

    update();
}

void SCXMLTransition::ApplyMetaData(QMap<QString, QString> *mapMetaData)
{
    foreach(QString key, mapMetaData->keys()) {
        QString value = mapMetaData->value(key);
        if (key == "description") {
            SetDescription(value);
            continue;
        }
        if (key == "x1") {
            SetX1(value.toDouble());
            continue;
        }
        if (key == "y1") {
            SetY1(value.toDouble());
            continue;
        }
        if (key == "x2") {
            SetX2(value.toDouble());
            continue;
        }
        if (key == "y2") {
            SetY2(value.toDouble());
            continue;
        }
        if (key == "cx1") {
            SetControlPointX1(value.toDouble());
            continue;
        }
        if (key == "cy1") {
            SetControlPointY1(value.toDouble());
            continue;
        }
        if (key == "cx2") {
            SetControlPointX2(value.toDouble());
            continue;
        }
        if (key == "cy2") {
            SetControlPointY2(value.toDouble());
            continue;
        }
    }
}

QString SCXMLTransition::GetMetaDataString()
{
    QString metadata = QString(" META-DATA [x1=%1] [y1=%2] [x2=%3] [y2=%4] [cx1=%5] [cy1=%6] [cx2=%7] [cy2=%8] [description=%9]")
            .arg(GetX1())
            .arg(GetY1())
            .arg(GetX2())
            .arg(GetY2())
            .arg(GetControlPointX1())
            .arg(GetControlPointY1())
            .arg(GetControlPointX2())
            .arg(GetControlPointY2())
            .arg(GetDescription());
    return metadata;
}

//!
//! \brief SCXMLTransition::CalculatePaths
//! Calculates the paths of the Bezier and arrow head
//! \param bezierPath
//! \param arrowHeadPath
//! \return
//!
bool SCXMLTransition::CalculatePaths(QPainterPath *bezierPath, QPainterPath *arrowHeadPath,
                                     QPainterPath *controlLine1Path, QPainterPath *controlLine2Path,
                                     QPainterPath *startPointPath, QPainterPath *endPointPath) const
{
    //SCXMLState* startState = dynamic_cast<SCXMLState*>(parent());
    SCXMLState* endState = dynamic_cast<SCXMLState*>(targetState());
    if (endState == nullptr) return false;

//    QPoint pointStart = QPoint(startState->GetShapeX(), startState->GetShapeY());
//    QPoint pointEnd = QPoint(endState->GetShapeX(), endState->GetShapeY());
    //QLineF lineToDraw = QLine(pointStart, pointEnd);

    //TODO: remove pointStart local?
//    mStartPoint = pointStart;
//    mEndPoint = pointEnd;
    QLineF lineToDraw = QLine(mStartPoint, mEndPoint);

    // draw the Bezier curve
    QPainterPath tempBezierPath;
    tempBezierPath.moveTo(mStartPoint);
    tempBezierPath.cubicTo(mControlPoint1, mControlPoint2, mEndPoint);

    QPainterPathStroker stroker;
    stroker.setWidth(1);
    stroker.setCurveThreshold(0.1);
    *bezierPath = stroker.createStroke(tempBezierPath);

    // calculate the arrowhead points
    QVector2D vectorLength(lineToDraw.dx(), lineToDraw.dy());
    vectorLength.normalize();
    QVector2D vectorWidth(-vectorLength.y(), vectorLength.x());
    // scale the vectors to resize the arrow head
    vectorLength *= 20;
    vectorWidth *= 4;
    // draw the arrow head using the vectors for identifying the points
    QPointF arrowP1 = lineToDraw.p1() + vectorLength.toPointF() + vectorWidth.toPointF();
    QPointF arrowP2 = lineToDraw.p1() + vectorLength.toPointF() - vectorWidth.toPointF();
    QPolygonF poly;
    poly.append(lineToDraw.p1());
    poly.append(arrowP1);
    poly.append(arrowP2);
    arrowHeadPath->addPolygon(poly);

    controlLine1Path->addEllipse(mStartPoint, 5, 5);
    controlLine1Path->addEllipse(mControlPoint1, 5, 5);
    QPolygonF controlLine1;
    controlLine1.append(mStartPoint);
    controlLine1.append(mControlPoint1);
    controlLine1Path->addPolygon(controlLine1);

    controlLine2Path->addEllipse(mEndPoint, 5, 5);
    controlLine2Path->addEllipse(mControlPoint2, 5, 5);
    QPolygonF controlLine2;
    controlLine2.append(mEndPoint);
    controlLine2.append(mControlPoint2);
    controlLine2Path->addPolygon(controlLine2);

    startPointPath->addEllipse(mStartPoint, 10, 10);

    endPointPath->addEllipse(mEndPoint, 5, 5);

    return true;
}

QPainterPath SCXMLTransition::shape() const
{
    QPainterPath bezierPath, arrowHeadPath, controlLine1Path, controlLine2Path, startPointPath, endPointPath, combinedPath;
    CalculatePaths(&bezierPath, &arrowHeadPath, &controlLine1Path, &controlLine2Path, &startPointPath, &endPointPath);
    combinedPath.addPath(bezierPath);
    combinedPath.addPath(arrowHeadPath);
    combinedPath.addPath(controlLine1Path);
    combinedPath.addPath(controlLine2Path);
    combinedPath.addPath(startPointPath);
    combinedPath.addPath(endPointPath);

    return combinedPath;
}

QRectF SCXMLTransition::boundingRect() const
{
    return shape().boundingRect();
}

void SCXMLTransition::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    //SCXMLState* startState = dynamic_cast<SCXMLState*>(parent());
    SCXMLState* endState = dynamic_cast<SCXMLState*>(targetState());
    if (endState == nullptr) return;

    QBrush blackBrush = QBrush(isSelected() ? Qt::blue : Qt::black);
    QBrush controlLineBrush = QBrush(isSelected() ? Qt::blue : Qt::darkYellow);
    QPen controlPointPen = QPen(controlLineBrush, 3, Qt::SolidLine);
    QBrush endPointBrush = QBrush(isSelected() ? Qt::darkYellow : Qt::yellow);
    QPen endPointPen = QPen(endPointBrush, 3, Qt::SolidLine);
    QPen transitionPen = QPen(blackBrush, 3, Qt::SolidLine);

    painter->setBrush(blackBrush);
    painter->setPen(transitionPen);
    painter->setRenderHint(QPainter::HighQualityAntialiasing, true);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);

    //TODO: only calculate when changes made - not on every paint
    QPainterPath bezierPath, arrowHeadPath, controlLine1Path, controlLine2Path, startPointPath, endPointPath;
    CalculatePaths(&bezierPath, &arrowHeadPath, &controlLine1Path, &controlLine2Path, &startPointPath, &endPointPath);
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(bezierPath);
    painter->setBrush(blackBrush);
    painter->drawPath(arrowHeadPath);

    // only show anchor points if selected
    if (isSelected()) {
        // anchor points
        painter->setPen(controlPointPen);
        painter->drawPath(controlLine1Path);
        painter->drawPath(controlLine2Path);
        painter->setPen(endPointPen);
        painter->drawPath(startPointPath);
        painter->drawPath(endPointPath);
    }
}

void SCXMLTransition::Update()
{
    prepareGeometryChange();
    update();
}
