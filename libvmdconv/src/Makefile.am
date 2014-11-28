AUTOMAKE_OPTIONS		= foreign
ACLOCAL_AMFLAGS			= -I m4

wxmmdviewerdir			= $(pkglibdir)
wxmmdviewer_LTLIBRARIES		= libvmdconv.la
AM_CXXFLAGS			= -I$(top_builddir)/libvmdconv/include -I$(top_builddir)/babel -pedantic
libvmdconv_la_LDFLAGS         	= -no-undefined -version-info 1:1:0
libvmdconv_la_LIBADD		= $(top_builddir)/babel/libbabel.la
libvmdconv_la_SOURCES		= MMD_IO.cpp	 \
				  clsCSVFile.cpp \
				  clsPMDFile.cpp \
				  clsVMDFile.cpp \
				  common.cpp	 \
				  d3dxhelper.cpp
