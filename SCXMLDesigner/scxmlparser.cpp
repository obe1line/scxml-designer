#include "scxmlparser.h"
#include "xmlutilities.h"
#include "scxmltags.h"

SCXMLParser::SCXMLParser() :
    mName("unnamed.scxml"),
    mRawSCXMLText("")
{
}

bool SCXMLParser::Deserialise(QDomDocument &doc)
{
    // keep the original SCXML but indent it
    mRawSCXMLText = doc.toString(1);

    QDomNode rootElement;
    if (!XMLUtilities::GetSingleElementWithTagName(rootElement, doc, SCXMLTags::SCXML_TAG_SCXML))
    {
        // invalid document - no scxml element, or more than one
        //TODO: LOG(ERROR, "Invlalid SCXML document - single scxml root node expected");
        return false;
    }

    return true;
}

bool SCXMLParser::Serialise(QDomDocument &doc)
{
    // ensure we have no existing content
    doc.clear();

    // create the root element with name attribute
    QDomElement rootElement = doc.createElementNS("http://www.w3.org/2005/07/scxml", "scxml");
    rootElement.setAttribute("name", mName);
    rootElement.setAttribute("version", "1.0");
    doc.appendChild(rootElement);

    return true;
}

/*
    // data model
    if (mDataModel.HasItems()) {
        QDomElement dataModelElement = doc.createElement("datamodel");
        rootElement.appendChild(dataModelElement);
        foreach (SCXMLDataItem* dataItem, mDataModel.GetDataItemList()) {
            QDomElement dataElement = doc.createElement("data");
            dataElement.setAttribute("id", dataItem->GetId());
            if (dataItem->GetSrc() != "") {
                dataElement.setAttribute("src", dataItem->GetSrc());
            }
            if (dataItem->GetExpr() != "") {
                dataElement.setAttribute("expr", dataItem->GetExpr());
            }
            dataModelElement.appendChild(dataElement);
        }
    }

    // traverse the states to build up the SCXML document
    foreach(QObject* child, this->children()) {
        SCXMLState* state = dynamic_cast<SCXMLState*>(child);

        QDomElement element = doc.createElement(state->GetFinal() ? "final" : "state");
        element.setAttribute("id", state->GetId());

        // add the state meta-data comment
        QDomComment metaDataComment = doc.createComment(state->GetMetaDataString());
        element.appendChild(metaDataComment);

        // add the onentry
//        QDomElement onEntryElement = state->GetOnEntry()->ToXMLElement(doc);
//        element.appendChild(onEntryElement);

        // add the transitions
        foreach(QAbstractTransition* trans, state->transitions()) {
            SCXMLTransition* transition = dynamic_cast<SCXMLTransition*>(trans);
            QDomElement transitionElement = doc.createElement("transition");
            transitionElement.setAttribute("type", transition->getTransitionType());
            SCXMLState* targetState = dynamic_cast<SCXMLState*>(transition->targetState());
            transitionElement.setAttribute("target", targetState->GetId());
            QString event = transition->GetEvent();
            if (!event.isEmpty()) {
                transitionElement.setAttribute("event", event);
            }
            transitionElement.setAttribute("target", targetState->GetId());

            // add the transition meta-data comment
            QDomComment metaDataComment = doc.createComment(transition->GetMetaDataString());
            transitionElement.appendChild(metaDataComment);

            element.appendChild(transitionElement);
        }
        rootElement.appendChild(element);
    }
}
 * */
