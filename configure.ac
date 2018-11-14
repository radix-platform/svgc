
dnl ============================================================
dnl  Process this file with autoconf to produce
dnl  a configure script.
dnl ============================================================

AC_PREREQ(2.53)dnl           dnl Minimum Autoconf version required.
AC_INIT([svgc], [1.0.8], [kx@radix.pro])


AC_SVGC_HEADLINE(dnl
:

svgc, Andrey V.Kosteltsev svgc library, dnl
_SVGC_VERSION, version.c, dnl
[Copyright (c) 2009-2010 Andrey V.Kosteltsev])


dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl $$                                                        $$
dnl $$ PART: Init Automake environment                        $$
dnl $$                                                        $$
dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl ============================================================
AC_MSG_CFG_PART(Init Automake environment)

AM_INIT_AUTOMAKE([subdir-objects])

AM_CONFIG_HEADER([config.h])

AC_PREFIX_DEFAULT(/usr/local)


dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl $$                                                        $$
dnl $$ PART: Test for GNU Shtool                              $$
dnl $$                                                        $$
dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl ============================================================
AC_MSG_CFG_PART(Test for GNU Shtool)
AC_PROG_SHTOOL


AC_CHECK_PROGS(CC, ${ac_tool_prefix}gcc ${ac_tool_prefix}cc)

AC_PROG_CC_LOCAL




dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl $$                                                        $$
dnl $$ PART: Test for Build Tools                             $$
dnl $$                                                        $$
dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl ============================================================
AC_MSG_CFG_PART(Test for Build Tools)

dnl ============================================================
dnl  Проверка версий программ
dnl ============================================================
dnl AC_REQUIRE([AC_CHECK_TOOL_PREFIX])dnl

dnl ============================================================
dnl Check GCC ( AC_CHECK_PROG_VERSION() находится в acsite.m4 )
dnl ============================================================
AC_CHECK_PROG_VERSION(CC, ${ac_tool_prefix}gcc ${ac_tool_prefix}cc, -v,
  [version \([egcygnustpi-]*[0-9.]*\)],
  [*gcc-2.9[5-9].*|*2.8.[1-9]*|*2.9|*2.9.[0-9]*|2.9[5-9]*|3.[0-9]*|[4-7].[0-9]*|cygnus-2.9[1-9]*|gcc-2.9[5-9]|gcc-2.1[0-9][0-9]|sgicc-*],
  critic_missing=gcc)
dnl
dnl set gcc_version - variable
dnl ==========================
dnl если использовать AC_CHECK_PROG_VER() то с помощью команды
dnl sed 's/,.*//' надо удалить ", ok" в конце $ac_prog_version
dnl следующим образом:
dnl    gcc_version=`echo $ac_prog_version | sed 's/,.*//'`  
dnl ============================================================
gcc_version=$ac_prog_version
AC_SUBST(gcc_version)


CCVERSION=`$CC -v 2>&1 | sed -n 's/gcc version //p'`
AC_SUBST(CCVERSION)

dnl ============================================================
dnl  We set BUILD_CC variable in any case .
dnl ============================================================
dnl if test $host != $build; then
  AC_CHECK_PROGS(BUILD_CC, gcc cc)
dnl fi
AC_SUBST(cross_compiling)

AC_PROG_CPP


dnl ============================================================
dnl Check MAKE
dnl ============================================================
AC_CHECK_PROG_VERSION(MAKE, make gmake gnumake, --version,
  [GNU Make[^0-9]*\([0-9][0-9.]*\)],
  [3.79* | 3.[89]* | 4.[0-9]*], critic_missing="$critic_missing make")
dnl
dnl set make_version - variable
dnl ==========================
dnl если использовать AC_CHECK_PROG_VER() то с помощью команды
dnl sed 's/,.*//' надо удалить ", ok" в конце $ac_prog_version
dnl следующим образом:
dnl    make_version=`echo $ac_prog_version | sed 's/,.*//'`  
dnl ============================================================
make_version=$ac_prog_version
AC_SUBST(make_version)



if test -n "$critic_missing"; then
AC_MSG_ERROR([
********   These critical programs are missing or too old: $critic_missing
********   Check the INSTALL file for required versions or
********   check the environment variable PATH for correct
********   destination to used programs.])
fi



AC_PATH_PROG(LIBTOOL, libtool, no,
                      /usr/local/bin:/usr/bin:/bin:$PATH)


dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl $$                                                        $$
dnl $$ PART: Test for Auxiliary (my be version sensitive)     $$
dnl $$       programs                                         $$
dnl $$                                                        $$
dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl ============================================================
AC_MSG_CFG_PART(Test for Auxiliary (my be version sensitive) programs)

dnl ============================================================
dnl  Locate tools( on build machine! ) .
dnl                =================
dnl ============================================================
AC_PATH_PROG_LN_S
AC_SUBST(LN)
AC_SUBST(LN_S)

