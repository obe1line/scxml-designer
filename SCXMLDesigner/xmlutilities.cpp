#include "xmlutilities.h"

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
