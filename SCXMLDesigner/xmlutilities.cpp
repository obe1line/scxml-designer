#include "xmlutilities.h"

const QString XMLUtilities::SCXML_TAG_LOG = "log";
const QString XMLUtilities::SCXML_TAG_LABEL = "label";
const QString XMLUtilities::SCXML_TAG_EXPR = "expr";
const QString XMLUtilities::SCXML_TAG_RAISE = "raise";
const QString XMLUtilities::SCXML_TAG_SEND = "send";
const QString XMLUtilities::SCXML_TAG_SCRIPT = "script";
const QString XMLUtilities::SCXML_TAG_ASSIGN = "assign";
const QString XMLUtilities::SCXML_TAG_CANCEL = "cancel";
const QString XMLUtilities::SCXML_TAG_IF = "if";
const QString XMLUtilities::SCXML_TAG_FOREACH = "foreach";

XMLUtilities::XMLUtilities()
{
}

QString XMLUtilities::GetAttributeOrDefault(QDomElement *element, QString tag, QString fallback)
{
    return element->attribute(tag, fallback);
}
