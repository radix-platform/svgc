#!/bin/sh

cd ..

aclocal
# --acdir=`svgc-config --acdir`
autoconf


echo -n -e "\\033[1m"
echo ""
echo "Cteated: aclocal.m4 and configure script"
echo ""
echo -n -e "\\033[m"
