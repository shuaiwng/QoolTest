#pragma once

#include <iostream>
#include <vector>
#include <tuple>


struct Node_data_t{
    int uid;
    std::string name;
    std::string full_name;
    int level;
    bool isTestCase;
    std::string testType;
    std::string comment;
    std::vector<std::tuple<std::string, std::string>> testdata;
};

struct Project_data_t{
    std::vector<Node_data_t> node_data;
    int max_level;
};

enum class NodeType {eNodeSubFolder, eNodeTestCase};

class Project
{
public:
    Project();
    ~Project();

    bool openProject(const char *);
    void saveProject(const char *);

    bool followNode(int uid_select, int uid_target, bool b_asChild);
    bool addNode(int uid_target, NodeType eNodeType, bool b_asChild);
    bool deleteNode(int uid_target);
    bool findNextAvailableUID(int & uid_find);
    bool compareSubMainEqual(int uid_select, bool & b_equal);

    std::vector<Node_data_t> getSubNodeList(int idx_node);
    bool getVecIndex(int uid, int & idx_got);
    void updateMaxLevel();
    bool doesUIDExist(int uid);

    Project_data_t * data();


private:
    Project_data_t m_project_data;

};
