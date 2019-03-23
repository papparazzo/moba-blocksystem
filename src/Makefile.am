AUTOMAKE_OPTIONS = subdir-objects
bin_PROGRAMS = \
    moba-blocksystem

moba_blocksystem_SOURCES = \
    main.cpp 

moba_blocksystem_LDADD = \
    -lMoBaCommon $(GTKMM_LIBS) \
    $(srcdir)/../modules/lib-msghandling/src/moba/libmsghandling.a

LDFLAGS += -L/usr/local/lib

AM_CPPFLAGS = \
    $(GTKMM_CFLAGS) \
    -I$(srcdir)/../modules/lib-msghandling/src \
    -g3 -g -ggdb -O3 -Wall

CLEANFILES = *~
