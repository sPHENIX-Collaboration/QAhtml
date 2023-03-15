AUTOMAKE_OPTIONS = foreign

INCLUDES = \
  -I$(includedir) \
  -I$(OFFLINE_MAIN)/include \
  -I$(ROOTSYS)/include \
  -I$(OPT_SPHENIX)/include

lib_LTLIBRARIES = libonlprodclient.la

noinst_HEADERS = \
  ClientHistoList.h \
  OnlProdHtml.h \
  OnlProdRunDBodbc.h

pkginclude_HEADERS = \
  OnlProdClient.h \
  OnlProdDraw.h

libonlprodclient_la_SOURCES = \
  ClientHistoList.cc \
  OnlProdClient.cc \
  OnlProdDraw.cc \
  OnlProdHtml.cc \
  OnlProdRunDBodbc.cc

libonlprodclient_la_LIBADD = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib \
  -L$(OPT_SPHENIX)/lib \
  -L$(ROOTSYS)/lib \
  -lfun4all \
  -lodbc \
  -lodbc++ \
  -lGui

noinst_PROGRAMS = \
  testexternals

testexternals_SOURCES = \
  testexternals.cc

testexternals_LDADD = \
  libonlprodclient.la

testexternals.cc:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

clean-local:
	rm -f *Dict.*
