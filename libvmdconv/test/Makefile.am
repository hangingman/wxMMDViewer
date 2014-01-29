check_PROGRAMS          = vmdconv-test1 vmdconv-test2
TESTS                   = vmdconv-test1 vmdconv-test2
AUTOMAKE_OPTIONS        = color-tests

vmdconv_test1_SOURCES	= vmdcomv-test1.cpp
vmdconv_test1_CXXFLAGS	= @CXXFLAGS@ -I$(top_builddir)/libvmdconv/include -I$(top_builddir)/libwxnkf/src
vmdconv_test1_LDADD	= $(top_builddir)/libvmdconv/src/libvmdconv.la $(top_builddir)/libwxnkf/src/libwxnkf.la
vmdconv_test1_LDFLAGS	= $(LDFLAGS)

vmdconv_test2_SOURCES	= vmdcomv-test2.cpp
vmdconv_test2_CXXFLAGS	= @CXXFLAGS@ -I$(top_builddir)/libvmdconv/include -I$(top_builddir)/libwxnkf/src
vmdconv_test2_LDADD	= $(top_builddir)/libvmdconv/src/libvmdconv.la $(top_builddir)/libwxnkf/src/libwxnkf.la
vmdconv_test2_LDFLAGS	= $(LDFLAGS)

# check_PROGRAMS	= wxnkf-test1 wxnkf-test2
# TESTS			= wxnkf-test1 wxnkf-test2
# AUTOMAKE_OPTIONS	= color-tests

# wxnkf_test1_SOURCES	= wxnkf-test1.cpp
# wxnkf_test1_CXXFLAGS	= @CXXFLAGS@ -I$(top_builddir)/src
# wxnkf_test1_LDADD	= $(top_builddir)/src/libwxnkf.la
# wxnkf_test1_LDFLAGS	= `@WX_CONFIG_PATH@ --libs base,core`

# wxnkf_test2_SOURCES	= wxnkf-test2.cpp
# wxnkf_test2_CXXFLAGS	= @CXXFLAGS@ -I$(top_builddir)/src
# wxnkf_test2_LDADD	= $(top_builddir)/src/libwxnkf.la
# wxnkf_test2_LDFLAGS	= `@WX_CONFIG_PATH@ --libs base,core`