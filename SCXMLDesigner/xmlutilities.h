#ifndef XMLUTILITIES_H
#define XMLUTILITIES_H

#include <QString>
#include <QDomNode>

class XMLUtilities
{
public:
    XMLUtilities();

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

    static QString GetAttributeOrDefault(QDomElement *element, QString tag, QString fallback);
};

#endif // XMLUTILITIES_H
