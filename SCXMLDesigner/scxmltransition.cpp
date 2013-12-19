#include <QCursor>
#include <QVector2D>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include "scxmltransition.h"

SCXMLTransition::SCXMLTransition(SCXMLState *parent) :
    QAbstractTransition(), mParentState(parent), mX1(0), mX2(0), mY1(0), mY2(0), mDescription(""),
    mMovingControlPoint1(false), mNewControlPoint1StartX(0), mNewControlPoint1StartY(0),
    mMovingControlPoint2(false), mNewControlPoint2StartX(0), mNewControlPoint2StartY(0)
{
    mPointControl1 = QPoint(100, 100);
    mPointControl2 = QPoint(50, 50);

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setCursor(Qt::OpenHandCursor);
    setAcceptHoverEvents(true);
}

void SCXMLTransition::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (mMovingControlPoint1) {
        prepareGeometryChange();
        mNewControlPoint1StartX = event->pos().x();
        mNewControlPoint1StartY = event->pos().y();
        mPointControl1.setX(mNewControlPoint1StartX);
        mPointControl1.setY(mNewControlPoint1StartY);

        update();
        event->accept();
        return;
    }

//    if (mMovingControlPoint2) {
//        prepareGeometryChange();

//        mNewControlPoint2StartX = event->pos().x();
//        mNewControlPoint2StartY = event->pos().y();
//        mPointControl2.setX(mNewControlPoint2StartX);
//        mPointControl2.setY(mNewControlPoint2StartY);

//        update();
//        event->accept();
//        return;
//    }

    //QGraphicsItem::mouseMoveEvent(event);
    //UpdateTransitions();
}

void SCXMLTransition::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // if mouse is over control point, move it
    QRect controlPoint1Rect = QRect(mPointControl1.x()-3, mPointControl1.y()-3, 12, 12);
    qreal x = event->pos().x();
    qreal y = event->pos().y();
    qDebug() << "x,y: " << x << " , " << y;
    qDebug() << "mPointControl1 x,y: " << mPointControl1.x() << " , " << mPointControl1.y();
    qDebug() << "contains? " << controlPoint1Rect.contains(x, y);
    if (controlPoint1Rect.contains(x, y)) {
    //if (true) {
        mMovingControlPoint1 = true;
        mNewControlPoint1StartX = x;
        mNewControlPoint1StartY = y;
        mPointControl1.setX(mNewControlPoint1StartX);
        mPointControl1.setY(mNewControlPoint1StartY);
        event->accept();
        update();
        return;
    }

//    QRect controlPoint2Rect = QRect(mPointControl2.x()-3, mPointControl2.y()-3, 6, 6);
//    if (controlPoint2Rect.contains(event->pos().x(), event->pos().y())) {
//        mMovingControlPoint2 = true;
//        mNewControlPoint2StartX = event->pos().x();
//        mNewControlPoint2StartY = event->pos().y();
//        mPointControl2.setX(mNewControlPoint2StartX);
//        mPointControl2.setY(mNewControlPoint2StartY);
//        event->accept();
//        return;
//    }

    QGraphicsItem::mousePressEvent(event);
    update();
}

void SCXMLTransition::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (mMovingControlPoint1) {
        mNewControlPoint1StartX = event->pos().x();
        mNewControlPoint1StartY = event->pos().y();
        mPointControl1.setX(mNewControlPoint1StartX);
        mPointControl1.setY(mNewControlPoint1StartY);
        mMovingControlPoint1 = false;
        QGraphicsItem::mouseReleaseEvent(event);
        return;
    }

    if (mMovingControlPoint2) {
        mNewControlPoint2StartX = event->pos().x();
        mNewControlPoint2StartY = event->pos().y();
        mPointControl2.setX(mNewControlPoint2StartX);
        mPointControl2.setY(mNewControlPoint2StartY);
        mMovingControlPoint2 = false;
        QGraphicsItem::mouseReleaseEvent(event);
        return;
    }

    //UpdateTransitions();
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
    }
}

