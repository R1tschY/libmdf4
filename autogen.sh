#!/bin/bash

SRCDIR=`dirname $0`
if [ -z "$SRCDIR" ]; then
  SRCDIR=.
fi

aclocal --install || exit 1
autoreconf --force --install || exit 1

if [ "$NOCONFIGURE" = "" ]; then
  $SRCDIR/configure "$@" || exit 1

  if [ "$1" = "--help" ]; then 
    exit 0 
  else
    echo "Now type \`make' to compile"
  fi
fi
