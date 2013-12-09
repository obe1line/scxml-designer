#include <QCursor>
#include <QVector2D>
#include "scxmltransition.h"

SCXMLTransition::SCXMLTransition(SCXMLState *parent) :
    QAbstractTransition(), mParentState(parent), mX1(0), mX2(0), mY1(0), mY2(0), mDescription("")
{
    mPointControl1 = QPoint(100, 100);
    mPointControl2 = QPoint(50, 50);

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setCursor(Qt::OpenHandCursor);
    setAcceptHoverEvents(true);
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
bool SCXMLTransition::CalculatePaths(QPainterPath *bezierPath, QPainterPath *arrowHeadPath) const
{
    SCXMLState* startState = dynamic_cast<SCXMLState*>(parent());
    SCXMLState* endState = dynamic_cast<SCXMLState*>(targetState());
    if (endState == nullptr) return false;

    QPoint pointStart = QPoint(startState->GetShapeX(), startState->GetShapeY());
    QPoint pointEnd = QPoint(endState->GetShapeX(), endState->GetShapeY());
    QLineF lineToDraw = QLine(pointStart, pointEnd);

    //TODO: make this a moveable control point

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

    return true;
}

QRectF SCXMLTransition::boundingRect() const
{
    QPainterPath bezierPath, arrowHeadPath, combinedPath;
    CalculatePaths(&bezierPath, &arrowHeadPath);
    combinedPath.addPath(bezierPath);
    combinedPath.addPath(arrowHeadPath);

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
    QBrush yellowBrush = QBrush(isSelected() ? Qt::blue : Qt::yellow);
    QPen controlPointPen = QPen(yellowBrush, 3, Qt::SolidLine);
    QPen transitionPen = QPen(blackBrush, 3, Qt::SolidLine);

    painter->setBrush(blackBrush);
    painter->setPen(transitionPen);
    painter->setRenderHint(QPainter::HighQualityAntialiasing, true);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);

    QPainterPath bezierPath, arrowHeadPath;
    CalculatePaths(&bezierPath, &arrowHeadPath);
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(bezierPath);
    painter->setBrush(blackBrush);
    painter->drawPath(arrowHeadPath);

    // anchor points
    QPoint pointStart = QPoint(startState->GetShapeX(), startState->GetShapeY());
    QPoint pointEnd = QPoint(endState->GetShapeX(), endState->GetShapeY());
    painter->setPen(controlPointPen);

    painter->drawEllipse(pointStart, 5, 5);
    painter->drawEllipse(mPointControl1, 5, 5);
    painter->drawLine(pointStart, mPointControl1);

    painter->drawEllipse(pointEnd, 5, 5);
    painter->drawEllipse(mPointControl2, 5, 5);
    painter->drawLine(pointEnd, mPointControl2);
    //TODO: anchor points
}

void SCXMLTransition::Update()
{
    prepareGeometryChange();
    update();
}
