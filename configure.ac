AC_INIT([rdb], [0.1], [bug-automake@gnu.org])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
LT_INIT([dlopen shared])

AC_PREREQ([2.69])
LT_PREREQ([2.4.2])

AC_PROG_CC
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([ po/Makefile.in
 Makefile
 src/Makefile
])

AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.21])

PKG_CHECK_MODULES([glib], [glib-2.0 >= 2.68.4])
PKG_CHECK_MODULES([json], [json-glib-1.0 >= 1.6.4])

AC_OUTPUT
