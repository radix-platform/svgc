#!/bin/sh

#
# Данный скрипт читает файл ../version.h, редактируемый вручную и
# создает ../version.c с помощью утилиты GNU Shtool.
#
# GNU Shtool добавляется в исходное дерево с помощью команды:
#
#   shtoolize -q all
#
# данная команда ничего не трогает, а просто создает скрипт
# shtool.

cd ..

VERSION=`sed -n -e 's/^#define VERSION "\([^"]*\)"/\1/p' < version.h`

./shtool version -l c -n _svgc -p _svgc_ -s $VERSION version.c
