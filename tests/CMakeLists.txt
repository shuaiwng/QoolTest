cmake_minimum_required(VERSION 3.16)
project(test)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(/usr/include)

set(SOURCE_FILES test.cpp)
set(TINYXML_FILE ../thirdparty/tinyxml2/tinyxml2.cpp)


add_executable(test ${SOURCE_FILES} ${TINYXML_FILE})


link_directories(/usr/lib64)
target_link_libraries(test -lgtest -lgtest_main)

