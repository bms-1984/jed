AC_INIT([jed], [0.0.1], [benjaminsutter@outlook.com])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_SRCDIR([src/jed.c])
AM_INIT_AUTOMAKE([-Wall -Werror dist-zstd tar-pax subdir-objects gnu])
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.19.1])
AC_PROG_CC
AC_CONFIG_HEADERS([lib/config.h])
AC_CONFIG_FILES([Makefile src/Makefile po/Makefile.in doc/Makefile])
LIBCURL_CHECK_CONFIG
PKG_CHECK_MODULES([JANSSON],[jansson])
AC_OUTPUT
