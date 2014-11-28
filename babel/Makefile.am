AUTOMAKE_OPTIONS		= foreign
ACLOCAL_AMFLAGS			= -I m4
EXTRA_DATA			= $(wildcard *.csv) $(wildcard *.dat)

wxmmdviewerdir			= $(pkglibdir)
wxmmdviewer_LTLIBRARIES		= libbabel.la
libbabel_la_LDFLAGS         	= -no-undefined -version-info 1:0:0
libbabel_la_SOURCES		= babel.cpp babel.h
