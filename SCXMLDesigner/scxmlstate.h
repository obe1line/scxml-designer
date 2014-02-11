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
    explicit SCXMLState(QString id, QMap<QString,QString> *metaData);

    //! Retrieves the identifier of the state as defined in the SCXML file
    QString GetId() { return mId; }
    qreal GetShapeX() { return x(); }
    qreal GetShapeY() { return y(); }
    qreal GetShapeWidth() { return mWidth; }
    qreal GetShapeHeight() { return mHeight; }
    QString GetDescription() { return mDescription; }
    bool GetFinal() { return mFinal; }
    QPainterPath GetNodeOutlinePath();
    QString GetOnEntry() { return mOnEntry; }
    QString GetOnExit() { return mOnExit; }

    //! gets the connection point for the given index (1=end point of perimeter of state, 0=start point)
    QPoint GetConnectionPoint(qreal connectionPointIndex);
    qreal GetConnectionPointIndex(QPoint point);

    void SetShapeX(qreal value) { setX(value); sizeChanged(); }
    void SetShapeY(qreal value) { setY(value); sizeChanged(); }
    void SetShapeWidth(qreal value) { mWidth = value; sizeChanged(); }
    void SetShapeHeight(qreal value) { mHeight = value; sizeChanged(); }
    void SetDescription(QString value) { mDescription = value; }
    void SetFinal(bool value) { mFinal = value; }
    void SetOnEntry(QString value) { mOnEntry = value; }
    void SetOnExit(QString value) { mOnExit = value; }

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    // MetaDataSupport overrides
    void ApplyMetaData(QMap<QString, QString>* mapMetaData);
    QString GetMetaDataString();

    // QGraphicsItem overrides
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void UpdateTransitions();

    void AddIncomingTransition(QAbstractTransition* transitionRef) { mIncomingTransitions.append(transitionRef); }

signals:
    void sizeChanged();

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
  bool mFinal;
  QList<QAbstractTransition*> mIncomingTransitions;
  QString mOnEntry;
  QString mOnExit;
};

#endif // SCXMLSTATE_H
