cmake_minimum_required(VERSION 3.20)

project(lol-tracker LANGUAGES CXX)

set(CMAKE_CXX_COMPILER)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)


add_executable(lol-tracker
    api/RiotAPI.cpp
    main.cpp
)


find_package(CURL REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

target_link_libraries(lol-tracker PRIVATE CURL::libcurl nlohmann_json::nlohmann_json)