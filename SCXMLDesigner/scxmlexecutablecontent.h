#ifndef SCXMLEXECUTABLECONTENT_H
#define SCXMLEXECUTABLECONTENT_H

#include <QDebug>
#include <QList>
#include <QDomNode>
#include <QDomElement>
#include "xmlutilities.h"

class SCXMLExecutableActionBase
{
public:
    SCXMLExecutableActionBase();

    virtual void Execute() = 0;
};

class SCXMLIf : public SCXMLExecutableActionBase
{
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

    void Execute() {
        qDebug() << mExpr;
    }

private:
    QString mLabel;
    QString mExpr;
};

class SCXMLExecutableContent
{
public:
    SCXMLExecutableContent();

    void AddAction(SCXMLExecutableActionBase* action) {
        if (action != nullptr) {
            mActions.append(action);
        }
    }

    static SCXMLExecutableContent* FromXmlElement(QDomNodeList content) {
        SCXMLExecutableContent* newContent = new SCXMLExecutableContent();
        for (int elementPos=0; elementPos<content.length(); elementPos++) {
            QDomNode node = content.at(elementPos);
            QDomElement element = node.toElement();
            QString tag = element.tagName();
            //TODO: use an array of tag names and function pointers
            if (tag == XMLUtilities::SCXML_TAG_LOG) {
                newContent->AddAction(SCXMLLog::FromXmlElement(&element));
                continue;
            }
            if (tag == XMLUtilities::SCXML_TAG_RAISE) {
                //newContent->AddAction(SCXMLRaise::FromXmlElement(element));
                continue;
            }
            if (tag == XMLUtilities::SCXML_TAG_SEND) {
                //newContent->AddAction(SCXMLSend::FromXmlElement(element));
                continue;
            }
            if (tag == XMLUtilities::SCXML_TAG_SCRIPT) {
                //newContent->AddAction(SCXMLScript::FromXmlElement(element));
                continue;
            }
            if (tag == XMLUtilities::SCXML_TAG_ASSIGN) {
                //newContent->AddAction(SCXMLAssign::FromXmlElement(element));
                continue;
            }
            if (tag == XMLUtilities::SCXML_TAG_CANCEL) {
                //newContent->AddAction(SCXMLCancel::FromXmlElement(element));
                continue;
            }
            if (tag == XMLUtilities::SCXML_TAG_IF) {
                //newContent->AddAction(SCXMLIf::FromXmlElement(element));
                continue;
            }
            if (tag == XMLUtilities::SCXML_TAG_FOREACH) {
                //newContent->AddAction(SCXMLForEach::FromXmlElement(element));
                continue;
            }
        }

        return newContent;
    }

    void Execute() {
        foreach (SCXMLExecutableActionBase* action, mActions) {
            action->Execute();
        }
    }
private:
    QList<SCXMLExecutableActionBase*> mActions;
};

#endif // SCXMLEXECUTABLECONTENT_H
