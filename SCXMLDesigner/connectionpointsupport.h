#ifndef CONNECTIONPOINTSUPPORT_H
#define CONNECTIONPOINTSUPPORT_H

#include <QPainterPath>

class ConnectionPointSupport
{
 public:
    ConnectionPointSupport() {}

    //! gets the connection point for the given index (1=end point of perimeter of state, 0=start point)
    virtual QPoint GetConnectionPoint(qreal connectionPointIndex) = 0;
    virtual qreal GetConnectionPointIndex(QPoint point) = 0;
    virtual QPainterPath GetNodeOutlinePath() = 0;
};

#endif // CONNECTIONPOINTSUPPORT_H
