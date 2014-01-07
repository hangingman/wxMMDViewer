AUTOMAKE_OPTIONS		= foreign
wxmmdviewerexecdir		= $(datarootdir)/@PACKAGE@
wxmmdviewerexec_PROGRAMS	= wxMMDViewer
#wxmmdviewerexecrcdir		= $(datarootdir)/@PACKAGE@/rc
#wxmmdviewerexecrc_DATA		= $(wildcard rc/*)

wxMMDViewerdir	  	= $(pkglibdir)
wxMMDViewer_SOURCES 	= main.cpp        \
			  enumstring.hpp  \
			  wxmmdviewer.cpp \
			  wxmmdviewer.hpp \
			  basicglpane.cpp \
			  basicglpane.hpp

# common build rule
wxMMDViewer_CXXFLAGS	= @CXXFLAGS@ -I$(top_builddir)/libvmdconv/include
ACLOCAL_AMFLAGS		= -I m4

# Library dependency
wxMMDViewer_LDADD	= $(top_builddir)/libvmdconv/src/libvmdconv.la
wxMMDViewer_LDFLAGS	= `@WX_CONFIG_PATH@ --libs aui,base,core,gl`
