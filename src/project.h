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

class Project
{
public:
    Project();
    ~Project();

    std::vector<Node_data_t> openProject();
    // bool closeProject();
    // bool updateProject();

    int getMaxLevel();


private:
    std::string m_version;
    int m_max_level;

};
