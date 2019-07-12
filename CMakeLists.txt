cmake_minimum_required(VERSION 2.8.9)
project(projectPSniffer)

include(FindPCAP.cmake)
#Bring the headers, such as hash.h etc. into the project
include_directories(include)

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.c")
#LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/usr/include/)
#TARGET_LINK_LIBRARIES(GLBall pcap)


add_executable(projectPSniffer ${SOURCES})
target_link_libraries(projectPSniffer ${PCAP_LIBRARY})
