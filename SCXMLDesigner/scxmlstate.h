#ifndef SCXMLSTATE_H
#define SCXMLSTATE_H

#include <QState>

//! Represents an SCXML state
//!
//! Implemented with QState as the underlying class. Additional attributes and meta data are
//! added to this for import and export to external SCXML files.
class SCXMLState : public QState
{
    Q_OBJECT
public:
    explicit SCXMLState(QString id);

    //! Retrieves the identifier of the state as defined in the SCXML file
    QString GetId() { return mId; }
    int GetXPosition() { return mX; }
    int GetYPosition() { return mY; }
    int GetWidth() { return mW; }
    int GetHeight() { return mH; }

    void SetXPosition(int value) { mX = value; }
    void SetYPosition(int value) { mY = value; }
    void SetWidth(int value) { mW = value; }
    void SetHeight(int value) { mH = value; }

    void ApplyMetaData(QMap<QString, QString>& mapMetaData);

    QString GetMetaDataString();
signals:

public slots:

private:
  QString mId;
  int mX;
  int mY;
  int mW;
  int mH;
};

#endif // SCXMLSTATE_H
