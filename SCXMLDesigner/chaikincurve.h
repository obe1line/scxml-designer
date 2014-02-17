#ifndef CHAIKINCURVE_H
#define CHAIKINCURVE_H

#include <QGraphicsItem>
#include <QVector>
#include <QVector3D>
#include <QGraphicsScene>
#include <QWidget>
#include <QPainter>
#include <QPropertyAnimation>
#include "connectionpointsupport.h"

class ChaikinCurve : public QGraphicsItem
{
    Q_INTERFACES(QGraphicsItem)
public:
    explicit ChaikinCurve(int iterationCount, QVector<QVector3D> points);

    QVector<QVector3D> GetCurveControlPoints() { return mOriginalCurvePoints; }

    void setCentrePoint(QPoint pt) { mCentrePoint = pt; this->update(); }
    QPoint getCentrePoint() const { return mCentrePoint; }
    bool mAnimationActive;
    void SetParentObject(QObject* obj) { mParentObject = obj; }

private:
    QPoint mCentrePoint;
    QObject *mParentObject;

signals:
    void centrePointChanged(QPoint);

public slots:
    void IncreaseLod();
    void DecreaseLod();

private:
    int mIterationCount;
    QBrush *mYellowBrush;
    QBrush *mGreenBrush;
    QBrush *mBlackBrush;
    QPen *mControlPointPen;
    QPen *mLinePen;
    QVector<QVector3D> mCurvePoints;
    QVector<QVector3D> mOriginalCurvePoints;
    bool mControlPointVisible;
    bool mDragInProgress;
    int mControlPointDragIndex;
    QPainterPath mStartNodePath;
    QPainterPath mEndNodePath;
    QPixmap mArrowImage;
    ConnectionPointSupport *mStartNodePathConnectionPointSupport;
    ConnectionPointSupport *mEndNodePathConnectionPointSupport;

    QPainterPath GetPathOfLines() const;
    QPainterPath GetPathOfControlPoints() const;
    void SetNewPointPosition(int controlPointIndex, QPointF dragDropPoint);
    void InitializeCurvePoints();
    int GetIndexOfControlPoint(QPointF pointerPosition);
    void DrawArrow(QPainter *painter);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    QPainterPath shape() const;

    void AnimateEvent();
    void SetStartingPoints(QVector<QVector3D> newCurvePoints);
    //void SetStartNodePath(QPainterPath path) { mStartNodePath = path; }
    void SetStartNodeConnectionPointSupport(ConnectionPointSupport *support) {
        mStartNodePathConnectionPointSupport = support;
        mStartNodePath = support->GetNodeOutlinePath();
    }
    //void SetEndNodePath(QPainterPath path) { mEndNodePath = path; }
    void SetEndNodeConnectionPointSupport(ConnectionPointSupport *support) {
        mEndNodePathConnectionPointSupport = support;
        mEndNodePath = support->GetNodeOutlinePath();
    }
    QPoint constrainPointToBoundary(QPoint point, ConnectionPointSupport *support);
    QPropertyAnimation* GetTestAnimation();
};
#endif // CHAIKINCURVE_H
