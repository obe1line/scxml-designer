#ifndef SCXMLTRANSITION_H
#define SCXMLTRANSITION_H

#include <QAbstractTransition>
#include "scxmlstate.h"
#include "metadatasupport.h"


class SCXMLTransition : public QAbstractTransition, public QGraphicsItem, public MetaDataSupport
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    explicit SCXMLTransition(SCXMLState *parent = 0);

    qreal GetX1() { return mStartPoint.x(); }
    qreal GetY1() { return mStartPoint.y(); }
    qreal GetX2() { return mEndPoint.x(); }
    qreal GetY2() { return mEndPoint.y(); }
    qreal GetControlPointX1() { return mControlPoint1.x(); }
    qreal GetControlPointY1() { return mControlPoint1.y(); }
    qreal GetControlPointX2() { return mControlPoint2.x(); }
    qreal GetControlPointY2() { return mControlPoint2.y(); }
    QString GetDescription() { return mDescription; }
    QString GetEvent() { return mEvent; }

    void SetX1(qreal value) { mStartPoint.setX(value); }
    void SetY1(qreal value) { mStartPoint.setY(value); }
    void SetX2(qreal value) { mEndPoint.setX(value); }
    void SetY2(qreal value) { mEndPoint.setY(value); }
    void SetControlPointX1(qreal value) { mControlPoint1.setX(value); }
    void SetControlPointY1(qreal value) { mControlPoint1.setY(value); }
    void SetControlPointX2(qreal value) { mControlPoint2.setX(value); }
    void SetControlPointY2(qreal value) { mControlPoint2.setY(value); }
    void SetDescription(QString value) { mDescription = value; }
    void SetEvent(QString value) { mEvent = value; }

    void setTransitionType(QString transitionType) { mTransitionType = transitionType; }
    QString getTransitionType() { return mTransitionType; }

    //FIXME: these two virtuals need to be implemented fully at some point
    bool eventTest(QEvent * event) { Q_UNUSED(event); return true; }
    void onTransition(QEvent * event) { Q_UNUSED(event) }

    // MetaDataSupport overrides
    void ApplyMetaData(QMap<QString, QString>* mapMetaData);
    QString GetMetaDataString();

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    // QGraphicsItem overrides
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void Update();
    void Connect(SCXMLState* parentState, SCXMLState* targetState);

    bool CalculatePaths(QPainterPath *bezierPath, QPainterPath *arrowHeadPath,
                        QPainterPath *controlLine1Path, QPainterPath *controlLine2Path,
                        QPainterPath *startPointPath, QPainterPath *endPointPath) const;

signals:
    
public slots:
    void UpdatePoints();

private:
    SCXMLState* mParentState;
    QString mTransitionType;
    QPoint mStartPoint;
    QPoint mEndPoint;
    QString mDescription;
    QPoint mControlPoint1;
    QPoint mControlPoint2;
    bool mMovingControlPoint1;
    bool mMovingControlPoint2;
    bool mMovingStartPoint;
    qreal mNewControlPoint1StartX;
    qreal mNewControlPoint1StartY;
    qreal mNewControlPoint2StartX;
    qreal mNewControlPoint2StartY;
    QString mEvent;
};

#endif // SCXMLTRANSITION_H
