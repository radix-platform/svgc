#!/bin/sh

cd ..

libtoolize --force --copy
aclocal
autoheader
automake --gnu --add-missing --copy
autoconf
