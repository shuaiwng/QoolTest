cmake_minimum_required(VERSION 3.16)
project(test)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets UiTools)

if(WIN32)
    include_directories(C:/Home/Development/googletest/googletest/include)
    include_directories(C:/Home/Development/googletest/googlemock/include)
endif (WIN32)

if(UNIX)
    include_directories(/usr/include)
endif (UNIX)


include_directories(../src)

set(SOURCE_FILES
    test.cpp
    ../src/project.h
    ../src/project.cpp
)
set(TINYXML_FILE ../thirdparty/tinyxml2/tinyxml2.cpp)

if(WIN32)
    link_directories(C:/Home/Development/googletest/build/lib)
endif (WIN32)

if(UNIX)
    link_directories(/usr/lib64)
endif (UNIX)


add_executable(test ${SOURCE_FILES} ${TINYXML_FILE})

target_link_libraries(test Qt6::Core -lgtest -lgtest_main)

