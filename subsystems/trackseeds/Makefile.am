AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = \
  -I$(includedir) \
  -isystem$(OFFLINE_MAIN)/include \
  -isystem$(OFFLINE_MAIN)/rootmacros \
  -isystem$(ROOTSYS)/include 

lib_LTLIBRARIES = \
  libqadrawsiliconseeds.la \
  libqadrawtpcseeds.la

libqadrawsiliconseeds_la_LIBADD = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib \
  -lqadrawclient \
  -lqadrawdb

libqadrawtpcseeds_la_LIBADD = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib \
  -lqadrawclient \
  -lqadrawdb

siliconseedsincludedir=$(pkgincludedir)/trackseeds
tpcseedsincludedir=$(pkgincludedir)/trackseeds

siliconseedsinclude_HEADERS = \
  SiliconSeedsDraw.h

tpcseedsinclude_HEADERS = \
  TpcSeedsDraw.h

libqadrawsiliconseeds_la_SOURCES = \
  SiliconSeedsDraw.cc

libqadrawtpcseeds_la_SOURCES = \
  TpcSeedsDraw.cc

noinst_PROGRAMS = \
  testexternals

testexternals_SOURCES = \
  testexternals.cc

testexternals_LDADD = \
  libqadrawsiliconseeds.la \
  libqadrawtpcseeds.la

testexternals.cc:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

clean-local:
	rm -f $(BUILT_SOURCES)
