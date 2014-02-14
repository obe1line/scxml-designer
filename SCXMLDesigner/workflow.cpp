#include <QDebug>
#include <QGraphicsRectItem>
#include "workflow.h"
#include "scxmlstate.h"
#include "scxmltransition.h"
#include "utilities.h"
#include "scxmlexecutablecontent.h"

Workflow::Workflow() :
    QStateMachine()
{
}

void Workflow::ConstructSCXMLFromStateMachine(QDomDocument &doc)
{
    // ensure we have no existing content
    doc.clear();

    // create the root element with name attribute
    QDomElement rootElement = doc.createElementNS("http://www.w3.org/2005/07/scxml", XMLUtilities::SCXML_TAG_SCXML);
    if (mName != "") rootElement.setAttribute(XMLUtilities::SCXML_TAG_NAME, mName);
    if (mInitialStateName != "") rootElement.setAttribute(XMLUtilities::SCXML_TAG_INITIAL, mInitialStateName);
    rootElement.setAttribute(XMLUtilities::SCXML_TAG_VERSION, "1.0");
    doc.appendChild(rootElement);

    // data model
    if (mDataModel.HasItems()) {
        QDomElement dataModelElement = doc.createElement(XMLUtilities::SCXML_TAG_DATAMODEL);
        rootElement.appendChild(dataModelElement);
        foreach (SCXMLDataItem* dataItem, mDataModel.GetDataItemList()) {
            QDomElement dataElement = doc.createElement(XMLUtilities::SCXML_TAG_DATA);
            dataElement.setAttribute(XMLUtilities::SCXML_TAG_ID, dataItem->GetId());
            if (dataItem->GetSrc() != "") {
                dataElement.setAttribute(XMLUtilities::SCXML_TAG_SRC, dataItem->GetSrc());
            }
            if (dataItem->GetExpr() != "") {
                dataElement.setAttribute(XMLUtilities::SCXML_TAG_EXPR, dataItem->GetExpr());
            }
            dataModelElement.appendChild(dataElement);
        }
    }

    // traverse the states to build up the SCXML document
    foreach(QObject* child, this->children()) {
        SCXMLState* state = dynamic_cast<SCXMLState*>(child);

        QDomElement element = doc.createElement(state->GetFinal() ? XMLUtilities::SCXML_TAG_FINAL : XMLUtilities::SCXML_TAG_STATE);
        element.setAttribute(XMLUtilities::SCXML_TAG_ID, state->GetId());

        // add the state meta-data comment
        QDomComment metaDataComment = doc.createComment(state->GetMetaDataString());
        element.appendChild(metaDataComment);

        // add the onentry
        SCXMLExecutableContent* onEntry = state->GetOnEntry();
        if (onEntry != nullptr) {
            QDomElement onEntryElement =  doc.createElement(XMLUtilities::SCXML_TAG_ONENTRY);
            element.appendChild(onEntryElement);
        }
        // add the transitions
        foreach(QAbstractTransition* trans, state->transitions()) {
            SCXMLTransition* transition = dynamic_cast<SCXMLTransition*>(trans);
            QDomElement transitionElement = doc.createElement(XMLUtilities::SCXML_TAG_TRANSITION);
            if (transition->getTransitionType() != "") {
                transitionElement.setAttribute(XMLUtilities::SCXML_TAG_TYPE, transition->getTransitionType());
            }
            SCXMLState* targetState = dynamic_cast<SCXMLState*>(transition->targetState());
            transitionElement.setAttribute(XMLUtilities::SCXML_TAG_TARGET, targetState->GetId());
            QString event = transition->GetEvent();
            if (!event.isEmpty()) {
                transitionElement.setAttribute(XMLUtilities::SCXML_TAG_EVENT, event);
            }
            transitionElement.setAttribute(XMLUtilities::SCXML_TAG_TARGET, targetState->GetId());

            // add the transition meta-data comment
            QDomComment metaDataComment = doc.createComment(transition->GetMetaDataString());
            transitionElement.appendChild(metaDataComment);

            element.appendChild(transitionElement);
        }
        rootElement.appendChild(element);
    }
}

