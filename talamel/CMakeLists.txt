cmake_minimum_required (VERSION 3.0)

project(talamel)

# Compile taglib
include(ExternalProject)

set(ZLIB_PREFIX "${CMAKE_BINARY_DIR}/zlib")
set(ZLIB_INSTALL "${ZLIB_PREFIX}/install")

ExternalProject_Add(
    zlib
    GIT_REPOSITORY "https://github.com/madler/zlib"
    GIT_TAG "v1.2.11"
    PREFIX ${ZLIB_PREFIX}
    INSTALL_DIR ${ZLIB_INSTALL}
    BUILD_IN_SOURCE 1
    BUILD_BYPRODUCTS ${ZLIB_STATIC_LIBRARY}
    CMAKE_CACHE_ARGS
          -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=TRUE
          -DCMAKE_BUILD_TYPE:STRING=Release
          -DCMAKE_INSTALL_PREFIX:STRING=${ZLIB_INSTALL}
)

include_directories(${ZLIB_INSTALL}/include)

set(TAGLIB_PREFIX "${CMAKE_BINARY_DIR}/taglib")
set(TAGLIB_INSTALL "${TAGLIB_PREFIX}/install")

ExternalProject_Add(
    taglib 
    GIT_REPOSITORY https://www.github.com/taglib/taglib
    PREFIX ${TAGLIB_PREFIX} 
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${TAGLIB_INSTALL} -DCMAKE_BUILD_TYPE:STRING=Release -DBUILD_SHARED_LIBS=OFF -DENABLE_STATIC_RUNTIME=ON -DCMAKE_C_FLAGS=-fPIC -Wall -O3
        -DCMAKE_CXX_FLAGS=-fPIC -Wall -03
)

include_directories(${TAGLIB_INSTALL}/include)
link_directories(${TAGLIB_INSTALL}/lib)

if(WIN32)
if(MSVC)
  set(ZLIB_STATIC_LIBRARY
      debug ${ZLIB_INSTALL}/lib/zlibstaticd.lib
      optimized ${ZLIB_INSTALL}/lib/zlibstatic.lib)
  set(TAGLIB_STATIC_LIBRARY
      debug ${TAGLIB_INSTALL}/lib/tag.lib
      optimized ${TAGLIB_INSTALL}/lib/tag.lib)
else()
  if(CMAKE_BUILD_TYPE EQUAL Debug)
    set(ZLIB_STATIC_LIBRARY
        ${ZLIB_INSTALL}/lib/zlibstaticd.lib)
    set(TAGLIB_STATIC_LIBRARY
        ${TAGLIB_INSTALL}/lib/tagd.lib)
  else()
    set(ZLIB_STATIC_LIBRARY
        ${ZLIB_INSTALL}/lib/zlibstatic.lib)
    set(TAGLIB_STATIC_LIBRARY
        ${TAGLIB_INSTALL}/lib/tag.lib)
  endif()
endif()
else()
    set(ZLIB_STATIC_LIBRARY
        ${ZLIB_INSTALL}/lib/libz.a)
    set(TAGLIB_STATIC_LIBRARY
        ${TAGLIB_INSTALL}/lib/libtag.a)
endif()

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

# Get zlib.
# find_package(ZLIB REQUIRED)

# Add the talamel library
add_library(talamel STATIC
    src/talamel.cpp 
)
target_include_directories(talamel
    PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
)
add_dependencies(talamel taglib zlib)
target_link_libraries(talamel PUBLIC ${TAGLIB_STATIC_LIBRARY} ${ZLIB_STATIC_LIBRARY})
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
