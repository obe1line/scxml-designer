#ifndef CHAIKINCURVE_H
#define CHAIKINCURVE_H

#include <QGraphicsItem>
#include <QVector>
#include <QVector3D>
#include <QGraphicsScene>


class ChaikinCurve : public QGraphicsItem
{
    Q_INTERFACES(QGraphicsItem)
public:
    explicit ChaikinCurve();

public slots:
    void IncreaseLod();
    void DecreaseLod();

private:
    QBrush *mYellowBrush;
    QPen *mControlPointPen;
    QVector<QVector3D> mCurvePoints;
    QVector<QVector3D> mOriginalCurvePoints;
    bool mControlPointVisible;
    bool mDragInProgress;
    int mControlPointDragIndex;

    QPainterPath GetPathOfLines() const;
    QPainterPath GetPathOfControlPoints() const;
    void SetNewPointPosition(int controlPointIndex, QPointF dragDropPoint);
    void InitializeCurvePoints();
    int GetIndexOfControlPoint(QPointF pointerPosition);

    // QGraphicsItem interface
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    // QGraphicsItem interface
public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    QPainterPath shape() const;
};


#endif // CHAIKINCURVE_H
