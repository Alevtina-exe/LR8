cmake_minimum_required(VERSION 3.5.0)
project(Task_1 VERSION 0.1.0 LANGUAGES C CXX)

add_executable(Task_1 main.cpp functions.cpp)
#add_executable(Task_1 main.cpp ivalidation.cpp struct_funcs.cpp filework.cpp elasticsearch.cpp)

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR}) 
target_link_libraries(Task_1 ${CURL_LIBRARIES})

