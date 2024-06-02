#include <gtest/gtest.h>
#include <tinyxml2/tinyxml2.h>
#include <iostream>
#include <fstream>
#include <string>
#include "project.h"

using namespace tinyxml2;

bool compare_txt_files(const char* file1, const char* file2);


TEST(ProjectTest, LoadSave) {
    std::unique_ptr<Project> testProj (new Project);
    const char * path_openPj = "../../../tests/testdata/qooltest_project.qlpj";
    const char * path_savePj = "./test_save_project.qlpj";
    testProj->openProject(path_openPj);
    testProj->saveProject(path_savePj);

    EXPECT_EQ(compare_txt_files(path_openPj, path_savePj), true);
}


TEST(ProjectTest, FollowNote) {
    const char * path_openPj = "../../../tests/testdata/qooltest_project.qlpj";

    std::unique_ptr<Project> testProj1 (new Project);
    testProj1->openProject(path_openPj);
    testProj1->followNode(84, 12, false);
    EXPECT_EQ(testProj1->data()->node_data[2].uid, 84);
    EXPECT_EQ(testProj1->data()->node_data[2].level, 2);

    std::unique_ptr<Project> testProj2 (new Project);
    testProj2->openProject(path_openPj);
    testProj2->followNode(45, 53, true);
    EXPECT_EQ(testProj2->data()->node_data.back().uid, 3);
    EXPECT_EQ(testProj2->data()->node_data.back().level, 3);

    std::unique_ptr<Project> testProj3 (new Project);
    testProj3->openProject(path_openPj);
    testProj3->followNode(368, 45, true);
    EXPECT_EQ(testProj3->data()->node_data[1].level, 2);
    EXPECT_EQ(testProj3->data()->node_data[2].uid, 12);

    std::unique_ptr<Project> testProj4 (new Project);
    testProj4->openProject(path_openPj);
    testProj4->followNode(53, 45, true);
    EXPECT_EQ(testProj4->data()->node_data[1].level, 2);
    EXPECT_EQ(testProj4->data()->node_data[1].uid, 53);

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
