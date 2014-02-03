#include <QCursor>
#include <QVector2D>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include "scxmltransition.h"

SCXMLTransition::SCXMLTransition(SCXMLState *parent) :
    QAbstractTransition(), ChaikinCurve(4, QVector<QVector3D>()), mParentState(parent),
    mDescription(""), mEvent("")
{
    // only the mid-control points can be moved - not the curve
    setFlag(QGraphicsItem::ItemIsMovable, false);
    // can select to show property dialogs
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setCursor(Qt::ArrowCursor);
    setAcceptHoverEvents(true);

    // for the curve animation
    SetParentObject(this);

    UpdatePoints();
}

//!
//! \brief SCXMLTransition::GetControlPoints
//! Converts the control points of the curve into a string to be used for export
//! \return String representation of the control points
//!
QString SCXMLTransition::GetControlPoints()
{
    QString resultString = "";
    QVector<QVector3D> points = GetCurveControlPoints();
    foreach (QVector3D point, points) {
        QString curvePosString = QString("%1,%2").arg(point.x()).arg(point.y());
        if (resultString.length() > 0) {
            resultString.append(":");
        }
        resultString.append(curvePosString);
    }
    return resultString;
}

//!
//! \brief SCXMLTransition::GetControlPoints
//! \param value String containing the point coordinates E.g. "1,2:5,6"
//! \return A vector of points
//!
QVector<QVector3D> SCXMLTransition::GetControlPoints(QString value)
{
    QVector<QVector3D> points;
    QStringList pointStrings = value.split(":");
    foreach (QString pointPair, pointStrings) {
        QStringList coord = pointPair.split(",");
        points.append(QVector3D(coord[0].toInt(), coord[1].toInt(), 0));
    }
    return points;
}

void SCXMLTransition::SetControlPoints(QString value)
{
    QVector<QVector3D> points = GetControlPoints(value);
    SetStartingPoints(points);
}

void SCXMLTransition::UpdatePoints()
{
    SCXMLState* startState = dynamic_cast<SCXMLState*>(mParentState);
    SCXMLState* endState = dynamic_cast<SCXMLState*>(targetState());
    if (endState != nullptr) {
        QPoint startPoint = QPoint(startState->GetShapeX(), startState->GetShapeY());
        QPoint endPoint = QPoint(endState->GetShapeX(), endState->GetShapeY());
        QVector<QVector3D> curvePoints = GetCurveControlPoints();
        if (curvePoints.length() < 2) {
            // No points set - use the start and end points with two points between
            curvePoints.append(QVector3D(startPoint));
            QPainterPath path;
            path.moveTo(startPoint.x(), startPoint.y());
            path.lineTo(endPoint.x(), endPoint.y());
            curvePoints.append(QVector3D(path.pointAtPercent(0.33)));
            curvePoints.append(QVector3D(path.pointAtPercent(0.66)));
            curvePoints.append(QVector3D(endPoint));
        }
        else {
            // Update start and end points
            curvePoints[0] = QVector3D(startPoint);
            curvePoints[curvePoints.length()-1] = QVector3D(endPoint);
        }

        // Set the outlines to restrict where the start and end curve control points
        // can be placed. This is so the transitions always connect states
        SetStartNodePath(startState->GetNodeOutlinePath());
        SetEndNodePath(endState->GetNodeOutlinePath());
        SetStartingPoints(curvePoints);
    }
}

void SCXMLTransition::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    ChaikinCurve::mouseMoveEvent(event);
}

void SCXMLTransition::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    ChaikinCurve::mousePressEvent(event);;
}

void SCXMLTransition::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    ChaikinCurve::mouseReleaseEvent(event);
}

void SCXMLTransition::ApplyMetaData(QMap<QString, QString> *mapMetaData)
{
    foreach(QString key, mapMetaData->keys()) {
        QString value = mapMetaData->value(key);
        if (key == "description") {
            SetDescription(value);
            continue;
        }
        if (key == "cp") {
            SetControlPoints(value);
            continue;
        }
    }
}

