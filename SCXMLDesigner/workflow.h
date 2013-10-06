#ifndef WORKFLOW_H
#define WORKFLOW_H

#include <QStateMachine>
#include <QDomDocument>

//! Represents an SCXML workflow
//!
//! This is a state machine constructed from the contents of an SCXML file.
class Workflow : public QStateMachine
{
    Q_OBJECT
public:
    explicit Workflow();

    //! Builds an SCXML representation of this workflow
    void ConstructSCXMLFromStateMachine(QDomDocument& doc);

    //! Builds a state machine representation from the SCXML
    void ConstructStateMachineFromSCXML(QDomDocument& doc);

    //! Gets the name of the workflow
    QString GetWorkflowName() { return mName; }

    //! Gets the raw SCXML for this workflow
    QString GetRawSCXML() { return mRawSCXMLText; }

signals:
    
public slots:
    
private:
    QString mName;
    QString mRawSCXMLText;
};

#endif // WORKFLOW_H
