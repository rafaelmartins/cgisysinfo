AC_PREREQ(2.68)

AC_INIT([cgisysinfo], [0.4], [rafael@rafaelmartins.eng.br], [cgisysinfo],
  [http://projects.rafaelmartins.eng.br/cgisysinfo])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([foreign no-dist-gzip dist-bzip2 subdir-objects])
AC_CONFIG_HEADERS([config.h])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

LT_INIT

AS_CASE([$host_os], [linux*], , AC_MSG_ERROR([cgisysinfo just works on Linux.]))

AS_IF([test -d "${ac_top_srcdir:-.}/.hg"], [
  AC_MSG_CHECKING([for mercurial tip])
  AS_IF([test "x`hg id -t -R ${ac_top_srcdir:-.}`" = "xtip"], [
    HGTIP=`hg id -i -R ${ac_top_srcdir:-.}`
    AC_MSG_RESULT([$HGTIP])
    AC_DEFINE_UNQUOTED([HGTIP], ["$HGTIP"],
      [Define to the changeset most recently added to the mercurial repository]
    )
  ], [
    AC_MSG_RESULT([no])
  ])
])

AC_PROG_CC_C99
AS_IF([test "x$ac_cv_prog_cc_c99" = "xno"], [
  AC_MSG_ERROR([no C99 compiler found, cgisysinfo requires a C99 compiler.])
])

AC_ARG_ENABLE([fastcgi], AS_HELP_STRING([--enable-fastcgi], [enable fastcgi support]))
AS_IF([test "x$enable_fastcgi" = "xyes"], [
  AC_SEARCH_LIBS([FCGI_Accept], [fcgi], ,
    AC_MSG_ERROR([FCGI_Accept not found. please install the fastcgi developer kit.])
  )
  AC_DEFINE([ENABLE_FASTCGI], [], [fastcgi support enabled])
  FASTCGI="enabled"
], [
  FASTCGI="disabled"
])

AC_SEARCH_LIBS([floor], [m], , AC_MSG_ERROR([floor not found in -lm.]))
AC_CHECK_FUNCS([strtoul])
AC_CHECK_HEADERS([sys/statvfs.h])

AC_SYS_LARGEFILE

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT

AS_IF([test -z "$HGTIP"], [
  MY_PACKAGE_STRING="$PACKAGE_STRING"
], [
  MY_PACKAGE_STRING="$PACKAGE_STRING/$HGTIP"
])

AS_ECHO("
        ==== ${MY_PACKAGE_STRING} ====

        prefix:       ${prefix}
        exec_prefix:  ${exec_prefix}
        bindir:       ${bindir}

        compiler:     ${CC}
        cflags:       ${CFLAGS}
        ldflags:      ${LDFLAGS}

        fastcgi:      ${FASTCGI}
")
