#ifndef XMLUTILITIES_H
#define XMLUTILITIES_H

#include <QString>
#include <QDomNode>
#include <QList>
#include <QDomDocument>
#include <QStringList>

class XMLUtilities
{
public:
    XMLUtilities();

    static const QString SCXML_TAG_ASSIGN;
    static const QString SCXML_TAG_CANCEL;
    static const QString SCXML_TAG_DATA;
    static const QString SCXML_TAG_DATAMODEL;
    static const QString SCXML_TAG_EXPR;
    static const QString SCXML_TAG_EVENT;
    static const QString SCXML_TAG_FINAL;
    static const QString SCXML_TAG_FOREACH;
    static const QString SCXML_TAG_ID;
    static const QString SCXML_TAG_IF;
    static const QString SCXML_TAG_INITIAL;
    static const QString SCXML_TAG_LABEL;
    static const QString SCXML_TAG_LOG;
    static const QString SCXML_TAG_NAME;
    static const QString SCXML_TAG_ONENTRY;
    static const QString SCXML_TAG_RAISE;
    static const QString SCXML_TAG_SCRIPT;
    static const QString SCXML_TAG_SCXML;
    static const QString SCXML_TAG_SEND;
    static const QString SCXML_TAG_SRC;
    static const QString SCXML_TAG_STATE;
    static const QString SCXML_TAG_TARGET;
    static const QString SCXML_TAG_TRANSITION;
    static const QString SCXML_TAG_TYPE;
    static const QString SCXML_TAG_VERSION;

    static QString GetAttributeOrDefault(QDomElement *element, QString tag, QString fallback);

    //! Extracts all DOM elements with a name in tags
    static bool GetElementsWithTagNames(QList<QDomNode> &elems, QDomDocument &doc, QStringList tags, bool clear = false);
    static bool GetElementsWithTagName(QList<QDomNode> &elems, QDomDocument &doc, QString tag, bool clear = false);
    static bool GetSingleElementWithTagName(QDomNode &elem, QDomDocument &doc, QString tag);
};

#endif // XMLUTILITIES_H
