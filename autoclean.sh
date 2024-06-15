#!/bin/sh

test ! -f Makefile || make distclean

rm -f \
   aclocal.m4 \
   configure \
   lib/config.h.in \
   Makefile \
   build-aux/compile \
   build-aux/depcomp \
   build-aux/install-sh \
   build-aux/missing \
   build-aux/config.guess \
   build-aux/config.sub
