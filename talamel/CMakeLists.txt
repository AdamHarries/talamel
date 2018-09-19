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
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${TAGLIB_INSTALL} -DBUILD_SHARED_LIBS=OFF -DENABLE_STATIC_RUNTIME=ON -DCMAKE_C_FLAGS=-fPIC -Wall -O3
        -DCMAKE_CXX_FLAGS=-fPIC -Wall -03
)

include_directories(${TAGLIB_INSTALL}/include)
link_directories(${TAGLIB_INSTALL}/lib)

# Decide on C/C++ standards
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(MSVC)
  foreach(flag_var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
    string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
  endforeach(flag_var)
endif()


# Add the talamel library

add_library(talamel STATIC
    src/talamel.cpp 
)
target_include_directories(talamel
    PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
)
add_dependencies(talamel taglib)
# target_link_libraries(talamel -Wl,--whole-archive tag -Wl,--no-whole-archive)
# target_link_libraries(talamel -Wl,-force_load tag z -Wl,-noforce_load)
target_link_libraries(talamel tag z)
set_target_properties(talamel PROPERTIES PUBLIC_HEADER "${talamel_headers}")
set_target_properties(talamel PROPERTIES LINKER_LANGUAGE C)


install(TARGETS talamel 
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_PREFIX}
    LIBRARY  DESTINATION ${CMAKE_INSTALL_PREFIX}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_PREFIX})  # This is for Windows
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_PREFIX})

# Add an example app depending on talamel
add_executable(Readmeta apps/readmeta.c)
add_dependencies(Readmeta talamel)
set_source_files_properties(Readmeta LANGUAGE C)
set_target_properties(Readmeta PROPERTIES LINKER_LANGUAGE C)
target_link_libraries(Readmeta talamel) 
