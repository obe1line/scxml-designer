#include <QMap>
#include <QCursor>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include "scxmlstate.h"
#include "scxmltransition.h"

#define MIN_STATE_HEIGHT 30
#define MIN_STATE_WIDTH 60

SCXMLState::SCXMLState(QString id) :
    QState(), mId(id), mDescription(""),
    mWidth(0), mHeight(0),
    mResizing(false),
    mResizeOriginalWidth(0), mResizeOriginalHeight(0),
    mResizeStartX(0), mResizeStartY(0),
    mFinal(false)
{
    setX(0);
    setY(0);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setCursor(Qt::OpenHandCursor);
    setAcceptHoverEvents(true);
}

void SCXMLState::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (mResizing) {
        prepareGeometryChange();
        qreal newWidth = mResizeOriginalWidth + (event->pos().x() - mResizeStartX);
        qreal newHeight = mResizeOriginalHeight + (event->pos().y() - mResizeStartY);
        if (newWidth >= MIN_STATE_WIDTH) SetShapeWidth(newWidth);
        if (newHeight >= MIN_STATE_HEIGHT) SetShapeHeight(newHeight);

        UpdateTransitions();
        event->accept();
        return;
    }

    QGraphicsItem::mouseMoveEvent(event);
    UpdateTransitions();
}

void SCXMLState::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // alternatively, this could be done with a grabber corner - possible refactor
    if ((event->pos().x() > (mWidth-10)) && (event->pos().y() > (mHeight-10))) {
        mResizing = true;
        mResizeOriginalWidth = mWidth;
        mResizeOriginalHeight = mHeight;
        mResizeStartX = event->pos().x();
        mResizeStartY = event->pos().y();
        event->accept();
    }

    update();

    QGraphicsItem::mousePressEvent(event);
}

void SCXMLState::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    mResizing = false;
    QGraphicsItem::mouseReleaseEvent(event);

    UpdateTransitions();
}

void SCXMLState::ApplyMetaData(QMap<QString, QString>* mapMetaData)
{
    foreach(QString key, mapMetaData->keys()) {
        QString value = mapMetaData->value(key);
        if (key == "description") {
            SetDescription(value);
            continue;
        }
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

QString SCXMLState::GetMetaDataString()
{
    QString metadata = QString(" META-DATA [x=%1] [y=%2] [width=%3] [height=%4] [description=%5]").arg(
                GetShapeX()).arg(GetShapeY()).arg(GetShapeWidth()).arg(GetShapeHeight()).arg(GetDescription());
    return metadata;
}

QRectF SCXMLState::boundingRect() const
{
    qreal penWidth = 2;
    return QRectF(0 - penWidth, 0 - penWidth,
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
    QBrush blackBrush = QBrush(isSelected() ? Qt::blue : Qt::black);
    QPen statePen = QPen(blackBrush, 2, Qt::SolidLine);
    painter->setPen(statePen);

    painter->drawRoundedRect(rect, 10.0, 10.0);
}


//!
//! \brief SCXMLState::UpdateTransitions
//!
//! Forces a redraw of all the transitions that are attached to this state
//!
void SCXMLState::UpdateTransitions()
{
    // update outgoing transitions
    foreach(QAbstractTransition* abtran, this->transitions()) {
        SCXMLTransition* tran = dynamic_cast<SCXMLTransition*>(abtran);
        tran->Update();
    }

    // update incoming transitions
    foreach(QAbstractTransition* abtran, this->mIncomingTransitions) {
        SCXMLTransition* tran = dynamic_cast<SCXMLTransition*>(abtran);
        tran->Update();
    }
}
