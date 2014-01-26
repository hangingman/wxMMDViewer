check_PROGRAMS          = vmdconv-test1
TESTS                   = vmdconv-test1
AUTOMAKE_OPTIONS        = color-tests

vmdconv_test1_SOURCES        = vmdcomv-test1.cpp
vmdconv_test1_CXXFLAGS       = @CXXFLAGS@ -I$(top_builddir)/libvmdconv/include -I$(top_builddir)/libwxnkf/src
vmdconv_test1_LDADD          = $(top_builddir)/libvmdconv/src/libvmdconv.la $(top_builddir)/libwxnkf/src/libwxnkf.la
