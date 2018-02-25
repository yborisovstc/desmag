includedir = @includedir@/desa

AM_CPPFLAGS = -g -O0

lib_LTLIBRARIES = libdesmag.la

libdesmag_la_SOURCES = \
		       mchromo.h \
		       mplugin.h \
		       chromo.h \
		       chromo.cpp \
		       uri.h \
		       uri.cpp \
		       env.h \
		       env.cpp \
		       chromox.h \
		       chromox.cpp \
		       prov.h \
		       prov.cpp \
		       provmgr.h \
		       provmgr.cpp \
		       provdef.h \
		       provdef.cpp \
		       menv.h \
		       elem.h \
		       elem.cpp

include_HEADERS = \
		  menv.h \
		  elem.h


libdesmag_la_LDFLAGS = -rdynamic -no-undefined -lrt
