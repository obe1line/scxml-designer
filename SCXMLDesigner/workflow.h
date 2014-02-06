#ifndef WORKFLOW_H
#define WORKFLOW_H

#include <QStateMachine>
#include <QDomDocument>
#include <QGraphicsScene>
#include "scxmlstate.h"
#include "scxmldatamodel.h"

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

    //! Gets a state defined by id if it exists, NULL otherwise
    SCXMLState* GetStateById(QString id);

    //! Creates the scene objects that correspond with the workflow
    void CreateSceneObjects(QGraphicsScene *scene);

    //! Parser the meta data comment text
    void ParseMetaData(QString text, QMap<QString, QString>& map);

    //! Extract the meta data from an element comment child nodes
    QMap<QString, QString> ExtractMetaDataFromElementComments(QDomElement *element);

    //! Extracts all DOM elements with a name in tags
    QList<QDomNode> GetElementsWithTagName(QDomDocument *doc, QStringList tags);

    //! Extracts the data model from a given element (looks in the child nodes)
    void ExtractDataModelFromElement(QDomElement* element, SCXMLState* state);

    //! Gets the underlying data model
    SCXMLDataModel *GetDataModel() { return &mDataModel; }
signals:
    
public slots:
    
private:
    QString mName;
    QString mRawSCXMLText;
    SCXMLDataModel mDataModel;
};

#endif // WORKFLOW_H
