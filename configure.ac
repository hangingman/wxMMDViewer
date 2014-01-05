#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([wxMMDViewer], [0.0.1], [newserver002@gmail.com])
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([-Wno-portability])
AC_CONFIG_SRCDIR([src/enumstring.hpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_SYS_LARGEFILE

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_LIBTOOL_WIN32_DLL
LT_INIT

# Checks for libraries.

#-----------------------------------------------------------------------
# Checks for wxWidgets.
#
AM_OPTIONS_WXCONFIG
reqwx=2.8.0
AM_PATH_WXCONFIG($reqwx, wxWin=1)
if test "$wxWin" != 1; then
	AC_MSG_ERROR([
		wxWidgets must be installed on your system.
 
		Please check that wx-config is in path, the directory
		where wxWidgets libraries are installed (returned by
		'wx-config --libs' or 'wx-config --static --libs' command)
		is in LD_LIBRARY_PATH or equivalent variable and
		wxWidgets version is $reqwx or above.
		])
fi

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Add Flags
CPPFLAGS="$CPPFLAGS $WX_CPPFLAGS"
CXXFLAGS="$CXXFLAGS $WX_CXXFLAGS_ONLY -std=c++11"
CFLAGS="$CFLAGS $WX_CFLAGS_ONLY"
LIBS="$LIBS $WX_LIBS"

# Check compile flag
# AX_CHECK_COMPILE_FLAG(FLAG, [ACTION-SUCCESS], [ACTION-FAILURE], [EXTRA-FLAGS], [INPUT])
# AX_CHECK_COMPILE_FLAG(-std=c++11, CXXFLAGS="$CXXFLAGS -std=c++0x")
# AX_CHECK_COMPILE_FLAG([-std=c++11], [CXXFLAGS="$CXXFLAGS -std=c++11"])
# AX_CHECK_COMPILE_FLAG([-fno-stack-protector], [CXXFLAGS="$CXXFLAGS -fno-stack-protector"])

# Checks for library functions.
AC_CHECK_FUNCS([memset])

AC_CONFIG_FILES([Makefile
                 libvmdconv/Makefile
                 libvmdconv/include/Makefile
                 libvmdconv/src/Makefile
                 src/Makefile])
AC_OUTPUT
