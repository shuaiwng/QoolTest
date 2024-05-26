#pragma once

#include <iostream>


class Project
{
public:
    Project();
    ~Project();

    bool openProject();
    // bool closeProject();
    // bool updateProject();


private:
    std::string m_version;

};
