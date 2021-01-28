AUTOMAKE_OPTIONS = subdir-objects
bin_PROGRAMS = \
    moba-blocksystem

moba_blocksystem_SOURCES = \
    layoutparser.cpp \
    main.cpp \
    msgloop.cpp \
    screen.cpp

moba_blocksystem_LDADD = \
    -lmobacommon -lncurses $(GTKMM_LIBS) \
    $(srcdir)/../modules/lib-msghandling/src/moba/libmsghandling.a \
    $(srcdir)/../modules/lib-tracklayout/src/moba/libtracklayout.a

LDFLAGS += -L/usr/local/lib

AM_CPPFLAGS = \
    $(GTKMM_CFLAGS) \
    -I$(srcdir)/../modules/lib-msghandling/src \
    -I$(srcdir)/../modules/lib-tracklayout/src \
    -g3 -g -ggdb  -Wall

CLEANFILES = *~
