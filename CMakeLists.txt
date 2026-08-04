cmake_minimum_required(VERSION 3.25)

project(MyMod VERSION 1.0.0)

find_package(Geode REQUIRED)

add_geode_mod(MyMod
    SOURCES
        src/main.cpp
)
