cmake_minimum_required (VERSION 3.0)

project(talamel)

# Compile taglib
include(ExternalProject)

set(TAGLIB_DIR "${CMAKE_BINARY_DIR}/taglib")
set(TAGLIB_INSTALL "${TAGLIB_DIR}/install")

ExternalProject_Add(
    taglib 
    GIT_REPOSITORY https://www.github.com/taglib/taglib
    PREFIX ${TAGLIB_DIR} 
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${TAGLIB_INSTALL} -DBUILD_SHARED_LIBS=OFF -DENABLE_STATIC_RUNTIME=ON -DCMAKE_C_FLAGS=-fPIC -Wall -O3 -DCMAKE_CXX_FLAGS=-fPIC -Wall -03
)

include_directories(${TAGLIB_INSTALL}/include)
link_directories(${TAGLIB_INSTALL}/lib)

# Decide on C/C++ standards
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add the talamel library

add_library(talamel STATIC
    src/talamel.cpp 
)
target_include_directories(talamel
    PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
)
add_dependencies(talamel taglib)
target_link_libraries(talamel tag)
set_target_properties(talamel PROPERTIES PUBLIC_HEADER "${talamel_headers}")


install(TARGETS talamel 
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_PREFIX}
    LIBRARY  DESTINATION ${CMAKE_INSTALL_PREFIX}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_PREFIX})  # This is for Windows
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_PREFIX})

# Add an example app depending on talamel
add_executable(Readmeta apps/readmeta.c)
add_dependencies(Readmeta talamel)
target_link_libraries(Readmeta talamel) 
