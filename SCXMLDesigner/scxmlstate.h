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

signals:

public slots:

private:
  QString mId;
};

#endif // SCXMLSTATE_H
