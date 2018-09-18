cmake_minimum_required (VERSION 3.0)

project (TALAMEL)

# Compile taglib
include(ExternalProject)

# set(TALAMEL_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(TAGLIB_DIR "${CMAKE_BINARY_DIR}/taglib")
set(TAGLIB_INSTALL "${TAGLIB_DIR}/install")

ExternalProject_Add(
    taglib 
    GIT_REPOSITORY git@github.com:taglib/taglib.git
    PREFIX ${TAGLIB_DIR} 
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${TAGLIB_INSTALL} -DBUILD_SHARED_LIBS=OFF -DENABLE_STATIC_RUNTIME=ON -DCMAKE_C_FLAGS=-fPIC -Wall -O3
)

include_directories(${TAGLIB_INSTALL}/include)
link_directories(${TAGLIB_INSTALL}/lib)

# Add the talamel library
# set(talamel_headers talamel.h)

add_library(talamel STATIC
    src/talamel.cpp 
    # ${talamel_headers}
)
target_include_directories(talamel
    PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
)
add_dependencies(talamel taglib)
target_link_libraries(talamel tag z stdc++)
set_target_properties(talamel PROPERTIES PUBLIC_HEADER "${talamel_headers}")

install(TARGETS talamel 
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_PREFIX}
    LIBRARY  DESTINATION ${CMAKE_INSTALL_PREFIX}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_PREFIX})  # This is for Windows
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_PREFIX})
# install(FILES ${CMAKE_SOURCE_DIR}/include/talamel.h DESTINATION ${CMAKE_INSTALL_PREFIX})

# Add an app depending on that
add_executable(Readmeta apps/readmeta.c)
add_dependencies(Readmeta talamel)
target_link_libraries(Readmeta talamel) 

# Add an app depending on taglib
add_executable(Metaprops apps/metaprops.cpp)
add_dependencies(Metaprops taglib)
target_link_libraries(Metaprops tag z)