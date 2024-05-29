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


std::vector<Node_data_t> Project::openProject()
{

    XMLDocument xml_doc;
    xml_doc.LoadFile("../../tests/testdata/qooltest_project.qlpj");
    XMLElement* dataElm = xml_doc.FirstChildElement("Root");

    m_max_level = 0;
    std::vector<Node_data_t> data_vec;
    for (XMLElement* nodeElm = dataElm->FirstChildElement("Node");
         nodeElm != NULL; nodeElm = nodeElm->NextSiblingElement())
    {
        Node_data_t pNodeData;
        XMLElement* infoElm = nodeElm->FirstChildElement("Info");
        pNodeData.name = infoElm->Attribute("Name");
        pNodeData.level = std::stoi(std::string(infoElm->Attribute("Level")));
        if (std::string(infoElm->Attribute("isTestCase")) == "YES"){
            pNodeData.isTestCase = true;
        } else {
            pNodeData.isTestCase = false;
        }
        pNodeData.testType = infoElm->Attribute("TestType");
        pNodeData.comment = infoElm->Attribute("Comment");
        pNodeData.uid = std::stoi(std::string(infoElm->Attribute("UID")));
        pNodeData.full_name = "UID-" + std::to_string(pNodeData.uid) + ": " + pNodeData.name;

        XMLElement* testcase = nodeElm->FirstChildElement("TestCase");
        if(testcase){
            for (XMLElement* step = testcase->FirstChildElement("StepIn");
                 step != NULL; step = step->NextSiblingElement("StepIn"))
            {
                pNodeData.testdata.push_back(std::make_pair(step->GetText(),
                                                            step->NextSiblingElement()->GetText()));
            }
        }

        data_vec.push_back(pNodeData);

        // find max level
        if (m_max_level < pNodeData.level){
            m_max_level = pNodeData.level;
        }
    }

    return data_vec;
}


int Project::getMaxLevel()
{
    return m_max_level;
}