QString SCXMLTransition::GetMetaDataString()
{
    QString metadata = QString(" META-DATA [x1=%1] [y1=%2] [x2=%3] [y2=%4] [description=%5]").arg(
                GetX1()).arg(GetY1()).arg(GetX2()).arg(GetY2()).arg(GetDescription());
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
                                     QPainterPath * controlLine1Path, QPainterPath *controlLine2Path) const
{
    SCXMLState* startState = dynamic_cast<SCXMLState*>(parent());
    SCXMLState* endState = dynamic_cast<SCXMLState*>(targetState());
    if (endState == nullptr) return false;

    QPoint pointStart = QPoint(startState->GetShapeX(), startState->GetShapeY());
    QPoint pointEnd = QPoint(endState->GetShapeX(), endState->GetShapeY());
    QLineF lineToDraw = QLine(pointStart, pointEnd);


    // draw the Bezier curve
    bezierPath->moveTo(pointStart);
    bezierPath->cubicTo(mPointControl1, mPointControl2, pointEnd);

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

    controlLine1Path->addEllipse(pointStart, 5, 5);
    controlLine1Path->addEllipse(mPointControl1, 5, 5);
    QPolygonF controlLine1;
    controlLine1.append(pointStart);
    controlLine1.append(mPointControl1);
    controlLine1Path->addPolygon(controlLine1);
    //controlLine2Path;

    return true;
}

QRectF SCXMLTransition::boundingRect() const
{
    QPainterPath bezierPath, arrowHeadPath, controlLine1, controlLine2, combinedPath;
    CalculatePaths(&bezierPath, &arrowHeadPath, &controlLine1, &controlLine2);
    combinedPath.addPath(bezierPath);
    combinedPath.addPath(arrowHeadPath);
    combinedPath.addPath(controlLine1);
    //combinedPath.addPath(controlLine2);

    return combinedPath.boundingRect();
}

void SCXMLTransition::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    SCXMLState* startState = dynamic_cast<SCXMLState*>(parent());
    SCXMLState* endState = dynamic_cast<SCXMLState*>(targetState());
    if (endState == nullptr) return;

    QBrush blackBrush = QBrush(isSelected() ? Qt::blue : Qt::black);
    QBrush controlLineBrush = QBrush(isSelected() ? Qt::blue : Qt::darkYellow);
    QPen controlPointPen = QPen(controlLineBrush, 3, Qt::SolidLine);
    QPen transitionPen = QPen(blackBrush, 3, Qt::SolidLine);

    painter->setBrush(blackBrush);
    painter->setPen(transitionPen);
    painter->setRenderHint(QPainter::HighQualityAntialiasing, true);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);

    QPainterPath bezierPath, arrowHeadPath, controlLine1Path, controlLine2Path;
    CalculatePaths(&bezierPath, &arrowHeadPath, &controlLine1Path, &controlLine2Path);
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(bezierPath);
    painter->setBrush(blackBrush);
    painter->drawPath(arrowHeadPath);

    // only show anchor points if selected
    if (isSelected()) {
        // anchor points
        //QPoint pointStart = QPoint(startState->GetShapeX(), startState->GetShapeY());
        //QPoint pointEnd = QPoint(endState->GetShapeX(), endState->GetShapeY());
        painter->setPen(controlPointPen);

        //painter->drawEllipse(pointStart, 5, 5);
        //painter->drawEllipse(mPointControl1, 5, 5);
        //painter->drawLine(pointStart, mPointControl1);
        painter->drawPath(controlLine1Path);
        //painter->drawEllipse(pointEnd, 5, 5);
        //painter->drawEllipse(mPointControl2, 5, 5);
        //painter->drawLine(pointEnd, mPointControl2);
    }
}

void SCXMLTransition::Update()
{
    prepareGeometryChange();
    update();
}
