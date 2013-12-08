#ifndef SCXMLDATAMODEL_H
#define SCXMLDATAMODEL_H

#include <QMap>
#include <QString>

class SCXMLDataItem
{
public:
    SCXMLDataItem(QString id, QString src, QString expr) :
        mId(id), mSrc(src), mExpr(expr)
    {}

public:
    QString GetId() { return mId; }
    QString GetSrc() { return mSrc; }
    QString GetExpr() { return mExpr; }

private:
    QString mId;
    QString mSrc;
    QString mExpr;
};

class SCXMLDataModel
{
public:
    SCXMLDataModel();

    void AddDataItem(SCXMLDataItem *item);
    SCXMLDataItem* GetDataItem(const QString id);

    bool HasItems() { return mDataItems.count() > 0; }

    QList<SCXMLDataItem*> GetDataItemList();

private:
    QMap<QString, SCXMLDataItem*> mDataItems;
};

#endif // SCXMLDATAMODEL_H
