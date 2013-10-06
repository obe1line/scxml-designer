#include "workflow.h"
#include "scxmlstate.h"

Workflow::Workflow() :
    QStateMachine()
{
}


void Workflow::ConstructSCXMLFromStateMachine(QDomDocument &doc)
{
    // ensure we have no existing content
    doc.clear();

    // root element
    QDomElement rootElement = doc.createElement("scxml");
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
    // ensure we have no existing state machine
    foreach(QObject* child, this->children()) {
        SCXMLState* state = dynamic_cast<SCXMLState*>(child);
        removeState(state);
    }

    // traverse the SCXML to build up the state machine
    QDomNodeList allElements = doc.elementsByTagName("state");
    for (int elementPos=0; elementPos<allElements.length(); elementPos++) {
        QDomElement element = allElements.at(elementPos).toElement();
        QDomAttr idAttr = element.attributes().namedItem("id").toAttr();
        QString id = idAttr.value();

        SCXMLState *newState = new SCXMLState(id);
        addState(newState);
        setInitialState(newState);
    }

//    foreach(QObject* child, this->children()) {
//        SCXMLState* state = dynamic_cast<SCXMLState*>(child);

//        QDomElement element = doc.createElement("State");
//        element.setAttribute("id", state->GetId());
//        doc.appendChild(element);
//    }
}
