bin_PROGRAMS = ut-desmag-lib

AM_CPPFLAGS = -D_PLAT_LINUX_ -g -O0

ut_desmag_lib_SOURCES = \
		       ut_main.cpp \
		       ut_base.cpp

ut_desmag_lib_LDFLAGS=-L../src/.libs/ -ldesmag  -lcppunit -lrt
ut_desmag_lib_CXXFLAGS=-I@srcdir@/../src -I@includedir@/desa -pthread

check: test_desmag_lib

test_desa_lib:
	 ./ut-desmag-lib