QString SCXMLTransition::GetMetaDataString()
{
    QString metadata = QString(" META-DATA [cp=%1] [description=%2]")
            .arg(GetControlPoints())
            .arg(GetDescription());
    return metadata;
}

////!
////! \brief SCXMLTransition::CalculatePaths
////! Calculates the paths of the Bezier and arrow head
////! \param bezierPath
////! \param arrowHeadPath
////! \return
////!
//bool SCXMLTransition::CalculatePaths(QPainterPath *bezierPath, QPainterPath *arrowHeadPath,
//                                     QPainterPath *controlLine1Path, QPainterPath *controlLine2Path,
//                                     QPainterPath *startPointPath, QPainterPath *endPointPath) const
//{
//    SCXMLState* endState = dynamic_cast<SCXMLState*>(targetState());
//    if (endState == nullptr) return false;

//    QLineF lineToDraw = QLine(mStartPoint, mEndPoint);

//    // draw the Bezier curve
//    QPainterPath tempBezierPath;
//    tempBezierPath.moveTo(mStartPoint);
//    tempBezierPath.cubicTo(mControlPoint1, mControlPoint2, mEndPoint);

//    QPainterPathStroker stroker;
//    stroker.setWidth(1);
//    stroker.setCurveThreshold(0.1);
//    *bezierPath = stroker.createStroke(tempBezierPath);

//    // calculate the arrowhead points
//    QVector2D vectorLength(lineToDraw.dx(), lineToDraw.dy());
//    vectorLength.normalize();
//    QVector2D vectorWidth(-vectorLength.y(), vectorLength.x());
//    // scale the vectors to resize the arrow head
//    vectorLength *= 20;
//    vectorWidth *= 4;
//    // draw the arrow head using the vectors for identifying the points
//    QPointF arrowP1 = lineToDraw.p1() + vectorLength.toPointF() + vectorWidth.toPointF();
//    QPointF arrowP2 = lineToDraw.p1() + vectorLength.toPointF() - vectorWidth.toPointF();
//    QPolygonF poly;
//    poly.append(lineToDraw.p1());
//    poly.append(arrowP1);
//    poly.append(arrowP2);
//    arrowHeadPath->addPolygon(poly);

//    controlLine1Path->addEllipse(mStartPoint, 5, 5);
//    controlLine1Path->addEllipse(mControlPoint1, 5, 5);
//    QPolygonF controlLine1;
//    controlLine1.append(mStartPoint);
//    controlLine1.append(mControlPoint1);
//    controlLine1Path->addPolygon(controlLine1);

//    controlLine2Path->addEllipse(mEndPoint, 5, 5);
//    controlLine2Path->addEllipse(mControlPoint2, 5, 5);
//    QPolygonF controlLine2;
//    controlLine2.append(mEndPoint);
//    controlLine2.append(mControlPoint2);
//    controlLine2Path->addPolygon(controlLine2);

//    startPointPath->addEllipse(mStartPoint, 10, 10);

//    endPointPath->addEllipse(mEndPoint, 5, 5);

//    return true;
//}


void SCXMLTransition::Update()
{
    prepareGeometryChange();
    update();
}

void SCXMLTransition::Connect(SCXMLState *parentState, SCXMLState *targetState)
{
    setTargetState(targetState);
    parentState->addTransition(this);
    targetState->AddIncomingTransition(this);

    // ensure size changes of the parent state are reflected in the transition start and end connectors
    connect(parentState, &SCXMLState::sizeChanged, this, &SCXMLTransition::UpdatePoints);
    connect(targetState, &SCXMLState::sizeChanged, this, &SCXMLTransition::UpdatePoints);

    // adjust start and end points with initial forced update
    parentState->UpdateTransitions();
    targetState->UpdateTransitions();
}
