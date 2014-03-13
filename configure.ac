dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(libmdf4, 0.1, r1tschy@yahoo.de)
AC_CONFIG_MACRO_DIR([m4])

AC_CANONICAL_SYSTEM

AM_INIT_AUTOMAKE([foreign no-dist-gzip dist-bzip2 -Wall -Werror])
AC_ISC_POSIX
AC_PROG_CXX
AC_PROG_INSTALL

dnl this allows us specify individual linking flags for each target
AM_PROG_CC_C_O

AM_PROG_AR

AC_HEADER_STDC
AC_TYPE_SIZE_T

CXXFLAGS="-g3 -fmessage-length=0 -Wall"

dnl Initialize Libtool
LT_INIT

dnl Check if Libtool is present
dnl Libtool is used for building share libraries 
AC_PROG_LIBTOOL

################################################################################
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES(Makefile
                mdf4-info/Makefile
                mdf4-export/Makefile
                lib/Makefile)
                
AC_OUTPUT

