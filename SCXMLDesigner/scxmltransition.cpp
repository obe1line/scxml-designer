#include <QCursor>
#include <QVector2D>
#include "scxmltransition.h"

SCXMLTransition::SCXMLTransition(SCXMLState *parent) :
    QAbstractTransition(), mParentState(parent), mX1(0), mX2(0), mY1(0), mY2(0), mDescription("")
{
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

QRectF SCXMLTransition::boundingRect() const
{
    //TODO: refactor duplicate code
    SCXMLState* startState = dynamic_cast<SCXMLState*>(parent());
    SCXMLState* endState = dynamic_cast<SCXMLState*>(targetState());
    if (endState == nullptr) return QRectF();

    QPoint pointStart = QPoint(startState->GetShapeX(), startState->GetShapeY());
    QPoint pointEnd = QPoint(endState->GetShapeX(), endState->GetShapeY());
    QLineF lineToDraw = QLine(pointStart, pointEnd);

    //TODO: make this a moveable control point
    QPoint pointControl = QPoint(50, 50);

    // draw the Bezier curve
    QPainterPath path;
    path.moveTo(pointStart);
    path.quadTo(pointControl,pointEnd);

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
    path.addPolygon(poly);

    return path.boundingRect();
}

void SCXMLTransition::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    QBrush blackBrush = QBrush(isSelected() ? Qt::blue : Qt::black);
    QPen transitionPen = QPen(blackBrush, 2, Qt::SolidLine);
    //QPen arrowPen = QPen(blackBrush, 2, Qt::SolidLine);
    painter->setPen(transitionPen);

    // determine the transition start and end points
    //TODO: anchor points
    SCXMLState* startState = dynamic_cast<SCXMLState*>(parent());
    SCXMLState* endState = dynamic_cast<SCXMLState*>(targetState());
    if (endState == nullptr) return;

    QPoint pointStart = QPoint(startState->GetShapeX(), startState->GetShapeY());
    QPoint pointEnd = QPoint(endState->GetShapeX(), endState->GetShapeY());
    QLineF lineToDraw = QLine(pointStart, pointEnd);

    //TODO: make this a moveable control point
    QPoint pointControl = QPoint(50, 50);

    SCXMLState* midPoint = new SCXMLState("test");
    midPoint->setX(50);
    midPoint->setY(50);

    // draw the Bezier curve
    QPainterPath path;
    path.moveTo(pointStart);
    path.quadTo(pointControl,pointEnd);
    painter->drawPath(path);

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
    QPointF polyPoints[3] = { lineToDraw.p1(), arrowP1, arrowP2 };
    painter->setBrush(blackBrush);
    painter->drawPolygon(polyPoints, 3);
}

void SCXMLTransition::Update()
{
    prepareGeometryChange();
    update();
}
