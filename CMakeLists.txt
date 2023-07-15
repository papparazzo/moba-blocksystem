cmake_minimum_required(VERSION 3.20)

project(moba-blocksystem VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(config.h.in config.h)

add_executable(
    moba-blocksystem

    src/main.cpp
)

install(TARGETS moba-blocksystem)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

target_include_directories(moba-blocksystem PUBLIC "${PROJECT_BINARY_DIR}")

target_link_libraries(moba-blocksystem mobacommon)
target_link_libraries(moba-blocksystem ${CMAKE_SOURCE_DIR}/modules/lib-msghandling/src/moba/libmsghandling.a)

include_directories(${CMAKE_SOURCE_DIR}/modules/lib-msghandling/src)

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
