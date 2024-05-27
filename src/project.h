#pragma once

#include <iostream>
#include <vector>


struct Node_info_t{
    int uid;
    std::string name;
    int level;
    bool isTestCase;
    std::string testType;
    std::string comment;
};

class Project
{
public:
    Project();
    ~Project();

    std::vector<Node_info_t> openProject();
    // bool closeProject();
    // bool updateProject();

    int getMaxLevel();


private:
    std::string m_version;
    int m_max_level;

};
