#include "scxmlexecutablecontent.h"

SCXMLExecutableActionBase::SCXMLExecutableActionBase()
{
}

SCXMLExecutableContent::SCXMLExecutableContent()
{
}

SCXMLExecutableContent* SCXMLExecutableContent::FromXmlElement(QDomNodeList content)
{
    SCXMLExecutableContent* newContent = new SCXMLExecutableContent();
    for (int elementPos=0; elementPos<content.length(); elementPos++) {
        QDomNode node = content.at(elementPos);
        QDomElement element = node.toElement();
        QString tag = element.tagName();

        if (tag == XMLUtilities::SCXML_TAG_LOG) {
            newContent->AddAction(SCXMLLog::FromXmlElement(&element));
            continue;
        }
        if (tag == XMLUtilities::SCXML_TAG_RAISE) {
            //newContent->AddAction(SCXMLRaise::FromXmlElement(element));
            continue;
        }
        if (tag == XMLUtilities::SCXML_TAG_SEND) {
            //newContent->AddAction(SCXMLSend::FromXmlElement(element));
            continue;
        }
        if (tag == XMLUtilities::SCXML_TAG_SCRIPT) {
            //newContent->AddAction(SCXMLScript::FromXmlElement(element));
            continue;
        }
        if (tag == XMLUtilities::SCXML_TAG_ASSIGN) {
            //newContent->AddAction(SCXMLAssign::FromXmlElement(element));
            continue;
        }
        if (tag == XMLUtilities::SCXML_TAG_CANCEL) {
            //newContent->AddAction(SCXMLCancel::FromXmlElement(element));
            continue;
        }
        if (tag == XMLUtilities::SCXML_TAG_IF) {
            //newContent->AddAction(SCXMLIf::FromXmlElement(element));
            continue;
        }
        if (tag == XMLUtilities::SCXML_TAG_FOREACH) {
            //newContent->AddAction(SCXMLForEach::FromXmlElement(element));
            continue;
        }
    }

    return newContent;
}

void SCXMLExecutableContent::ToXmlElement(QDomDocument &doc, QDomElement containerElement)
{
    foreach (SCXMLExecutableActionBase* action, mActions) {
        action->ToXmlElement(doc, containerElement);
    }
}
