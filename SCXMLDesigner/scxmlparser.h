#ifndef SCXMLPARSER_H
#define SCXMLPARSER_H

#include <QDomDocument>


class SCXMLParser
{
public:
    SCXMLParser();

    bool Deserialise(QDomDocument &doc);
    bool Serialise(QDomDocument &doc);

    QString GetRawSCXML() { return mRawSCXMLText; }

private:
    QString mName;
    QString mRawSCXMLText;
    //SCXMLDataModel mDataModel;
};

#endif // SCXMLPARSER_H
