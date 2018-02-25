
AC_INIT([desmag], [0.1], [yuri.borisov.v@gmail.com])

AM_INIT_AUTOMAKE([-Wall])

AM_PROG_AR

AC_CONFIG_FILES([
		 Makefile
		 src/Makefile
		 test/Makefile
		 ])


AC_PROG_CC
AC_PROG_CXX
AC_PROG_LIBTOOL

# Configure options: --enable-debug[=no].
AC_ARG_ENABLE([debug],
	      [AS_HELP_STRING([--enable-debug],
			      [enable debug code (default is no)])],
			      [debug="$withval"], [debug=no])


PKG_CHECK_MODULES([LIBDESA], [libdesa >= 0.1], , [AC_MSG_ERROR([
You must have the libdesa-0.1 and development headers installed to build.
])])

AC_SUBST(LIBDESA_CFLAGS)
AC_SUBST(LIBDESA_LIBS)


PKG_CHECK_MODULES(LIBXML, [libxml-2.0], , [AC_MSG_ERROR([
You must have the libxml-2.0 and development headers installed to build.
])])

AC_SUBST(LIBXML_CFLAGS)
AC_SUBST(LIBXML_LIBS)

PKG_CHECK_MODULES([LIBCPPU], [cppunit >= 1.13], , [AC_MSG_ERROR([
You must have the libcppunit-1.13 and development headers installed to build.
])])

AC_SUBST(LIBCPPU_CFLAGS)
AC_SUBST(LIBCPPU_LIBS)

CXXFLAGS="-std=c++11"

if test x$debug = xyes; then
    CXXFLAGS="$CXXFLAGS -g -O0"
else
#    CXXFLAGS="-g -O2"
    CXXFLAGS="$CXXFLAGS -g -O0"
fi


CXXFLAGS="$CXXFLAGS $LIBCPPU_CFLAGS $LIBDESA_CFLAGS $LIBXML_CFLAGS"
LDFLAGS="$LDFLAGS $LIBCPPU_LIBS $LIBDESA_LIBS $LIBXML_LIBS"

AC_CONFIG_FILES([libdesmag.pc])

AC_OUTPUT

echo CXXFLAGS $CXXFLAGS
echo "
($PACKAGE_NAME) version $PACKAGE_VERSION
Prefix.........: $prefix
Debug Build....: $debug
C++ Compiler...: $CXX $CXXFLAGS $CPPFLAGS
Linker.........: $LD $LDFLAGS $LIBS
Doxygen........: ${DOXYGEN:-NONE}
"

