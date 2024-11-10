lib_LTLIBRARIES = lib.la
lib_la_SOURCES = lib.c
include_HEADERS = ${PACKAGE}.h

bin_PROGRAMS = rdb
rdb_SOURCES = main.c ${PACKAGE}.h
rdb_LDADD = lib.la @glib_LIBS@ @json_LIBS@
AM_CFLAGS = @glib_CFLAGS@ @json_CFLAGS@ \
  -DLOCALEDIR='"$(localedir)"'\
  -DVERSION="${VERSION}" \
  -DPACKAGE='"${PACKAGE}"'
