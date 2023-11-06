cmake_minimum_required(VERSION 3.10)

#Set the project name
project(PPN)

#Enable modern C++
set(CMAKE_CXX_STANDARD 17)

#Add the executable
add_executable(Projet_run main.cpp src/data_loading.cpp src/decision_tree.cpp)






