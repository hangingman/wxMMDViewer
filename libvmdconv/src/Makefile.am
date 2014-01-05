AUTOMAKE_OPTIONS		= foreign
ACLOCAL_AMFLAGS			= -I m4

wxmmdviewerdir			= $(pkglibdir)
wxmmdviewer_LTLIBRARIES		= libvmdconv.la
AM_CXXFLAGS			= -I$(top_builddir)/libvmdconv/include -pedantic
libvmdconv_la_LDFLAGS         	= -no-undefined -version-info 1:1:0
libvmdconv_la_SOURCES		= MMD_IO.cpp	 \
				  clsCSVFile.cpp \
				  clsPMDFile.cpp \
				  clsVMDFile.cpp \
				  common.cpp	 \
				  d3dxhelper.cpp