#include <gtest/gtest.h>
#include <QDomDocument>
//#include "scxmlstate.h"

const QString noRootNodes = "<notscxml></notscxml>";
const QString oneRootNode = "<scxml></scxml>";

TEST(SCXMLParserTests, DeserialiseDetectsSingleSCXMLElement) {
    //SCXMLParser* parser = new SCXMLParser();
//    QDomDocument doc;
//    doc.setContent(noRootNodes);
//    EXPECT_EQ(false, parser->Deserialise(doc));
//    doc.setContent(oneRootNode);
//    EXPECT_EQ(true, parser->Deserialise(doc));
//    EXPECT_EQ(oneRootNode, parser->GetRawSCXML());
}
