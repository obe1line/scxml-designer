#ifndef SCXMLEXECUTABLECONTENT_H
#define SCXMLEXECUTABLECONTENT_H

#include <QDebug>
#include <QList>
#include <QDomNode>
#include <QDomElement>
#include "xmlutilities.h"

class SCXMLExecutableActionBase : public QObject
{
    Q_OBJECT

public:
    SCXMLExecutableActionBase();

    virtual void ToXmlElement(QDomDocument &doc, QDomElement containerElement) = 0;
    virtual void Execute() = 0;

signals:
    void LogToOutput(QString msg);
};

class SCXMLIf : public SCXMLExecutableActionBase
{
    Q_OBJECT

    // if, ifelse, else
public:
    SCXMLIf() {}
};

///!
//! \brief The SCXMLLog class
//! \example
//! <log label='test' expr='21' />
class SCXMLLog : public SCXMLExecutableActionBase
{
    Q_OBJECT

public:
    SCXMLLog(QString label, QString expr) : mLabel(label), mExpr(expr) {}

    static SCXMLLog* FromXmlElement(QDomElement* element) {
        if (element->tagName() != XMLUtilities::SCXML_TAG_LOG) return nullptr;

        //label - optional
        //expr - optional
        QString label = XMLUtilities::GetAttributeOrDefault(element, XMLUtilities::SCXML_TAG_LABEL, "");
        QString expr = XMLUtilities::GetAttributeOrDefault(element, XMLUtilities::SCXML_TAG_EXPR, "");
        return new SCXMLLog(label, expr);
    }

    virtual void ToXmlElement(QDomDocument &doc, QDomElement containerElement) final
    {
        QDomElement elem = doc.createElement(XMLUtilities::SCXML_TAG_LOG);
        if (mLabel != "") elem.setAttribute(XMLUtilities::SCXML_TAG_LABEL, mLabel);
        if (mExpr != "") elem.setAttribute(XMLUtilities::SCXML_TAG_EXPR, mExpr);
        containerElement.appendChild(elem);
    }

    void Execute() {
        emit LogToOutput(QString("Log expr:%1 label:%2").arg(mExpr).arg(mLabel));
    }


private:
    QString mLabel;
    QString mExpr;
};

class SCXMLExecutableContent : public SCXMLExecutableActionBase
{
    Q_OBJECT

public:
    SCXMLExecutableContent();

    static SCXMLExecutableContent* FromXmlElement(QDomNodeList content);
    virtual void ToXmlElement(QDomDocument &doc, QDomElement containerElement) final;

    void AddAction(SCXMLExecutableActionBase* action) {
        if (action != nullptr) {
            mActions.append(action);
            connect(action, SIGNAL(LogToOutput(QString)), this, SLOT(WriteToOutput(QString)));
        }
    }

    void Execute() {
        foreach (SCXMLExecutableActionBase* action, mActions) {
            action->Execute();
        }
    }

public slots:
    void WriteToOutput(QString msg) {
        // forward to the parent
        emit LogToOutput(msg);
    }

private:
    QList<SCXMLExecutableActionBase*> mActions;
};

#endif // SCXMLEXECUTABLECONTENT_H
