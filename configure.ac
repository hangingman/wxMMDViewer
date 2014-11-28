#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])
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
AC_CHECK_SIZEOF([void *])
LT_INIT

# Checks for HostOS
case "$host" in
        *-*-mingw*|*-*-cygwin*)
                AC_DEFINE(PLATFORM_WIN32, 1, [Platform is Win32])
                PLATFORM_STRING="Win32"
                AC_SUBST(PLATFORM_STRING)
                ;;
        *-*-linux*)
                AC_DEFINE(PLATFORM_LINUX, 1, [Platform is Linux])
                PLATFORM_STRING="Linux"
                AC_SUBST(PLATFORM_STRING)
		dnl Recent Debian versions (as of 2011) use new approach to multiarch
                dnl and put the libraries under /usr/lib/arch-linux-gnu. Annoyingly,
                dnl "arch" here is not `uname -m` because it is "i386" even when uname
                dnl returns e.g. "i686". So we need to test for it explicitly.
                if test "$ac_cv_sizeof_void_p" = 8; then
                    if test -d "/usr/lib/`uname -m`-linux-gnu"; then
                        LDFLAGS="-L/usr/lib/`uname -m`-linux-gnu"
                    elif test -d "/usr/lib64" -a ! -h "/usr/lib64"; then
                        LDFLAGS="-L/use/lib64"
                    fi
                else
                    case "${host}" in
                        i*86-*-linux* )
                            if test -d '/usr/lib/i386-linux-gnu'; then
                                LDFLAGS='-L/usr/lib/i386-linux-gnu'
                            fi
                    esac
                fi
                ;;
        *-*-darwin*)
                AC_DEFINE(PLATFORM_APPLE, 1, [Platform is Apple])
                PLATFORM_STRING="Apple"
                AC_SUBST(PLATFORM_STRING)
                ;;
        *-*-*bsd*)
                AC_DEFINE(PLATFORM_BSD, 1, [Platform is BSD])
                PLATFORM_STRING="BSD"
                AC_SUBST(PLATFORM_STRING)
                ;;
        *)
                AC_MSG_WARN([*** Please add $host to configure.ac checks!])
                ;;
esac

# Checks for libraries.
if test "$PLATFORM_STRING" = "Apple"; then
   if hash port 2>/dev/null; then
      CFLAGS="$CFLAGS -I/opt/local/include";
      CXXFLAGS="$CXXFLAGS -I/opt/local/include";
      CPPFLAGS="$CPPFLAGS -I/opt/local/include";
      LIBS="$LIBS -L/opt/local/lib"
   fi

   case "$CXX" in
           *clang*)
	   CXXFLAGS="$CXXFLAGS -stdlib=libc++ -Wno-invalid-source-encoding"
    	;;
           *)
        ;;
   esac
fi

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

# Checks for m
AC_CHECK_LIB(m, exp, LIBS="-lm $LIBS" has_m=1,has_m=0)
if test $has_m = 0; then
   AC_MSG_ERROR("ERROR: cannot find libm.")
fi

AC_MSG_CHECKING([for OpenGL headers])
if test "$PLATFORM_STRING" = "Linux"; then

   WX_FIND_LIB(GL, glBegin, [/opt/graphics/OpenGL/lib])
   if test "$ac_find_libraries" != "" ; then
      if test "$ac_find_libraries" != "std" ; then
      	 WX_LINK_PATH_EXIST([$ac_find_libraries],[$LDFLAGS])
         if test "$ac_path_to_link" != " -L/usr/lib" ; then
    	    LDFLAGS_GL="$ac_path_to_link"
         fi
       fi
    
       dnl don't suppose that libGL and libGLU are always in the
       dnl same directory -- this is not true for some common
       dnl distributions
       WX_FIND_LIB(GLU, gluBeginCurve, [/opt/graphics/OpenGL/lib])
       if test "$ac_find_libraries" != "" ; then
       	  if test "$ac_find_libraries" != "std" ; then
    	     WX_LINK_PATH_EXIST([$ac_find_libraries],[$LDFLAGS])
    	     if test "$ac_path_to_link" != " -L/usr/lib" -a \
    	     	"$ac_path_to_link" != "$LDFLAGS_GL" ; then
    	  	LDFLAGS_GL="$LDFLAGS_GL$ac_path_to_link"
	     fi
          fi

	  GL_LIBS="-lGL"
	  GLU_LIBS="-lGLU"
       fi
   fi

   # glut
   AC_SEARCH_LIBS([glutInit], [glut], [], [
     AC_MSG_FAILURE(["Cannot find glut. Try setting LDFLAGS -L"], [-1])
   ])
else
   # Checks for GL
   AX_CHECK_GL()
   # Checks for GLU
   AX_CHECK_GLU()
   # Checks for glut
   AX_CHECK_GLUT()
fi

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h langinfo.h locale.h stdlib.h string.h strings.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Add Flags
CPPFLAGS="$CPPFLAGS $WX_CPPFLAGS"
CXXFLAGS="$CXXFLAGS $WX_CXXFLAGS_ONLY $GL_CFLAGS $GLU_CFLAGS $GLUT_CFLAGS"
CFLAGS="$CFLAGS $WX_CFLAGS_ONLY $GL_CFLAGS $GLU_CFLAGS $GLUT_CFLAGS"
LIBS="$LIBS $WX_LIBS $GL_LIBS $GLU_LIBS $GLUT_LIBS"
AC_SUBST(GL_LIBS)
AC_SUBST(GLU_LIBS)
AC_SUBST(GLUT_LIBS)

# Check compile flag
AX_CXX_COMPILE_STDCXX_11([noext], [mandatory])

# Checks for library functions.
AC_CHECK_FUNCS([memset munmap nl_langinfo strcasecmp strchr strcspn strpbrk strrchr strspn strstr fdatasync usleep fullfsync localtime_r gmtime_r])

AC_CONFIG_FILES([Makefile
		 babel/Makefile
                 libvmdconv/Makefile
                 libvmdconv/include/Makefile
                 libvmdconv/src/Makefile
		 libvmdconv/test/Makefile
                 src/Makefile])
AC_OUTPUT

AC_MSG_RESULT([CC:		$CC])
AC_MSG_RESULT([CXX:		$CXX])
AC_MSG_RESULT([CPPFLAGS:	$CPPFLAGS])
AC_MSG_RESULT([CXXFLAGS:	$CXXFLAGS])
AC_MSG_RESULT([LIBS:		$LIBS])
