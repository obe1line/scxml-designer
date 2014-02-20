#ifndef SCXMLTAGS_H
#define SCXMLTAGS_H

#include <QString>

struct SCXMLTags
{
    static const QString SCXML_TAG_SCXML;
    static const QString SCXML_TAG_LOG;
    static const QString SCXML_TAG_LABEL;
    static const QString SCXML_TAG_EXPR;
    static const QString SCXML_TAG_RAISE;
    static const QString SCXML_TAG_SEND;
    static const QString SCXML_TAG_SCRIPT;
    static const QString SCXML_TAG_ASSIGN;
    static const QString SCXML_TAG_CANCEL;
    static const QString SCXML_TAG_IF;
    static const QString SCXML_TAG_FOREACH;
};

const QString SCXMLTags::SCXML_TAG_SCXML("scxml");
const QString SCXMLTags::SCXML_TAG_LOG("log");
const QString SCXMLTags::SCXML_TAG_LABEL("label");
const QString SCXMLTags::SCXML_TAG_EXPR("expr");
const QString SCXMLTags::SCXML_TAG_RAISE("raise");
const QString SCXMLTags::SCXML_TAG_SEND("send");
const QString SCXMLTags::SCXML_TAG_SCRIPT("script");
const QString SCXMLTags::SCXML_TAG_ASSIGN("assign");
const QString SCXMLTags::SCXML_TAG_CANCEL("cancel");
const QString SCXMLTags::SCXML_TAG_IF("if");
const QString SCXMLTags::SCXML_TAG_FOREACH("foreach");

#endif // SCXMLTAGS_H
