#include "workflow.h"
#include "scxmlstate.h"
#include "utilities.h"

Workflow::Workflow() :
    QStateMachine()
{
}

void Workflow::ConstructSCXMLFromStateMachine(QDomDocument &doc)
{
    // ensure we have no existing content
    doc.clear();

    // create the root element with name attribute
    QDomElement rootElement = doc.createElementNS("http://www.w3.org/2005/07/scxml", "scxml");
    rootElement.setAttribute("name", mName);
    rootElement.setAttribute("version", "1.0");
    doc.appendChild(rootElement);

    // traverse the states to build up the SCXML document
    foreach(QObject* child, this->children()) {
        SCXMLState* state = dynamic_cast<SCXMLState*>(child);

        QDomElement element = doc.createElement("state");
        element.setAttribute("id", state->GetId());
        rootElement.appendChild(element);
    }
}

void Workflow::ConstructStateMachineFromSCXML(QDomDocument &doc)
{
    mRawSCXMLText = doc.toString();

    // ensure we have no existing state machine
    foreach(QObject* child, this->children()) {
        SCXMLState* state = dynamic_cast<SCXMLState*>(child);
        removeState(state);
    }

    // traverse the SCXML to build up the state machine
    QDomNodeList scxmlElements = doc.elementsByTagName("scxml");
    if (scxmlElements.length() != 1) {
        Utilities::ShowWarning("SCXML file does not have a single scxml tag");
        return;
    }

    // get the name of the workflow
    QDomElement scxmlRoot = scxmlElements.at(0).toElement();
    mName = scxmlRoot.attribute("name", "Unnamed");

    QDomNodeList allElements = scxmlRoot.elementsByTagName("state");
    for (int elementPos=0; elementPos<allElements.length(); elementPos++) {
        QDomElement element = allElements.at(elementPos).toElement();
        QString id = element.attribute("id", "unnamed");

        SCXMLState *newState = new SCXMLState(id);
        addState(newState);
        setInitialState(newState);
    }
}
