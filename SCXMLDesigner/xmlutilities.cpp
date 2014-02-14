#include "xmlutilities.h"

const QString XMLUtilities::SCXML_TAG_ASSIGN = "assign";
const QString XMLUtilities::SCXML_TAG_CANCEL = "cancel";
const QString XMLUtilities::SCXML_TAG_DATA = "data";
const QString XMLUtilities::SCXML_TAG_DATAMODEL = "datamodel";
const QString XMLUtilities::SCXML_TAG_EXPR = "expr";
const QString XMLUtilities::SCXML_TAG_EVENT = "event";
const QString XMLUtilities::SCXML_TAG_FINAL = "final";
const QString XMLUtilities::SCXML_TAG_FOREACH = "foreach";
const QString XMLUtilities::SCXML_TAG_ID = "id";
const QString XMLUtilities::SCXML_TAG_IF = "if";
const QString XMLUtilities::SCXML_TAG_INITIAL = "initial";
const QString XMLUtilities::SCXML_TAG_LABEL = "label";
const QString XMLUtilities::SCXML_TAG_LOG = "log";
const QString XMLUtilities::SCXML_TAG_NAME = "name";
const QString XMLUtilities::SCXML_TAG_ONENTRY = "onentry";
const QString XMLUtilities::SCXML_TAG_RAISE = "raise";
const QString XMLUtilities::SCXML_TAG_SCRIPT = "script";
const QString XMLUtilities::SCXML_TAG_SCXML = "scxml";
const QString XMLUtilities::SCXML_TAG_SEND = "send";
const QString XMLUtilities::SCXML_TAG_SRC = "src";
const QString XMLUtilities::SCXML_TAG_STATE = "state";
const QString XMLUtilities::SCXML_TAG_TARGET = "target";
const QString XMLUtilities::SCXML_TAG_TRANSITION = "transition";
const QString XMLUtilities::SCXML_TAG_TYPE = "type";
const QString XMLUtilities::SCXML_TAG_VERSION = "version";


XMLUtilities::XMLUtilities()
{
}

QString XMLUtilities::GetAttributeOrDefault(QDomElement *element, QString tag, QString fallback)
{
    return element->attribute(tag, fallback);
}

bool XMLUtilities::GetElementsWithTagNames(QList<QDomNode> &elems, QDomDocument &doc, QStringList tags, bool clear)
{
    if (clear) elems.clear();
    foreach (QString tag, tags) {
        GetElementsWithTagName(elems, doc, tag, false);
    }

    return true;
}

bool XMLUtilities::GetElementsWithTagName(QList<QDomNode> &elems, QDomDocument &doc, QString tag, bool clear)
{
    if (clear) elems.clear();
    QDomNodeList elements = doc.elementsByTagName(tag);
    for (int pos=0; pos < elements.length(); pos++) {
        elems.append(elements.at(pos));
    }
    return true;
}

bool XMLUtilities::GetSingleElementWithTagName(QDomNode &elem, QDomDocument &doc, QString tag)
{
    QList<QDomNode> elements;
    XMLUtilities::GetElementsWithTagName(elements, doc, tag, true);
    if (elements.length() != 1) {
        return false;
    }
    elem = elements.at(0);
    return true;
}
