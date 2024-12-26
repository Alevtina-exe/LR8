cmake_minimum_required(VERSION 3.5.0)
project(Task_2 VERSION 0.1.0 LANGUAGES C CXX)

add_executable(Task_2 elasticsearch.cpp filework.cpp ivalidation.cpp main.cpp struct_funcs.cpp)

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR}) 
target_link_libraries(Task_2 ${CURL_LIBRARIES})