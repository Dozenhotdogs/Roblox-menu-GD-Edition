cmake_minimum_required(VERSION 3.25)

project(RobloxUIMod)

find_package(Geode REQUIRED)

add_geode_mod(
  dozenhotdogs.roblox_ui_mod
    VERSION 1.0.0
    CONTAINS_RESOURCES
    SOURCES 
        src/main.cpp
)
