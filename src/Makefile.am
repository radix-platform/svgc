
lib_LTLIBRARIES = libsvgc.la

libsvgc_la_SOURCES = svgc-dlist.c \
                     svgc-dlist.h \
                     svgc.c \
                     svgc.h \
                     _svgc_version.c \
                     $(top_srcdir)/version.c \
                     $(top_srcdir)/version.h

libsvgc_la_CFLAGS = -I$(top_srcdir)

libsvgc_la_LDFLAGS = -version-info 1:2:0

include_HEADERS = svgc.h svgc-dlist.h
