project(Source_Lib)

# Get every .cpp file in subdirectories
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Make a lib containing all our .cpp
add_library(Source_Lib  ${SOURCE_FILES})   

# Dont forget to include headers so cpp can compile
target_include_directories(Source_Lib PUBLIC ../include/)