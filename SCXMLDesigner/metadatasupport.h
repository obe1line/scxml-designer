#ifndef METADATASUPPORT_H
#define METADATASUPPORT_H

#include <QMap>

class MetaDataSupport
{
public:
    MetaDataSupport();
    virtual void ApplyMetaData(QMap<QString, QString>* mapMetaData) = 0;
    virtual QString GetMetaDataString() = 0;
};

#endif // METADATASUPPORT_H
