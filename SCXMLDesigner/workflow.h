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
    
    //! Loads a workflow from an SCXML file
    bool LoadWorkflow(QString workflowPath);

    //! Saves a workflow to an SCXML file
    bool SaveWorkflow(QString workflowPath);

    //! Builds an SCXML representation of this workflow
    void ConstructSCXMLFromStateMachine(QDomDocument& doc);

    //! Builds a state machine representation from the SCXML
    void ConstructStateMachineFromSCXML(QDomDocument& doc);
signals:
    
public slots:
    
private:
    QString mName;
};

#endif // WORKFLOW_H
