AC_PREREQ([2.68])

AC_INIT([cgisysinfo],[0.1],[rafael@rafaelmartins.eng.br],[cgisysinfo],[http://labs.rafaelmartins.org/wiki/cgisysinfo])
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

AC_ARG_ENABLE(cgi, AS_HELP_STRING([--enable-cgi],[enable CGI support]),
              enable_cgi=yes)

AC_MSG_CHECKING([for instalation directory])
AC_ARG_WITH(cgibindir,
            AS_HELP_STRING([--with-cgibindir],[specify the directory where CGI files go]),
            CGIBINDIR="${withval}")
if test "x$enable_cgi" = "xyes"; then
  if test -z "$CGIBINDIR"; then
    AC_MSG_ERROR([As you enabled CGI support, you should specify a cgi-bin directory])
  else
    AC_MSG_RESULT([$CGIBINDIR])
    INSTALL_DIR="$CGIBINDIR"
  fi
else
  AC_MSG_RESULT([$bindir])
  INSTALL_DIR="$bindir"
fi
AC_SUBST([INSTALL_DIR])

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
        
        installation dir:      ${INSTALL_DIR}
        
        compiler:              ${CC}
        cflags:                ${CFLAGS}
        ldflags:               ${LDFLAGS}
"
