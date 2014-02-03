#ifndef SCXMLTRANSITION_H
#define SCXMLTRANSITION_H

#include <QAbstractTransition>
#include "scxmlstate.h"
#include "metadatasupport.h"
#include "chaikincurve.h"

class SCXMLTransition : public QAbstractTransition, public ChaikinCurve, public MetaDataSupport
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    explicit SCXMLTransition(SCXMLState *parent = 0);

//    qreal GetX1() { return mStartPoint.x(); }
//    qreal GetY1() { return mStartPoint.y(); }
//    qreal GetX2() { return mEndPoint.x(); }
//    qreal GetY2() { return mEndPoint.y(); }
    QString GetControlPoints();
    QString GetDescription() { return mDescription; }
    QString GetEvent() { return mEvent; }

//    void SetX1(qreal value) { mStartPoint.setX(value); }
//    void SetY1(qreal value) { mStartPoint.setY(value); }
//    void SetX2(qreal value) { mEndPoint.setX(value); }
//    void SetY2(qreal value) { mEndPoint.setY(value); }
//    void SetControlPointX1(qreal value) { mControlPoint1.setX(value); }
//    void SetControlPointY1(qreal value) { mControlPoint1.setY(value); }
//    void SetControlPointX2(qreal value) { mControlPoint2.setX(value); }
//    void SetControlPointY2(qreal value) { mControlPoint2.setY(value); }
    void SetControlPoints(QString value);
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

    void Update();
    void Connect(SCXMLState* parentState, SCXMLState* targetState);

    bool CalculatePaths(QPainterPath *bezierPath, QPainterPath *arrowHeadPath,
                        QPainterPath *controlLine1Path, QPainterPath *controlLine2Path,
                        QPainterPath *startPointPath, QPainterPath *endPointPath) const;

    QVector<QVector3D> GetControlPoints(QString value);
signals:
    
public slots:
    void UpdatePoints();

private:
    SCXMLState* mParentState;
    QString mTransitionType;
    QString mDescription;
    QString mEvent;
};

#endif // SCXMLTRANSITION_H
