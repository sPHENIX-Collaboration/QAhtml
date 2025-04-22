AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = \
  -I$(includedir) \
  -isystem$(OFFLINE_MAIN)/include \
  -isystem$(ROOTSYS)/include 

lib_LTLIBRARIES = \
  libqadrawglobalqa.la

# Tell the linker where to find RooFit libs
AM_LDFLAGS = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib \
  -L$(OFFLINE_MAIN)/lib64 \
  -L$(ROOTSYS)/lib

AM_CPPFLAGS = \
  -I$(includedir) \
  -isystem$(OFFLINE_MAIN)/include \
  -isystem$(ROOTSYS)/include

libqadrawglobalqa_la_LIBADD = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib \
  -lqadrawclient \
  -lqadrawdb \
  -lphool \
  -lffamodules \
  -lfun4all \
  -lcalo_io

globalqaincludedir=$(pkgincludedir)/globalqa

globalqainclude_HEADERS = \
  GlobalQADraw.h

libqadrawglobalqa_la_SOURCES = \
  GlobalQADraw.cc

noinst_PROGRAMS = \
  testexternals

testexternals_SOURCES = \
  testexternals.cc

testexternals_LDADD = \
  libqadrawglobalqa.la \
  -lRooFit \
  -lRooFitCore \
  -lMinuit2


testexternals.cc:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

clean-local:
	rm -f $(BUILT_SOURCES)

