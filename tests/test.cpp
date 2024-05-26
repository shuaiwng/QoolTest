#include <gtest/gtest.h>
#include <tinyxml2/tinyxml2.h>

using namespace tinyxml2;

bool read_project();


TEST(ProjectTest, DataHandling) {
    EXPECT_EQ(read_project(), true);
}


bool read_project()
{
    XMLDocument xml_doc;

    XMLError eResult = xml_doc.LoadFile("../../../tests/testdata/qooltest_project.qlpj");
    if (eResult != tinyxml2::XML_SUCCESS) return false;

    tinyxml2::XMLNode* root = xml_doc.FirstChild();
    if (root == nullptr) return false;

    tinyxml2::XMLElement* element = root->FirstChildElement("General");
    if (element == nullptr) return false;//Fails here

    return true;
}
