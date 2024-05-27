#include "tinyxml2/tinyxml2.h"
#include "project.h"
#include <iostream>
#include <vector>
#include <memory>

using namespace tinyxml2;


Project::Project()
{

}
Project::~Project()
{

}


std::vector<Node_info_t> Project::openProject()
{

    XMLDocument xml_doc;
    xml_doc.LoadFile("../../tests/testdata/qooltest_project.qlpj");
    XMLElement* dataElm = xml_doc.FirstChildElement("Root");

    m_max_level = 0;
    std::vector<Node_info_t> info_vec;
    for (XMLElement* nodeElm = dataElm->FirstChildElement("Node");
         nodeElm != NULL; nodeElm = nodeElm->NextSiblingElement())
    {
        Node_info_t pNodeInfo;
        XMLElement* infoElm = nodeElm->FirstChildElement("Info");
        pNodeInfo.name = infoElm->Attribute("Name");
        pNodeInfo.level = std::stoi(std::string(infoElm->Attribute("Level")));
        if (std::string(infoElm->Attribute("isTestCase")) == "YES"){
            pNodeInfo.isTestCase = true;
        } else {
            pNodeInfo.isTestCase = false;
        }
        pNodeInfo.testType = infoElm->Attribute("TestType");
        pNodeInfo.comment = infoElm->Attribute("Comment");
        pNodeInfo.uid = std::stoi(std::string(infoElm->Attribute("UID")));

        info_vec.push_back(pNodeInfo);

        // find max level
        if (m_max_level < pNodeInfo.level){
            m_max_level = pNodeInfo.level;
        }
    }

    return info_vec;
}


int Project::getMaxLevel()
{
    return m_max_level;
}

