AC_PREREQ(2.68)

AC_INIT([cgisysinfo], [0.1], [http://labs.rafaelmartins.org/wiki/cgisysinfo])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([foreign no-dist-gzip dist-bzip2 subdir-objects])
AC_CONFIG_HEADERS([config.h])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

LT_PREREQ([2.2.4])
LT_INIT

AC_PROG_CC_C99
AS_IF([test "x$ac_cv_prog_cc_c99" = "xno"], [
  AC_MSG_ERROR([no C99 compiler found, cgisysinfo requires a C99 compiler.])
])

AC_SEARCH_LIBS([floor], [m], , AC_MSG_ERROR([floor not found in -lm.]))
AC_CHECK_FUNCS([strtoul])
AC_CHECK_HEADERS([sys/statvfs.h])

AC_SYS_LARGEFILE

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT

echo "
        ==== ${PACKAGE_STRING} ====
        
        prefix:      ${prefix}
        exec_prefix: ${exec_prefix}
        bindir:      ${bindir}
        
        compiler:    ${CC}
        cflags:      ${CFLAGS}
        ldflags:     ${LDFLAGS}
"
