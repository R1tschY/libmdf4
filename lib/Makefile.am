#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
lib_LTLIBRARIES = libmdf4.la

#######################################
# Build information for each library

# Sources for libmdf
libmdf4_la_SOURCES = libmdf4.cpp libmdf4.h \
                   file.cpp file.h \
                   channel.cpp channel.h \
                   channelconversation.cpp channelconversation.h \
                   channelgroup.cpp channelgroup.h \
                   datagroup.cpp datagroup.h \
                   sourceinformation.cpp sourceinformation.h \
                   block.cpp block.h \
                   detail/rawfile.cpp detail/rawfile.h \
                   detail/mdf4.cpp detail/mdf4.h \
                   detail/macros.h \
                   detail/xml.cpp detail/xml.h

# These files will end up in the install include directory
# For example, /usr/include
include_HEADERS = libmdf4.h file.h channel.h channelconversation.h \
                  datagroup.h sourceinformation.h detail/rawfile.h \
                  channelgroup.h \
                  block.h \
                  detail/mdf4.h detail/macros.h

libmdf4_la_LDFLAGS = 

libmdf4_la_CPPFLAGS = \
  -std=gnu++0x \
  -Wall \
  -Wextra \
  -Wshadow \
  -Wnon-virtual-dtor \
  -Wold-style-cast \
  -Wcast-align \
  -Wunused \
  -Woverloaded-virtual \
  -Wundef \
  -Wcast-qual \
  -Wconversion

