#ifndef CHAIKINCURVE_H
#define CHAIKINCURVE_H

#include <QGraphicsItem>
#include <QVector>
#include <QVector3D>
#include <QGraphicsScene>
#include <QWidget>
#include <QPainter>


class ChaikinCurve : public QGraphicsItem
{
    Q_INTERFACES(QGraphicsItem)
public:
    explicit ChaikinCurve(int iterationCount, QVector<QVector3D> points);

    QVector<QVector3D> GetCurveControlPoints() { return mOriginalCurvePoints; }
//    Q_PROPERTY(QPoint centrePoint READ getCentrePoint WRITE setCentrePoint NOTIFY centrePointChanged)

//    void setCentrePoint(QPoint pt) { mCentrePoint = pt; this->update(); }
//    QPoint getCentrePoint() const { return mCentrePoint; }
//    bool mAnimationActive;
//private:
//    QPoint mCentrePoint;

//signals:
//    void centrePointChanged(QPoint);

public slots:
    void IncreaseLod();
    void DecreaseLod();
    //void AnimationComplete() { mAnimationActive = false; update(); }

private:
    int mIterationCount;
    QBrush *mYellowBrush;
    QBrush *mGreenBrush;
    QPen *mControlPointPen;
    QVector<QVector3D> mCurvePoints;
    QVector<QVector3D> mOriginalCurvePoints;
    bool mControlPointVisible;
    bool mDragInProgress;
    int mControlPointDragIndex;
    QPainterPath mStartNodePath;
    QPainterPath mEndNodePath;

    QPainterPath GetPathOfLines() const;
    QPainterPath GetPathOfControlPoints() const;
    void SetNewPointPosition(int controlPointIndex, QPointF dragDropPoint);
    void InitializeCurvePoints();
    int GetIndexOfControlPoint(QPointF pointerPosition);

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
    void SetStartNodePath(QPainterPath path) { mStartNodePath = path; }
    void SetEndNodePath(QPainterPath path) { mEndNodePath = path; }
};
#endif // CHAIKINCURVE_H
