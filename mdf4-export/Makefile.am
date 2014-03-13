#######################################
# The list of executables we are building seperated by spaces
# the 'bin_' indicates that these build products will be installed
# in the $(bindir) directory. For example /usr/bin
bin_PROGRAMS=mdf4-export

#######################################
# Build information for each executable. The variable name is derived
# by use the name of the executable with each non alpha-numeric character is
# replaced by '_'. So a.out becomes a_out and the appropriate suffex added.
# '_SOURCES' for example.

# Sources for the a.out 
mdf4_export_SOURCES= main.cpp

# Linker options for a.out
mdf4_export_LDFLAGS = $(top_srcdir)/lib/libmdf4.la

# Compiler options for a.out
mdf4_export_CPPFLAGS = -I$(top_srcdir)/lib  -std=gnu++0x