dnl  Нам нужен исполняемый файл pwd. Встроенный "pwd -P" нас
dnl  не устраивает( из соображений переносимости ) .
AC_PATH_PROG(PWD_P, pwd, no)
if test "$PWD_P" = no; then
   AC_MSG_ERROR(********   A pwd binary could not be found.)
fi

AC_PATH_PROGS(SED, sed gsed, no, /usr/local/bin:/usr/bin:/bin:$PATH)
if test "$SED" != no; then
AC_CHECK_PROG_VERSION(SED, $SED, --version,
  [GNU sed version \([0-9]*\.[0-9.]*\)],
  [3.0[2-9]*|3.[1-9]*|[4-9]*],
  SED=: aux_missing="$aux_missing sed")
sed_version=$ac_prog_version
fi
AC_SUBST(sed_version)

test -n "$aux_missing" && AC_MSG_WARN([
********   These auxiliary programs are missing or too old: $aux_missing
********   some features will be disabled.
********   Check the INSTALL file for required versions.])


dnl 
dnl Remove TAR option '-o' for allow long file names in DIST archive.
dnl Standard tar.m4 from aclocal-1.9 package probvides following
dnl 'am__tar' command:
dnl    ${AMTAR} chof - "$$tardir"
dnl and uses '-chof' options for backward compatibility.
dnl 
am__tar='${AMTAR} chf - "$$tardir"'
AC_SUBST(am__tar)

AC_PATH_PROGS(TAR, tar gtar, no, /usr/local/bin:/usr/bin:/bin:$PATH)


AC_PATH_PROG(BASH, bash, no)
if test "$BASH" != no &&
   $BASH -c 'test "$BASH_VERSINFO" \
             && test "$BASH_VERSINFO" -ge 2 >&/dev/null'; then
   svgc_cv_have_bash2=yes
else
   svgc_cv_have_bash2=no
fi
AC_SUBST(svgc_cv_have_bash2)


AC_PATH_PROGS(AWK, gawk awk, no, /usr/local/bin:/usr/bin:/bin:$PATH)

AC_PATH_PROG(BISON, bison, no, /usr/local/bin:/usr/bin:/bin:$PATH)




dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl $$                                                        $$
dnl $$ PART: Creating LIBTOOL.                                $$
dnl $$                                                        $$
dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl ============================================================
AC_MSG_CFG_PART(Creating LIBTOOL)


dnl Checks for compiler output filename suffixes.
AC_OBJEXT
AC_EXEEXT

dnl Check for build configuration.
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL

dnl
dnl значение следующей переменной равно l, так как мы хотим
dnl использовать LIBTOOL; в противном случае мы написали бы
dnl SVGC_LIBTOOL_SUFFIX_PREFIX=
dnl
dnl *** вообще, это полезно вынести в опции ./configure ***
dnl
SVGC_LIBTOOL_SUFFIX_PREFIX=l
AC_SUBST(SVGC_LIBTOOL_SUFFIX_PREFIX)


dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl $$                                                        $$
dnl $$ PART: Whether necessary to build Bill Gates's DLL?     $$
dnl $$                                                        $$
dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl ============================================================
AC_MSG_CFG_PART(Whether necessary to build Bill Gates's DLL?)

AC_BUILD_CYGSVGC_DLL
AC_SUBST(CYGSVGC_DLL)
AC_SUBST(LIBSVGC_LD_FLAG)

dnl ============================================================
dnl ============================================================
dnl ============================================================



dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl $$                                                        $$
dnl $$ PART: Build Parameters                                 $$
dnl $$                                                        $$
dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl ============================================================
AC_MSG_CFG_PART(Build Parameters)

dnl
dnl  Check for system header files.
dnl  =============================
dnl  /* GetText это проверил. А мы делаем для себя. */
AC_HEADER_STDC
AC_HEADER_TIME
AC_CHECK_HEADERS(unistd.h stdlib.h stdio.h string.h)




libsvgc_ac_config=yes
AC_SUBST(libsvgc_ac_config)

sinclude([svgc.m4])

dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl $$                                                        $$
dnl $$ PART: OUTPUT Substitution                              $$
dnl $$                                                        $$
dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl ============================================================
dnl ============================================================
AC_MSG_CFG_PART(OUTPUT)


AC_CONFIG_FILES([
Makefile
svgc-config/Makefile
svgc-config/make-svgc-config
svgc-config/head
svgc-config/tail
src/Makefile
])
AC_OUTPUT

chmod 0755 svgc-config/make-svgc-config

if test -f "Makefile"; then
    echo ""
    echo "Now please type:"
    echo "   \`${TB}make${TN}'           to compile,"
    echo "   \`${TB}make install${TN}'   to make and install ${TB}svgc${TN} library,"
    echo "   \`${TB}make dist${TN}'      to create distributable tarball, or"
    echo "   \`${TB}make distclean${TN}' to clean befor configure for another target."
    echo "Good luck."
    echo ""
fi