void Workflow::ConstructStateMachineFromSCXML(QDomDocument &doc)
{
    SCXMLState* initialState = nullptr;

    mRawSCXMLText = doc.toString();

    // ensure we have no existing state machine
    foreach(QObject* child, this->children()) {
        SCXMLState* state = dynamic_cast<SCXMLState*>(child);
        removeState(state);
    }

    // traverse the SCXML to build up the state machine
    QDomNodeList scxmlElements = doc.elementsByTagName(XMLUtilities::SCXML_TAG_SCXML);
    if (scxmlElements.length() != 1) {
        Utilities::ShowWarning("SCXML file does not have a single scxml tag");
        return;
    }

    // get the name of the workflow
    QDomElement scxmlRoot = scxmlElements.at(0).toElement();
    mName = scxmlRoot.attribute(XMLUtilities::SCXML_TAG_NAME, "");
    mInitialStateName = scxmlRoot.attribute(XMLUtilities::SCXML_TAG_INITIAL, "");

    // add all the states before we add transitions (they need to exist!)
    QStringList stateTags;
    stateTags << XMLUtilities::SCXML_TAG_STATE << XMLUtilities::SCXML_TAG_FINAL;
    QList<QDomNode> allElements;
    XMLUtilities::GetElementsWithTagNames(allElements, doc, stateTags);
    for (int elementPos=0; elementPos<allElements.length(); elementPos++) {
        QDomElement element = allElements.at(elementPos).toElement();
        QString id = element.attribute(XMLUtilities::SCXML_TAG_ID, "unnamed");
        QDomNodeList onEntryElements = element.elementsByTagName(XMLUtilities::SCXML_TAG_ONENTRY);
        SCXMLExecutableContent* onEntryContent = nullptr;
        if (onEntryElements.count() > 0) {
            onEntryContent = SCXMLExecutableContent::FromXmlElement(onEntryElements.at(0).childNodes());
        }

        QMap<QString,QString> metaData = ExtractMetaDataFromElementComments(&element);
        SCXMLState *newState = new SCXMLState(id, &metaData);
        ExtractDataModelFromElement(&element, newState);
        newState->SetFinal(element.tagName() == XMLUtilities::SCXML_TAG_FINAL);
        newState->SetOnEntry(onEntryContent);

        addState(newState);
        if (id == mInitialStateName) {
            initialState = newState;
        }
    }

    // add top level data model if exists
    ExtractDataModelFromElement(&scxmlRoot, nullptr);

    // add transitions
    for (int elementPos=0; elementPos<allElements.length(); elementPos++) {
        QDomElement element = allElements.at(elementPos).toElement();
        QString id = element.attribute(XMLUtilities::SCXML_TAG_ID, "unnamed");
        SCXMLState *sourceState = GetStateById(id);
        QDomNodeList stateTransitions = element.elementsByTagName(XMLUtilities::SCXML_TAG_TRANSITION);
        for (int transitionPos=0; transitionPos<stateTransitions.length(); transitionPos++) {
            QDomElement stateTransition = stateTransitions.at(transitionPos).toElement();
            QString transitionTarget = stateTransition.attribute(XMLUtilities::SCXML_TAG_TARGET, "");
            QString transitionType = stateTransition.attribute(XMLUtilities::SCXML_TAG_TYPE, "");
            QString transitionEvent = stateTransition.attribute(XMLUtilities::SCXML_TAG_EVENT, "");

            SCXMLState* targetState = GetStateById(transitionTarget);
            if (targetState == nullptr) {
                qDebug() << "No such state: " << transitionTarget;
                continue;
            }
            QMap<QString,QString> metaData = ExtractMetaDataFromElementComments(&stateTransition);
            SCXMLTransition* newTransition = new SCXMLTransition(sourceState, targetState, transitionEvent, transitionType, &metaData);
        }

        // need to adjust start and end points with update
        sourceState->UpdateTransitions();

        // set the initial state of the state machine
        if (initialState != nullptr) {
            setInitialState(initialState);
        }
    }
}

void Workflow::ExtractDataModelFromElement(QDomElement* element, SCXMLState* state)
{
    QDomNodeList allSubNodes = element->childNodes();
    for (int subPos=0; subPos<allSubNodes.length(); subPos++) {
        QDomNode node = allSubNodes.at(subPos);
        if (!node.isElement()) continue;
        QDomElement elementDataModel = node.toElement();
        if (elementDataModel.nodeName() != "datamodel") continue;

        // found the data model, now traverse the data items
        QDomNodeList dataNodes = elementDataModel.elementsByTagName("data");
        for (int dataPos=0; dataPos<dataNodes.length(); dataPos++) {
            QDomNamedNodeMap attrMap = dataNodes.at(dataPos).attributes();
            QString src = "";
            QString expr = "";
            if (attrMap.contains("src")) src = attrMap.namedItem("src").toAttr().value();
            if (attrMap.contains("expr")) expr = attrMap.namedItem("expr").toAttr().value();
            if (attrMap.contains("id")) {
                SCXMLDataItem* dataItem = new SCXMLDataItem(attrMap.namedItem("id").toAttr().value(), src, expr);
                mDataModel.AddDataItem(dataItem);
                if (state != nullptr) {
                    //TODO: add details to state
                }
            }
        }
    }
}

QMap<QString, QString> Workflow::ExtractMetaDataFromElementComments(QDomElement* element)
{
    QMap<QString, QString> mapMetaData;
    QDomNodeList allSubNodes = element->childNodes();
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

    return mapMetaData;
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
        QGraphicsItem* item = dynamic_cast<QGraphicsItem*>(state);
        scene->addItem(item);

        foreach(QAbstractTransition* abtran, state->transitions()) {
            SCXMLTransition* tran = dynamic_cast<SCXMLTransition*>(abtran);
            QGraphicsItem* itemTran = dynamic_cast<QGraphicsItem*>(tran);
            scene->addItem(itemTran);
        }
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
