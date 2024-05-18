AUTOMAKE_OPTIONS = foreign

lib_LTLIBRARIES = \
  libqadrawdb.la

AM_CPPFLAGS = \
  -I$(includedir) \
  -isystem$(OFFLINE_MAIN)/include \
  -isystem$(OPT_SPHENIX)/include \
  -isystem$(ROOTSYS)/include

pkginclude_HEADERS = \
  QADrawDB.h \
  QADrawDBReturnCodes.h \
  QADrawDBVar.h \
  ScalerDBodbc.h

noinst_HEADERS = \
  QADrawDBodbc.h \
  Scalers.h

libqadrawdb_la_SOURCES = \
  QADrawDB.cc \
  QADrawDBodbc.cc \
  QADrawDBVar.cc \
  ScalerDBodbc.cc \
  Scalers.cc

libqadrawdb_la_LDFLAGS = \
  `root-config --glibs`

libqadrawdb_la_LIBADD = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib \
  -L$(OPT_SPHENIX)/lib \
  -lqadrawclient \
  -lfun4all \
  -lodbc \
  -lodbc++

noinst_PROGRAMS = \
  testexternals

testexternals_SOURCES = \
  testexternals.cc

testexternals_LDADD = \
  libqadrawdb.la

testexternals.cc:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

clean-local:
	rm -f *Dict.*
