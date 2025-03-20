AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = \
  -I$(includedir) \
  -isystem$(OFFLINE_MAIN)/include \
  -isystem$(ROOTSYS)/include \
  -isystem$(OPT_SPHENIX)/include

lib_LTLIBRARIES = libqadrawclient.la

noinst_HEADERS = \
  ClientHistoList.h \
  QAHtml.h \
  QARunDBodbc.h

pkginclude_HEADERS = \
  QADrawClient.h \
  QADraw.h \
  SingleCanvasDrawer.h

libqadrawclient_la_SOURCES = \
  ClientHistoList.cc \
  QADrawClient.cc \
  QADraw.cc \
  QAHtml.cc \
  QARunDBodbc.cc \
  SingleCanvasDrawer.cc

libqadrawclient_la_LIBADD = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib \
  -L$(OPT_SPHENIX)/lib \
  -L$(ROOTSYS)/lib \
  -lfun4all \
  -lodbc \
  -lodbc++ \
  -lGui

BUILT_SOURCES = \
  testexternals.cc

noinst_PROGRAMS = \
  testexternals

testexternals_SOURCES = \
  testexternals.cc

testexternals_LDADD = \
  libqadrawclient.la

testexternals.cc:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

clean-local:
	rm -f $(BUILT_SOURCES)
