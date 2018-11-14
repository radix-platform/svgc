#!/bin/sh

cd ..

libtoolize --force --copy
aclocal --install -I m4 --force
autoheader
automake --gnu --add-missing --copy --force-missing
autoconf --force
