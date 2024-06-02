#include "tinyxml2/tinyxml2.h"
#include "project.h"
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

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

std::vector<Node_data_t> Project::getSubNodeList(int idx_node)
{
    // determine note list, which includes child elements, if any
    std::vector<Node_data_t> node_mv_list;
    for (std::vector<Node_data_t>::iterator it=m_project_data.node_data.begin() + idx_node;
         it != m_project_data.node_data.end(); ++it)
    {
        if (node_mv_list.size() >= 1){
            if (it->level > node_mv_list[0].level){
                node_mv_list.push_back(*it);
            } else {
                break;
            }
        } else {
            node_mv_list.push_back(*it);
        }
    }
    return node_mv_list;
}


bool Project::followNode(int uid_select, int uid_target, bool b_asChild)
{
    if (!doesUIDExist(uid_target)){
        return false;
    }

    int idx_select, idx_target, idx_select_orig, idx_target_orig;
    if(!getVecIndex(uid_select, idx_select_orig) || !getVecIndex(uid_target, idx_target_orig)){
        return false;
    }

    // uids shall not follow each other, if they are on the same branch
    std::vector<Node_data_t> node_mv_list = getSubNodeList(idx_select_orig);
    std::vector<int> uid_mv_list;
    for (auto & it : node_mv_list){
        uid_mv_list.push_back(it.uid);
    }
    if (std::count(uid_mv_list.begin(), uid_mv_list.end(), uid_target)){    // parent -> child
        return false;
    }
    std::vector<Node_data_t> node_tag_list = getSubNodeList(idx_target_orig);
    std::vector<int> uid_tag_list;
    for (auto & it : node_tag_list){
        uid_tag_list.push_back(it.uid);
    }
    if (std::count(uid_tag_list.begin(), uid_tag_list.end(), uid_select)){  // child -> parent
        return false;
    }

    // handle the target children, if any
    int child_count = 0;
    for (std::vector<Node_data_t>::iterator it=m_project_data.node_data.begin()+idx_target_orig+1;
         it != m_project_data.node_data.end(); ++it)
    {
        if(it->level > m_project_data.node_data[idx_target_orig].level){
            child_count += 1;
        } else {
            break;
        }
    }

    // adjust the level info to the target node
    int diff_level = m_project_data.node_data[idx_target_orig].level -
                     m_project_data.node_data[idx_select_orig].level;

    if (b_asChild){
        diff_level += 1;
    }
    for (int i=0; i<node_mv_list.size(); i++){
        m_project_data.node_data[idx_select_orig+i].level += diff_level;
    }
    updateMaxLevel();

    // move note list
    bool b_leftRot = (idx_target_orig >= idx_select_orig) ? true : false;
    if(b_leftRot)
    {
        idx_select = idx_select_orig;
        idx_target = idx_target_orig;
        if (idx_target == m_project_data.node_data.size()-1){   // the last element as target
            std::rotate(m_project_data.node_data.begin()+idx_select,
                        m_project_data.node_data.begin()+idx_select+node_mv_list.size(),
                        m_project_data.node_data.end());
        } else {
            std::rotate(m_project_data.node_data.begin()+idx_select,
                        m_project_data.node_data.begin()+idx_select+node_mv_list.size(),
                        m_project_data.node_data.begin()+idx_target+child_count+1); // itr_last + 1 != vec.end()
        }
    }
    else
    {
        idx_select = m_project_data.node_data.size() - idx_select_orig - node_mv_list.size();
        idx_target = m_project_data.node_data.size() - idx_target_orig - 1;
        std::rotate(m_project_data.node_data.rbegin()+idx_select,
                    m_project_data.node_data.rbegin()+idx_select+node_mv_list.size(),
                    m_project_data.node_data.rbegin()+idx_target-child_count);
    }
    return true;
}


bool Project::getVecIndex(int uid, int & idx_got)
{
    int idx = 0;
    for (auto & it : m_project_data.node_data){
        if(it.uid == uid){
            idx_got = idx;
            return true;
        } else {
            idx++;
        }
    }
    return false;
}


bool Project::doesUIDExist(int uid){
    std::vector<int> vec_uid;
    for (auto & it : m_project_data.node_data)
    {
        vec_uid.push_back(it.uid);
    }
    if (std::count(vec_uid.begin(), vec_uid.end(), uid)){
        return true;
    }
    return false;
}


void Project::updateMaxLevel()
{
    std::vector<int> vec_maxLevel;
    for (auto & it : m_project_data.node_data)
    {
        vec_maxLevel.push_back(it.level);
    }
    m_project_data.max_level = *std::max_element(vec_maxLevel.begin(), vec_maxLevel.end());
}


Project_data_t * Project::data()
{
    return & m_project_data;
}
