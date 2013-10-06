#include <QDebug>
#include <QGraphicsRectItem>
#include "workflow.h"
#include "scxmlstate.h"
#include "scxmltransition.h"
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

        // add the state meta-data comment
        QDomComment metaDataComment = doc.createComment(state->GetMetaDataString());
        element.appendChild(metaDataComment);

        // add the transitions
        foreach(QAbstractTransition* trans, state->transitions()) {
            SCXMLTransition* transition = dynamic_cast<SCXMLTransition*>(trans);
            QDomElement transitionElement = doc.createElement("transition");
            transitionElement.setAttribute("type", transition->getTransitionType());
            SCXMLState* targetState = dynamic_cast<SCXMLState*>(transition->targetState());
            transitionElement.setAttribute("target", targetState->GetId());
            element.appendChild(transitionElement);
        }
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

    // add all the states before we add transitions (they need to exist!)
    QDomNodeList allElements = scxmlRoot.elementsByTagName("state");
    for (int elementPos=0; elementPos<allElements.length(); elementPos++) {
        QDomElement element = allElements.at(elementPos).toElement();
        QString id = element.attribute("id", "unnamed");

        QMap<QString, QString> mapMetaData;
        QDomNodeList allSubNodes = element.childNodes();
        for (int subPos=0; subPos<allSubNodes.length(); subPos++) {
            QDomNode node = allSubNodes.at(subPos);
            if (!node.isComment()) continue;

            // comment detected - is it meta data?
            QDomComment comment = node.toComment();
            QString text = comment.data();
            if (text.contains("META-DATA")) {
                ParseMetaData(text, mapMetaData);
            }
        }

        SCXMLState *newState = new SCXMLState(id);
        newState->ApplyMetaData(mapMetaData);
        addState(newState);
    }

    // add transitions
    for (int elementPos=0; elementPos<allElements.length(); elementPos++) {
        QDomElement element = allElements.at(elementPos).toElement();
        QString id = element.attribute("id", "unnamed");
        SCXMLState *parentState = GetStateById(id);
        QDomNodeList stateTransitions = element.elementsByTagName("transition");
        for (int transitionPos=0; transitionPos<stateTransitions.length(); transitionPos++) {
            QDomElement stateTransition = stateTransitions.at(transitionPos).toElement();
            QString transitionTarget = stateTransition.attribute("target", "");
            QString transitionType = stateTransition.attribute("type", "");

            SCXMLTransition* newTransition = new SCXMLTransition(parentState);
            newTransition->setTransitionType(transitionType);

            QState* targetState = GetStateById(transitionTarget);
            newTransition->setTargetState(targetState);
            parentState->addTransition(newTransition);
        }

        //setInitialState(newState);
    }
}

SCXMLState* Workflow::GetStateById(QString id)
{
    foreach(QObject* child, this->children()) {
        SCXMLState* state = dynamic_cast<SCXMLState*>(child);
        if (state->GetId() == id) return state;
    }
    return NULL;
}

void Workflow::CreateSceneObjects(QGraphicsScene* scene)
{
    foreach(QObject* child, this->children()) {
        SCXMLState* state = dynamic_cast<SCXMLState*>(child);

        QGraphicsRectItem* item = scene->addRect(state->GetXPosition(),
                       state->GetYPosition(),
                       state->GetWidth(),
                       state->GetHeight());
        item->setFlag(QGraphicsItem::ItemIsMovable, true);
    }
}

void Workflow::ParseMetaData(QString text, QMap<QString, QString> &map)
{
    // may need to change this to handle more complex meta-data, for now looks for "[data]" and assumes a single = sign
    QRegExp rx(tr("\\[([^\\[]+)\\]"));
    int pos = 0;
    while ((pos = rx.indexIn(text, pos)) != -1) {
        QString metaData = rx.cap(1);
        QStringList parts = metaData.split('=');
        if (parts.length() > 1) {
            map.remove(parts[0]);
            map.insert(parts[0], parts[1]);
        }
        pos += rx.matchedLength();
    }
}
