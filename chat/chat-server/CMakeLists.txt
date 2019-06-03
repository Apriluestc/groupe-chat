project(server)
cmake_minimum_required(VERSION 2.8)

aux_source_directory(./src SRC_LIST)

find_package(Qt5Sql REQUIRED)

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} Qt5::Sql)
#qt5_use_modules(${PROJECT_NAME} Sql)
set(CMAKE_CXX_FLAGS "-g -pthread")
