#include <gtest/gtest.h>
#include <tinyxml2/tinyxml2.h>
#include <iostream>
#include <fstream>
#include <string>
#include "project.h"

using namespace tinyxml2;

bool compare_txt_files(const char* file1, const char* file2);


TEST(LoadSave, ProjectTest) {
    std::unique_ptr<Project> testProj (new Project);
    const char * path_openPj = "../../../tests/testdata/qooltest_project.qlpj";
    const char * path_savePj = "./test_save_project.qlpj";
    testProj->openProject(path_openPj);
    testProj->saveProject(path_savePj);

    EXPECT_EQ(compare_txt_files(path_openPj, path_savePj), true);
}


bool compare_txt_files(const char* file1, const char* file2)
{
    std::fstream newFile1;
    std::fstream newFile2;
    newFile1.open(file1, std::ios::in);
    newFile2.open(file2, std::ios::in);

    std::string line_file1, line_file2;
    bool isLineFile1, isLineFile2;
    while(true)
    {
        if(std::getline(newFile1, line_file1)) {
            isLineFile1 = true;
        } else {
            isLineFile1 = false;
        }
        if(std::getline(newFile2, line_file2)) {
            isLineFile2 = true;
        } else {
            isLineFile2 = false;
        }

        if(!isLineFile1 && !isLineFile2){
            break;
        }
        if(isLineFile1 != isLineFile2) return false;
        if(line_file1 != line_file2) return false;
    }
    return true;
}
