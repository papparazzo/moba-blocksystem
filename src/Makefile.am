AUTOMAKE_OPTIONS = subdir-objects
bin_PROGRAMS = \
    moba-blocksystem

moba_blocksystem_SOURCES = \
    main.cpp \
    msgloop.cpp \
    train.cpp \
    node_block.cpp \
    node_switch.cpp \
    node_terminator.cpp \
    node_crossoverswitch.cpp

moba_blocksystem_LDADD = \
    -lMoBaCommon $(GTKMM_LIBS) \
    $(srcdir)/../modules/lib-msghandling/src/moba/libmsghandling.a

LDFLAGS += -L/usr/local/lib

AM_CPPFLAGS = \
    $(GTKMM_CFLAGS) \
    -I$(srcdir)/../modules/lib-msghandling/src \
    -I$(srcdir)/../modules/lib-tracklayout/src \
    -g3 -g -ggdb -O3 -Wall

CLEANFILES = *~
