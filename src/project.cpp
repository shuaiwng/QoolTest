#include "tinyxml2/tinyxml2.h"
#include "project.h"
#include <iostream>
#include <vector>>

using namespace tinyxml2;


Project::Project()
{

}
Project::~Project()
{

}


bool Project::openProject()
{
    // tinyxml test
    XMLDocument xmlDoc;
    XMLNode * pRoot = xmlDoc.NewElement("Root");
    xmlDoc.InsertFirstChild(pRoot);
    XMLElement * pGeneral = xmlDoc.NewElement("General");
    XMLElement * pVersion = xmlDoc.NewElement("Version");
    pVersion->SetText("1.0.0");
    pGeneral->InsertEndChild(pVersion);
    pRoot->InsertEndChild(pGeneral);

    XMLElement * pTest_1 = xmlDoc.NewElement("UID-1");
    XMLElement * pInfo = xmlDoc.NewElement("Info");
    pInfo->SetAttribute("Name", "Test Installation");
    pInfo->SetAttribute("Level", "1");
    pInfo->SetAttribute("isTestCase", "YES");
    pInfo->SetAttribute("TestType", "Smoke");
    pInfo->SetAttribute("Comment", "Deivce is already connected.");
    pTest_1->InsertEndChild(pInfo);

    XMLElement * pTestCase = xmlDoc.NewElement("TestCase");
    XMLElement * pStepIn = xmlDoc.NewElement("StepIn");
    pStepIn->SetText("Press the start button.");
    XMLElement * pStepOut = xmlDoc.NewElement("StepOut");
    pStepOut->SetText("Live View is working.");
    pTestCase->InsertEndChild(pStepIn);
    pTestCase->InsertEndChild(pStepOut);
    pTest_1->InsertEndChild(pTestCase);

    pRoot->InsertEndChild(pTest_1);

    xmlDoc.SaveFile("SavedData.xml");


    return true;
}

