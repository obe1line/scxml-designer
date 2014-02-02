#ifndef CHAIKINCURVE_H
#define CHAIKINCURVE_H

#include <QGraphicsItem>
#include <QVector>
#include <QVector3D>


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

    // QGraphicsItem interface
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
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
};


#endif // CHAIKINCURVE_H
