#ifndef SCXMLSTATE_H
#define SCXMLSTATE_H

#include <QState>
#include <QGraphicsItem>
#include <QPainter>
#include "metadatasupport.h"

//! Represents an SCXML state
//!
//! Implemented with QState as the underlying class. Additional attributes and meta data are
//! added to this for import and export to external SCXML files.
class SCXMLState : public QState, public QGraphicsItem, public MetaDataSupport
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    explicit SCXMLState(QString id);

    //! Retrieves the identifier of the state as defined in the SCXML file
    QString GetId() { return mId; }
    qreal GetShapeX() { return x(); }
    qreal GetShapeY() { return y(); }
    qreal GetShapeWidth() { return mWidth; }
    qreal GetShapeHeight() { return mHeight; }
    QString GetDescription() { return mDescription; }

    void SetShapeX(qreal value) { setX(value); }
    void SetShapeY(qreal value) { setY(value); }
    void SetShapeWidth(qreal value) { mWidth = value; }
    void SetShapeHeight(qreal value) { mHeight = value; }
    void SetDescription(QString value) { mDescription = value; }

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    // MetaDataSupport overrides
    void ApplyMetaData(QMap<QString, QString> &mapMetaData);
    QString GetMetaDataString();

    // QGraphicsItem overrides
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
signals:

public slots:

private:
  QString mId;
  QString mDescription;
  qreal mWidth;
  qreal mHeight;
  bool mResizing;
  qreal mResizeOriginalWidth;
  qreal mResizeOriginalHeight;
  qreal mResizeStartX;
  qreal mResizeStartY;
};

#endif // SCXMLSTATE_H
