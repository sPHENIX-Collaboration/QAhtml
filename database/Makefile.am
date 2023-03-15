AUTOMAKE_OPTIONS = foreign

lib_LTLIBRARIES = \
  libonlproddb.la

INCLUDES = \
  -I$(includedir) \
  -I$(OFFLINE_MAIN)/include \
  -I$(OPT_SPHENIX)/include \
  -I$(ROOTSYS)/include 

pkginclude_HEADERS = \
  OnlProdDB.h \
  OnlProdDBReturnCodes.h \
  OnlProdDBVar.h \
  ScalerDBodbc.h

noinst_HEADERS = \
  OnlProdDBodbc.h \
  Scalers.h

libonlproddb_la_SOURCES = \
  OnlProdDB.cc \
  OnlProdDBodbc.cc \
  OnlProdDBVar.cc \
  ScalerDBodbc.cc \
  Scalers.cc

libonlproddb_la_LDFLAGS = \
  `root-config --glibs`

libonlproddb_la_LIBADD = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib \
  -L$(OPT_SPHENIX)/lib \
  -lonlprodclient \
  -lfun4all \
  -lodbc \
  -lodbc++

noinst_PROGRAMS = \
  testexternals

testexternals_SOURCES = \
  testexternals.cc

testexternals_LDADD = \
  libonlproddb.la

testexternals.cc:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

clean-local:
	rm -f *Dict.*
