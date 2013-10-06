#include <QMap>
#include "scxmlstate.h"

SCXMLState::SCXMLState(QString id) :
    QState(), mId(id), mX(0), mY(0), mW(100), mH(50)
{
}

void SCXMLState::ApplyMetaData(QMap<QString, QString> &mapMetaData)
{
    foreach(QString key, mapMetaData.keys()) {
        QString value = mapMetaData.value(key);
        if (key == "x") {
            SetXPosition(value.toInt());
            continue;
        }
        if (key == "y") {
            SetYPosition(value.toInt());
            continue;
        }
        if (key == "height") {
            SetHeight(value.toInt());
            continue;
        }
        if (key == "width") {
            SetWidth(value.toInt());
            continue;
        }
    }
}

QString SCXMLState::GetMetaDataString()
{
    QString metadata = QString(" META-DATA [x=%1] [y=%2] [width=%3] [height=%4]").arg(
                GetXPosition()).arg(GetYPosition()).arg(GetWidth()).arg(GetHeight());
    return metadata;
}
