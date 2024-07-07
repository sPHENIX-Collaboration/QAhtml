AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = \
  -I$(includedir) \
  -isystem$(OFFLINE_MAIN)/include \
  -isystem$(OFFLINE_MAIN)/rootmacros \
  -isystem$(ROOTSYS)/include \
  -isystem$(OPT_SPHENIX)/include

lib_LTLIBRARIES = \
  libqadrawsiliconseeds.la

libqadrawsiliconseeds_la_LIBADD = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib \
  -lqadrawclient \
  -lqadrawdb \
  -lodbc++

siliconseedsincludedir=$(pkgincludedir)/siliconseeds

siliconseedsinclude_HEADERS = \
  SiliconSeedsDraw.h \
  SiSeedsGoodRunChecker.h \ 
  OfflineQAKSTest.h

libqadrawsiliconseeds_la_SOURCES = \
  SiliconSeedsDraw.cc \
  SiSeedsGoodRunChecker.cc \
  OfflineQAKSTest.cc

noinst_PROGRAMS = \
  testexternals

testexternals_SOURCES = \
  testexternals.cc

testexternals_LDADD = \
  libqadrawsiliconseeds.la

testexternals.cc:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

clean-local:
	rm -f $(BUILT_SOURCES)
