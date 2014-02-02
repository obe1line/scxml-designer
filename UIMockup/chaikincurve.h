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
    QPointF mControlDragStartPoint;

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
    QPainterPath GetPathOfLines() const;
    QPainterPath GetPathOfControlPoints() const;
    void SetNewPointPosition(QPointF dragStartPoint, QPointF dragDropPoint);

    // QGraphicsItem interface
    void InitializeCurvePoints();
protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);
};


#endif // CHAIKINCURVE_H
