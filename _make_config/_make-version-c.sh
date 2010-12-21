#!/bin/sh

#
# ������ ������ ������ ���� ../version.h, ������������� ������� �
# ������� ../version.c � ������� ������� GNU Shtool.
#
# GNU Shtool ����������� � �������� ������ � ������� �������:
#
#   shtoolize -q all
#
# ������ ������� ������ �� �������, � ������ ������� ������
# shtool.

cd ..

VERSION=`sed -n -e 's/^#define VERSION "\([^"]*\)"/\1/p' < version.h`

./shtool version -l c -n _svgc -p _svgc_ -s $VERSION version.c
