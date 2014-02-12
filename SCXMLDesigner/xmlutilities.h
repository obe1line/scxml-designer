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

    static QString GetAttributeOrDefault(QDomElement *element, QString tag, QString fallback);

    //! Extracts all DOM elements with a name in tags
    static bool GetElementsWithTagNames(QList<QDomNode> &elems, QDomDocument &doc, QStringList tags, bool clear = false);
    static bool GetElementsWithTagName(QList<QDomNode> &elems, QDomDocument &doc, QString tag, bool clear = false);
    static bool GetSingleElementWithTagName(QDomNode &elem, QDomDocument &doc, QString tag);
};

#endif // XMLUTILITIES_H
