TESTS			=
check_PROGRAMS		=
AUTOMAKE_OPTIONS        = color-tests

TESTS                   += vmdconv-test1
check_PROGRAMS          += vmdconv-test1 
vmdconv_test1_SOURCES	=  vmdcomv-test1.cpp
vmdconv_test1_CXXFLAGS	=  @CXXFLAGS@ -I$(top_builddir)/libvmdconv/include -I(top_builddir)/babel
vmdconv_test1_LDADD	=  $(top_builddir)/libvmdconv/src/libvmdconv.la    $(top_builddir)/libbabel.la 
vmdconv_test1_LDFLAGS	=  $(LDFLAGS)

TESTS                   += vmdconv-test2
check_PROGRAMS          += vmdconv-test2 
vmdconv_test2_SOURCES	=  vmdcomv-test2.cpp
vmdconv_test2_CXXFLAGS	=  @CXXFLAGS@ -I$(top_builddir)/libvmdconv/include -I(top_builddir)/babel     
vmdconv_test2_LDADD	=  $(top_builddir)/libvmdconv/src/libvmdconv.la	   $(top_builddir)/libbabel.la
vmdconv_test2_LDFLAGS	=  $(LDFLAGS)

TESTS                   += vmdconv-test3
check_PROGRAMS          += vmdconv-test3 
vmdconv_test3_SOURCES	=  vmdconv-test3.cpp
vmdconv_test3_CXXFLAGS	=  @CXXFLAGS@ -I$(top_builddir)/libvmdconv/include -I(top_builddir)/babel     
vmdconv_test3_LDADD	=  $(top_builddir)/libvmdconv/src/libvmdconv.la	   $(top_builddir)/libbabel.la
vmdconv_test3_LDFLAGS	=  $(LDFLAGS)
