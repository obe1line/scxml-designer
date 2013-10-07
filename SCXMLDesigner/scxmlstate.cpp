#include <QMap>
#include <QCursor>
#include "scxmlstate.h"

SCXMLState::SCXMLState(QString id) :
    QState(), mId(id), mWidth(0), mHeight(0)
{
    setX(0);
    setY(0);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setCursor(Qt::OpenHandCursor);
}

void SCXMLState::ApplyMetaData(QMap<QString, QString> &mapMetaData)
{
    foreach(QString key, mapMetaData.keys()) {
        QString value = mapMetaData.value(key);
        if (key == "x") {
            SetShapeX(value.toDouble());
            continue;
        }
        if (key == "y") {
            SetShapeY(value.toDouble());
            continue;
        }
        if (key == "height") {
            SetShapeHeight(value.toDouble());
            continue;
        }
        if (key == "width") {
            SetShapeWidth(value.toDouble());
            continue;
        }
    }
}

QRectF SCXMLState::boundingRect() const
{
    qreal penWidth = 1;
//    return QRectF(-10 - (penWidth / 2), -10 - (penWidth / 2), mWidth + penWidth, mHeight + penWidth);
    return QRectF(0 - penWidth/2, 0 - penWidth/2,
                  mWidth + penWidth, mHeight + penWidth);
}

void SCXMLState::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    QRect rect = QRect(0, 0, mWidth, mHeight);

    QLinearGradient gradient(rect.x(), rect.y(), rect.x()+rect.width(), rect.y()+rect.height());
    gradient.setColorAt(0, QColor::fromRgb(0xD3, 0xDE, 0x92));  //D3DE92
    gradient.setColorAt(1, QColor::fromRgb(0xE6, 0xF2, 0xA2));  //E6F2A2
    QBrush stateBrush = QBrush(gradient);
    painter->setBrush(stateBrush);
    QBrush blackBrush = QBrush(Qt::black);
    QPen statePen = QPen(blackBrush, 2, Qt::SolidLine);
    painter->setPen(statePen);

    painter->drawRoundedRect(rect, 10.0, 10.0);
}

QString SCXMLState::GetMetaDataString()
{
    QString metadata = QString(" META-DATA [x=%1] [y=%2] [width=%3] [height=%4]").arg(
                GetShapeX()).arg(GetShapeY()).arg(GetShapeWidth()).arg(GetShapeHeight());
    return metadata;
}
