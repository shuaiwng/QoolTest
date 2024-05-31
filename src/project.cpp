#include "tinyxml2/tinyxml2.h"
#include "project.h"
#include <iostream>
#include <vector>
#include <memory>
#include <QtCore>

using namespace tinyxml2;


Project::Project()
{

}
Project::~Project()
{

}


void Project::openProject(const char * proj_path)
{
    XMLDocument xml_doc;
    xml_doc.LoadFile(proj_path);
    XMLElement* dataElm = xml_doc.FirstChildElement("Root");

    int max_level = 0;
    for (XMLElement* nodeElm = dataElm->FirstChildElement("Node");
         nodeElm != NULL; nodeElm = nodeElm->NextSiblingElement())
    {
        Node_data_t pNodeData;
        XMLElement* infoElm = nodeElm->FirstChildElement("Info");
        pNodeData.name = infoElm->Attribute("Name");
        pNodeData.level = std::stoi(std::string(infoElm->Attribute("Level")));
        if (std::string(infoElm->Attribute("isTestCase")) == "1"){
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
        m_project_data.node_data.push_back(pNodeData);

        // find max level
        if (max_level < pNodeData.level){
            max_level = pNodeData.level;
        }
    }
    m_project_data.max_level = max_level;
}


void Project::saveProject(const char *proj_path)
{
    XMLDocument xmlDoc;
    XMLNode * pRoot = xmlDoc.NewElement("Root");
    xmlDoc.InsertFirstChild(pRoot);

    int node_vec_sz = m_project_data.node_data.size();
    for (int i = 0; i < node_vec_sz; i++)
    {
        XMLElement * pNodeElement = xmlDoc.NewElement("Node");
        XMLElement * pInfoElement = xmlDoc.NewElement("Info");
        pInfoElement->SetAttribute("UID", std::to_string(m_project_data.node_data[i].uid).c_str());
        pInfoElement->SetAttribute("Name", m_project_data.node_data[i].name.c_str());
        pInfoElement->SetAttribute("Level", std::to_string(m_project_data.node_data[i].level).c_str());
        pInfoElement->SetAttribute("isTestCase", std::to_string(int(m_project_data.node_data[i].isTestCase)).c_str());
        pInfoElement->SetAttribute("TestType", m_project_data.node_data[i].testType.c_str());
        pInfoElement->SetAttribute("Comment", m_project_data.node_data[i].comment.c_str());

        pNodeElement->InsertEndChild(pInfoElement);

        if(!m_project_data.node_data[i].isTestCase){
            pRoot->InsertEndChild(pNodeElement);
            continue;
        }

        XMLElement * pTestElm = xmlDoc.NewElement("TestCase");
        int steps_sz = m_project_data.node_data[i].testdata.size();
        for (int i_test = 0; i_test < steps_sz; i_test++)
        {
            XMLElement * pStepInElm = xmlDoc.NewElement("StepIn");
            XMLElement * pStepOutElm = xmlDoc.NewElement("StepOut");
            pStepInElm->SetText(std::get<0>(m_project_data.node_data[i].testdata[i_test]).c_str());
            pStepOutElm->SetText(std::get<1>(m_project_data.node_data[i].testdata[i_test]).c_str());
            pTestElm->InsertEndChild(pStepInElm);
            pTestElm->InsertEndChild(pStepOutElm);
        }
        pNodeElement->InsertEndChild(pTestElm);

        pRoot->InsertEndChild(pNodeElement);
    }
    xmlDoc.SaveFile(proj_path);
}


Project_data_t * Project::data()
{
    return & m_project_data;
}
