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


class Project
{
public:
    Project();
    ~Project();

    void openProject(const char *);
    // bool updateProject();

    Project_data_t * data();


private:
    Project_data_t m_project_data;

};
