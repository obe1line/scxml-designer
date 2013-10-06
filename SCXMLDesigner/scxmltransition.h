#ifndef SCXMLTRANSITION_H
#define SCXMLTRANSITION_H

#include <QAbstractTransition>
#include "scxmlstate.h"

class SCXMLTransition : public QAbstractTransition
{
    Q_OBJECT
public:
    explicit SCXMLTransition(SCXMLState *parent = 0);

    void setTransitionType(QString transitionType) { mTransitionType = transitionType; }
    QString getTransitionType() { return mTransitionType; }

    //FIXME: these two virtuals need to be implemented fully at some point
    bool eventTest(QEvent * event) { Q_UNUSED(event); return true; }
    void onTransition(QEvent * event) { Q_UNUSED(event) }
signals:
    
public slots:
    
private:
    SCXMLState* mParentState;
    QString mTransitionType;
};

#endif // SCXMLTRANSITION_H
