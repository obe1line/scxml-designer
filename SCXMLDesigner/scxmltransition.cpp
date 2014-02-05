#include <QCursor>
#include <QVector2D>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include "scxmltransition.h"

SCXMLTransition::SCXMLTransition(SCXMLState *parent) :
    QAbstractTransition(), ChaikinCurve(4, QVector<QVector3D>()), mParentState(parent),
    mDescription(""), mEvent(""), mStartConnectionPointIndex(0), mEndConnectionPointIndex(0)
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
//! \brief SCXMLTransition::SetConnectorPoints
//!
//! Sets the percentage index of where the connection point is located on the
//! start and end states.
//!
//! \param start  = start state connection point index (0..1)
//! \param end    = end state connection point index (0..1)
//!
void SCXMLTransition::SetConnectorPoints(qreal start, qreal end)
{
    mStartConnectionPointIndex = start;
    mEndConnectionPointIndex = end;
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
        QPoint startPoint = startState->GetConnectionPoint(mStartConnectionPointIndex);
        QPoint endPoint = endState->GetConnectionPoint(mEndConnectionPointIndex);
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

void SCXMLTransition::UpdateConnectionPointIndexes()
{
    SCXMLState* startState = dynamic_cast<SCXMLState*>(mParentState);
    SCXMLState* endState = dynamic_cast<SCXMLState*>(targetState());
    if (endState != nullptr) {
        QVector<QVector3D> curvePoints = GetCurveControlPoints();
        mStartConnectionPointIndex = startState->GetConnectionPointIndex(curvePoints[0].toPoint());
        mEndConnectionPointIndex = endState->GetConnectionPointIndex(curvePoints[curvePoints.length()-1].toPoint());
    }
}

void SCXMLTransition::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    ChaikinCurve::mouseReleaseEvent(event);
    UpdateConnectionPointIndexes();
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

    //UpdateConnectionPointIndexes();

    // ensure size changes of the parent state are reflected in the transition start and end connectors
    connect(parentState, &SCXMLState::sizeChanged, this, &SCXMLTransition::UpdatePoints);
    connect(targetState, &SCXMLState::sizeChanged, this, &SCXMLTransition::UpdatePoints);

    // adjust start and end points with initial forced update
    parentState->UpdateTransitions();
    targetState->UpdateTransitions();
}
