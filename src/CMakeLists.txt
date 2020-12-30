cmake_minimum_required(VERSION 3.7)

file(GLOB SOURCES
    FileProcessor/*.h
    FileProcessor/*.cpp
    ArgumentsParser/*.h
    ArgumentsParser/*.cpp
)

# add the executable
add_executable(2GIS_Test main.cpp ${SOURCES})

# target_link_libraries(2GIS_Test include/cxxopts)
# target_link_libraries(2GIS_Test include/CRC)