AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = \
  -I$(includedir) \
  -isystem$(OFFLINE_MAIN)/include \
  -isystem$(OFFLINE_MAIN)/rootmacros \
  -isystem$(ROOTSYS)/include 

lib_LTLIBRARIES = \
  libqadrawkfparticle.la

libqadrawkfparticle_la_LIBADD = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib \
  -lqadrawclient \
  -lqadrawdb

libqadrawkfparticle_la_LIBADD = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib \
  -lqadrawclient \
  -lqadrawdb 

kfparticleincludedir=$(pkgincludedir)/kfparticle

kfparticleinclude_HEADERS = \
  KFParticleDraw.h

libqadrawkfparticle_la_SOURCES = \
  KFParticleDraw.cc

noinst_PROGRAMS = \
  testexternals

testexternals_SOURCES = \
  testexternals.cc

testexternals_LDADD = \
  libqadrawkfparticle.la

testexternals.cc:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

clean-local:
	rm -f $(BUILT_SOURCES)
