#include "scxmldatamodel.h"

SCXMLDataModel::SCXMLDataModel()
{
}

void SCXMLDataModel::AddDataItem(SCXMLDataItem* item)
{
    mDataItems.insert(item->GetId(), item);
}

SCXMLDataItem* SCXMLDataModel::GetDataItem(const QString id)
{
    if (mDataItems.contains(id)) return mDataItems[id];
    return nullptr;
}

QList<SCXMLDataItem*> SCXMLDataModel::GetDataItemList()
{
    return mDataItems.values();
}
