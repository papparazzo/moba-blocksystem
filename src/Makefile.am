AUTOMAKE_OPTIONS = subdir-objects
bin_PROGRAMS = \
    moba-blocksystem

moba_blocksystem_SOURCES = \
    contacttoblock.cpp \
    layoutparser.cpp \
    main.cpp \
    msgloop.cpp \
    node_block.cpp \
    node_crossoverswitch.cpp \
    node_switch.cpp \
    node_threewayswitch.cpp \
    train.cpp

moba_blocksystem_LDADD = \
    -lmobacommon $(GTKMM_LIBS) \
    $(srcdir)/../modules/lib-msghandling/src/moba/libmsghandling.a \
    $(srcdir)/../modules/lib-tracklayout/src/moba/libtracklayout.a

LDFLAGS += -L/usr/local/lib

AM_CPPFLAGS = \
    $(GTKMM_CFLAGS) \
    -I$(srcdir)/../modules/lib-msghandling/src \
    -I$(srcdir)/../modules/lib-tracklayout/src \
    -g3 -g -ggdb -O3 -Wall

CLEANFILES = *~
