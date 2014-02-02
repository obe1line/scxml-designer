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

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

signals:

public slots:
    void IncreaseLod();
    void DecreaseLod();

private:
    QBrush *mYellowBrush;
    QPen *mMoveablePointPen;
    QVector<QVector3D> mCurvePoints;
    QVector<QVector3D> mOriginalCurvePoints;
};

#endif // CHAIKINCURVE_H
