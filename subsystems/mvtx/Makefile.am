AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = \
  -I$(includedir) \
  -isystem$(OFFLINE_MAIN)/include \
  -isystem$(OFFLINE_MAIN)/rootmacros \
  -isystem$(ROOTSYS)/include 

lib_LTLIBRARIES = \
  libqadrawmvtx.la \
  libqadrawmvtxrawhit.la

libqadrawmvtx_la_LIBADD = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib \
  -lqadrawclient \
  -lqadrawdb

libqadrawmvtxrawhit_la_LIBADD = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib \
  -lqadrawclient \
  -lqadrawdb

mvtxincludedir=$(pkgincludedir)/mvtx
mvtxrawhitincludedir=$(pkgincludedir)/mvtx

mvtxinclude_HEADERS = \
  MVTXDraw.h

mvtxrawhitinclude_HEADERS = \
  MVTXRawHitDraw.h

libqadrawmvtx_la_SOURCES = \
  MVTXDraw.cc

libqadrawmvtxrawhit_la_SOURCES = \
  MVTXRawHitDraw.cc

noinst_PROGRAMS = \
  testexternals

testexternals_SOURCES = \
  testexternals.cc

testexternals_LDADD = \
  libqadrawmvtx.la \
  libqadrawmvtxrawhit.la

testexternals.cc:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

clean-local:
	rm -f $(BUILT_SOURCES)
