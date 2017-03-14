#!/bin/sh

# have automake do an initial population iff necessary
if [ ! -e config.guess -o ! -e config.sub -o ! -e install-sh -o ! -e missing ]; then
    touch NEWS README AUTHORS ChangeLog
    autoreconf -i
    autoheader -f
    touch stamp-h
    libtoolize || glibtoolize
    aclocal -I m4
    autoconf -f
    automake --add-missing --copy
else
    automake --add-missing --copy
    autoreconf -f
fi
echo bootstrap Done.
