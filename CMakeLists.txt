cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

project(moba-blocksystem VERSION 1.0.0)

configure_file(config.h.in config.h)

add_executable(
    moba-blocksystem

    src/main.cpp
    src/msgloop.cpp
    src/layoutparser.cpp
    src/screen.cpp
)

install(TARGETS moba-blocksystem)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

target_include_directories(moba-blocksystem PUBLIC "${PROJECT_BINARY_DIR}")

target_link_libraries(moba-blocksystem mobacommon)
target_link_libraries(moba-blocksystem ncurses)
target_link_libraries(moba-blocksystem ${CMAKE_SOURCE_DIR}/modules/lib-msghandling/libmoba-lib-msghandling.a)
target_link_libraries(moba-blocksystem ${CMAKE_SOURCE_DIR}/modules/lib-tracklayout/libmoba-lib-tracklayout.a)

include_directories(${CMAKE_SOURCE_DIR}/modules/lib-msghandling/src)
include_directories(${CMAKE_SOURCE_DIR}/modules/lib-tracklayout/src)

#AM_CPPFLAGS = \
#    $(GTKMM_CFLAGS) \
#    -I$(srcdir)/../modules/lib-msghandling/src \
#    -I$(srcdir)/../modules/lib-tracklayout/src \
#    -g3 -g -ggdb  -Wall
#
### Process this file with automake to generate Makefile.in
#SUBDIRS = \
#    modules/lib-msghandling/src/moba \
#    modules/lib-tracklayout/src/moba \
#    src
#
