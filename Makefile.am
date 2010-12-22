INCLUDES = $(all_includes) -I$(top_srcdir)/src

bin_PROGRAMS = cgisysinfo

cgisysinfo_SOURCES = .c src/system.c src/memory.c src/network.c src/filesystem.c src/utils.c
cgisysinfo_LDADD = $(AM_LDADD) -lm

noinst_HEADERS = src/system.h src/memory.h src/network.h src/filesystem.h src/utils.h

ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = foreign no-dist-gzip dist-bzip2

EXTRA_DIST = bootstrap.sh

dist_doc_DATA = NEWS COPYING README AUTHORS
