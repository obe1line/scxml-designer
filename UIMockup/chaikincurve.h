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
    QVector<QVector3D> mCurvePoints;
};

#endif // CHAIKINCURVE_H
