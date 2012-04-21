AC_PREREQ(2.68)

AC_INIT([cgisysinfo], [0.2], [rafael@rafaelmartins.eng.br], [cgisysinfo],
  [http://hg.rafaelmartins.eng.br/cgisysinfo])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([foreign no-dist-gzip dist-bzip2 subdir-objects])
AC_CONFIG_HEADERS([config.h])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

LT_INIT

AS_CASE([$host_os], [linux*], [os="linux"], [os="unsuported"])
if test "x$os" != "xlinux"; then
  AC_MSG_ERROR([cgisysinfo just works on Linux. ;) ])
fi

AC_MSG_CHECKING([for mercurial tip])
if test -d "${ac_top_srcdir:-.}/.hg"; then
  if test "x`hg id -t -R ${ac_top_srcdir:-.}`" = "xtip"; then
    HGTIP=`hg id -i -R ${ac_top_srcdir:-.}`
  fi
fi
if test -z "$HGTIP"; then
  AC_MSG_RESULT([no])
else
  AC_MSG_RESULT([$HGTIP])
  AC_DEFINE_UNQUOTED([HGTIP], ["$HGTIP"],
    [the changeset most recently added to the mercurial repository]
  )
fi

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

if test -z "$HGTIP"; then
  MY_PACKAGE_STRING="$PACKAGE_STRING"
else
  MY_PACKAGE_STRING="$PACKAGE_STRING/$HGTIP"
fi

echo "
        ==== ${MY_PACKAGE_STRING} ====
        
        prefix:       ${prefix}
        exec_prefix:  ${exec_prefix}
        bindir:       ${bindir}
        
        compiler:     ${CC}
        cflags:       ${CFLAGS}
        ldflags:      ${LDFLAGS}

	fastcgi:      ${FASTCGI}
"
