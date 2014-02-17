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
    explicit SCXMLTransition(SCXMLState *source, SCXMLState *target, QString event, QString transitionType, QMap<QString,QString> *metaData);

    Q_PROPERTY(QPoint centrePoint READ getCentrePoint WRITE setCentrePoint NOTIFY centrePointChanged)

    QString GetControlPoints();
    QString GetDescription() { return mDescription; }
    QString GetEvent() { return mEvent; }

    void SetControlPoints(QString value);
    void SetDescription(QString value) { mDescription = value; }

    //void setTransitionType(QString transitionType) { mTransitionType = transitionType; }
    QString getTransitionType() { return mTransitionType; }

    bool eventTest(QEvent * event);
    //FIXME: need to be implemented fully at some point
    void onTransition(QEvent * event) { Q_UNUSED(event) }

    // MetaDataSupport overrides
    void ApplyMetaData(QMap<QString, QString>* mapMetaData);
    QString GetMetaDataString();

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    void Update();
    void Connect();

    bool CalculatePaths(QPainterPath *bezierPath, QPainterPath *arrowHeadPath,
                        QPainterPath *controlLine1Path, QPainterPath *controlLine2Path,
                        QPainterPath *startPointPath, QPainterPath *endPointPath) const;

    QVector<QVector3D> GetControlPoints(QString value);
    void SetConnectorPoints(qreal start, qreal end);
    void UpdateConnectionPointIndexes();
signals:
    void centrePointChanged(QPoint);

public slots:
    void UpdatePoints();
    //void AnimationComplete() { mAnimationActive = false; update(); }

private:
    SCXMLState* mParentState;
    QString mTransitionType;
    QString mDescription;
    QString mEvent;
    qreal mStartConnectionPointIndex;
    qreal mEndConnectionPointIndex;
    SCXMLState* mSourceState;
    SCXMLState* mTargetState;
    bool mConnected;
};

#endif // SCXMLTRANSITION_H
