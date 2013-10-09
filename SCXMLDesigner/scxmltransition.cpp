#include "scxmltransition.h"

SCXMLTransition::SCXMLTransition(SCXMLState *parent) :
    QAbstractTransition(), mParentState(parent), mX1(0), mX2(0), mY1(0), mY2(0), mDescription("")
{
}

void SCXMLTransition::ApplyMetaData(QMap<QString, QString> &mapMetaData)
{
    foreach(QString key, mapMetaData.keys()) {
        QString value = mapMetaData.value(key);
        if (key == "description") {
            SetDescription(value);
            continue;
        }
        if (key == "x1") {
            SetX1(value.toDouble());
            continue;
        }
        if (key == "y1") {
            SetY1(value.toDouble());
            continue;
        }
        if (key == "x2") {
            SetX2(value.toDouble());
            continue;
        }
        if (key == "y2") {
            SetY2(value.toDouble());
            continue;
        }
    }
}

QString SCXMLTransition::GetMetaDataString()
{
    QString metadata = QString(" META-DATA [x1=%1] [y1=%2] [x1=%3] [x2=%4] [description=%5]").arg(
                GetX1()).arg(GetY1()).arg(GetX2()).arg(GetY2()).arg(GetDescription());
    return metadata;
}

QRectF SCXMLTransition::boundingRect() const
{
    qreal penWidth = 1;
    return QRectF(0 - penWidth/2, 0 - penWidth/2,
                  mX2-mX1 + penWidth, mY2-mY1 + penWidth);
}

void SCXMLTransition::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    QBrush blackBrush = QBrush(isSelected() ? Qt::blue : Qt::black);
    QPen transitionPen = QPen(blackBrush, 2, Qt::SolidLine);
    painter->setPen(transitionPen);

    QPoint pointStart = QPoint(mX1, mY1);
    QPoint pointEnd = QPoint(mX2, mY2);
    painter->drawLine(pointStart, pointEnd);
}
