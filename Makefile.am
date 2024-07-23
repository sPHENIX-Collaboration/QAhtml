AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = -I$(srcdir)/client

include_HEADERS =

#database has to come before client since client
# uses include from database
SUBDIRS = \
  client \
  database \
  subsystems
