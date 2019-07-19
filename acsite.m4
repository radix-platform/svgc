
dnl ============================================================
dnl Display Configuration Headers
dnl 
dnl    configure.in:
dnl       AC_MSG_CFG_PART(<text>)
dnl ============================================================

define(AC_MSG_CFG_PART,[dnl
   AC_MSG_RESULT()
   AC_MSG_RESULT(${TB}$1:${TN})
])dnl


dnl ============================================================
dnl Support for Configuration Headers
dnl 
dnl    configure.in:
dnl       AC_SVGC_HEADLINE(<short-name>, <long-name>,
dnl                              <vers-var>, <vers-file>,
dnl                              <copyright>)
dnl ============================================================

dnl
dnl Line: 2: of following func AC_SVGC_HEADLINE (notice):
dnl ===========================================================
dnl
dnl for version 2.13 we used:
dnl
dnl AC_DIVERT_PUSH(AC_DIVERSION_NOTICE)dnl
dnl
dnl now for version 2.53 we use:
dnl
dnl AC_DIVERT_PUSH(_m4_divert_diversion)dnl
dnl

AC_DEFUN(AC_SVGC_HEADLINE,[dnl
AC_DIVERT_PUSH(_m4_divert_diversion)dnl
if test ".`echo dummy [$]@ | grep help`" = .; then
   ####### bootstrapping shtool
   ac_prog=[$]0
changequote(, )dnl
   ac_srcdir=`echo $ac_prog | sed -e 's%/[^/][^/]*$%%' -e 's%\([^/]\)/*$%\1%'`
changequote([, ])dnl
   test ".$ac_srcdir" = ".$ac_prog" && ac_srcdir=.
   ac_shtool="${CONFIG_SHELL-/bin/sh} $ac_srcdir/shtool"

   ####### нахождение escape последовательностей для выделения
   ####### начала и конца текста, который мы будем выделять
   if test ".$enable_subdir" != .yes; then
      TB=`$ac_shtool echo -n -e %B 2>/dev/null`
      TN=`$ac_shtool echo -n -e %b 2>/dev/null`
   else
      TB=''
      TN=''
   fi

   ####### чтение короткого номера версии продукта (например, "1.0.0")
   $3_STR="`$ac_shtool version -lc -dshort $ac_srcdir/$4`"
   AC_SUBST($3_STR)

   ####### печать заголовка
   if test ".$enable_subdir" != .yes; then
      echo "Configuring ${TB}$1${TN} ($2), Version ${TB}${$3_STR}${TN}"
      echo "$5"
      echo ""
   fi

   ####### чтение шестнадцатеричного номера версии продукта (например, 0x100200)
   $3_HEX="`$ac_shtool version -lc -dhex $ac_srcdir/$4`"
   AC_SUBST($3_HEX)
fi
AC_DIVERT_POP()
])dnl


dnl /////////////////////////////////////////////////

define([AC_FD_MSG],6)dnl Autoconf lossage.
define([AC_FD_CC],5)dnl Autoconf lossage.


dnl ============================================================
dnl Создание и инсталляция svgc требует наличия компилятора
dnl и утилиты install .
dnl ============================================================
AC_DEFUN([SVGC_PROVIDES], [dnl
AC_PROVIDE([AC_PROG_INSTALL])dnl
AC_PROVIDE([AC_PROG_CC])dnl
define([AC_LANG], [C])dnl
# This file is generated from configure.in by Autoconf.  DO NOT EDIT!
])dnl

dnl ============================================================
dnl
dnl Тест версии программы.
dnl =====================
dnl
dnl Аргументы:
dnl ---------
dnl     var                      - переменная окружения, значением
dnl                                которой является имя тестируемой
dnl                                программы;
dnl     namelist                 - список допустимых имен
dnl                                тестируемой программы;
dnl     version-switch           - опция по которой, тестируемая
dnl                                программа выдает номер версии;
dnl     [version-extract-regexp] - выражение, позволяющее из
dnl                                потока, выдаваемого тестируемой
dnl                                программой, выделить номер версии;
dnl     version-glob             - regexp-выражение, задающее
dnl                                допустимые номера версий;
dnl     [, do-if-fail]           - что делать, если версия не допустима .
dnl
dnl Возвращаемое значение:
dnl ---------------------
dnl    ac_prog_version           - номер версии тестируемой программы;
dnl                                значение действительно до следующего
dnl                                вызова AC_PROG_CHECK_VERSION() .
dnl
dnl AC_PROG_CHECK_VERSION(var, namelist, version-switch,
dnl 		          [version-extract-regexp],
dnl                       version-glob [, do-if-fail])
dnl 
AC_DEFUN(AC_CHECK_PROG_VERSION,
[AC_CHECK_PROGS([$1], [$2])
if test -z "[$]$1"; then
   ac_verc_fail=yes
else
   # Found it, now check the version.
   AC_MSG_CHECKING([version of [$]$1])
changequote(<<,>>)dnl
   ac_prog_version=`<<$>>$1 $3 2>&1 ifelse(<<$4>>,,,
                    <<| sed -n 's/^.*patsubst(<<$4>>,/,\/).*$/\1/p'>>)`
   case $ac_prog_version in
      '') ac_prog_version_msg="v. ?.??, bad"; ac_verc_fail=yes;;
      <<$5>>)
changequote([,])dnl
         ac_prog_version_msg="$ac_prog_version, ok";  ac_verc_fail=no;;
      *) ac_prog_version_msg="$ac_prog_version, bad"; ac_verc_fail=yes;;

   esac
   AC_MSG_RESULT([$ac_prog_version_msg])
fi
ifelse([$6],,,
[if test $ac_verc_fail = yes; then
   $6
fi])
])
dnl ============================================================
dnl End of AC_PROG_CHECK_VERSION() .
dnl ============================================================
dnl Old version for previous func .
dnl =============================
dnl
dnl AC_PROG_CHECK_VER(var, namelist, version-switch,
dnl 		      [version-extract-regexp], version-glob [, do-if-fail])
dnl
AC_DEFUN(AC_CHECK_PROG_VER,
[AC_CHECK_PROGS([$1], [$2])
if test -z "[$]$1"; then
   ac_verc_fail=yes
else
   # Found it, now check the version.
   AC_MSG_CHECKING([version of [$]$1])
changequote(<<,>>)dnl
   ac_prog_version=`<<$>>$1 $3 2>&1 ifelse(<<$4>>,,,
                    <<| sed -n 's/^.*patsubst(<<$4>>,/,\/).*$/\1/p'>>)`
   case $ac_prog_version in
      '')
         ac_prog_version="v. ?.??, bad"; ac_verc_fail=yes
         ;;
      <<$5>>)
changequote([,])dnl
         ac_prog_version="$ac_prog_version, ok"; ac_verc_fail=no
         ;;
      *)
         ac_prog_version="$ac_prog_version, bad"; ac_verc_fail=yes
         ;;
   esac
   AC_MSG_RESULT([$ac_prog_version])
fi
ifelse([$6],,,
[if test $ac_verc_fail = yes; then
   $6
fi])
])



dnl ============================================================
dnl  These modifications are to allow for an empty cross
dnl  compiler tree.  In the situation that cross-linking
dnl  is impossible, the variable `cross_linkable' will be
dnl  substituted with "yes".  The vercheck macros are expected
dnl  to have been called already.
dnl ============================================================
AC_DEFUN(AC_PROG_CC_LOCAL,
[AC_BEFORE([$0], [AC_PROG_CPP])dnl

AC_PROG_CC_WORKS_LOCAL
dnl AC_PROG_CC_GNU
AC_PROG_CC
if test $ac_cv_c_compiler_gnu != yes; then
   AC_MSG_ERROR([svgc must be compiled using GNU CC])
fi
])

AC_DEFUN(AC_PROG_CC_WORKS_LOCAL,
[AC_MSG_CHECKING([whether the C compiler ($CC $CFLAGS $LDFLAGS) works])
AC_CACHE_VAL(ac_cv_prog_cc_works,
[AC_LANG_SAVE
AC_LANG_C
AC_TRY_COMPILE([main(){return(0);}], ac_cv_prog_cc_works, ac_cv_prog_cc_cross)
AC_LANG_RESTORE])
AC_MSG_RESULT($ac_cv_prog_cc_works)
if test "$ac_cv_prog_cc_works" = no; then
   cross_linkable=no
   ac_cv_prog_cc_cross=yes
dnl AC_MSG_ERROR([installation or configuration problem: C compiler cannot create executables.])
else
   cross_linkable=yes
fi
AC_CACHE_CHECK(
[whether the C compiler ($CC $CFLAGS $LDFLAGS) is a cross-compiler],
ac_cv_prog_cc_cross, [:])
AC_SUBST(cross_linkable)
cross_compiling=$ac_cv_prog_cc_cross
])


AC_DEFUN(SVGC_PROG_FOO_GNU,
[# Most GNU programs take a -v and spit out some text including
# the word 'GNU'.  Some try to read stdin, so give them /dev/null.
if $1 -o conftest -v </dev/null 2>&1 | grep GNU > /dev/null 2>&1; then
   $2
else
   $3
fi
rm -fr contest*])


AC_DEFUN(SVGC_PROG_BINUTILS,
[# Was a --with-binutils option given?
if test -n "$path_binutils"; then
    # Make absolute; ensure a single trailing slash.
    path_binutils=`(cd $path_binutils; pwd) | sed 's%/*$%/%'`
    CC="$CC -B$path_binutils"
fi
AS=`$CC -print-prog-name=as`
LD=`$CC -print-prog-name=ld`
AR=`$CC -print-prog-name=ar`
AC_SUBST(AR)

# ranlib has to be treated a bit differently since it might not exist at all.
RANLIB=`$CC -print-prog-name=ranlib`
if test $RANLIB = ranlib; then
# This extra check has to happen since gcc simply echos the parameter in
# case it cannot find the value in its own directories.
AC_CHECK_TOOL(RANLIB, ranlib, :)
fi
AC_SUBST(RANLIB)

# Determine whether we are using GNU binutils.
AC_CACHE_CHECK(whether $AS is GNU as, svgc_cv_prog_as_gnu,
[SVGC_PROG_FOO_GNU($AS, svgc_cv_prog_as_gnu=yes, svgc_cv_prog_as_gnu=no)])
rm -f a.out
gnu_as=$svgc_cv_prog_as_gnu

AC_CACHE_CHECK(whether $LD is GNU ld, svgc_cv_prog_ld_gnu,
[SVGC_PROG_FOO_GNU($LD, svgc_cv_prog_ld_gnu=yes, svgc_cv_prog_ld_gnu=no)])
gnu_ld=$svgc_cv_prog_ld_gnu
])



dnl ============================================================
dnl  Test for build_host `ln -s' .
dnl  ============================
dnl
dnl Usage:
dnl -----
dnl    AC_PATH_PROG_LN_S
dnl    AC_SUBST(LN)
dnl    AC_SUBST(LN_S)
dnl
dnl ============================================================
AC_DEFUN(AC_PATH_PROG_LN_S,
[AC_PATH_PROG(LN, ln, no, /usr/local/bin:/usr/bin:/bin:$PATH)
AC_MSG_CHECKING(whether ln -s works on build host)
AC_CACHE_VAL(ac_cv_path_prog_LN_S,
[rm -f conftestdata
if $LN -s X conftestdata 2>/dev/null
then
   rm -f conftestdata
   ac_cv_path_prog_LN_S="$LN -s"
else
   ac_cv_path_prog_LN_S="$LN"
fi])dnl
LN_S="$ac_cv_path_prog_LN_S"
if test "$ac_cv_path_prog_LN_S" = "$LN -s"; then
   AC_MSG_RESULT(yes)
else
   AC_MSG_RESULT(no)
fi
AC_SUBST(LN)dnl
AC_SUBST(LN_S)dnl
])


dnl ============================================================
dnl  Test for build_host `strip -s' .
dnl  ==============================
dnl
dnl Usage:
dnl -----
dnl    AC_PATH_PROG_BUILD_STRIP_S
dnl    AC_SUBST(BUILD_STRIP)
dnl    AC_SUBST(BUILD_STRIP_S)
dnl
dnl ============================================================
AC_DEFUN(AC_PATH_PROG_BUILD_STRIP_S,
[AC_PATH_PROG(BUILD_STRIP, strip, no, /usr/local/bin:/usr/bin:/bin:$PATH)
AC_MSG_CHECKING(whether strip -s works on build host)
AC_CACHE_VAL(ac_cv_path_prog_STRIP_S,
[rm -f conftest
cat > conftest.c <<EOF

int main()
{
   char *s = "strip";
   
   return( 0 );
}

EOF
$BUILD_CC -g -O2 -o conftest conftest.c
if $BUILD_STRIP -s conftest 2>/dev/null
then
   rm -f conftest conftest.c
   ac_cv_path_prog_STRIP_S="$BUILD_STRIP -s"
else
   rm -f conftest conftest.c
   ac_cv_path_prog_STRIP_S="$BUILD_STRIP"
fi])dnl
BUILD_STRIP_S="$ac_cv_path_prog_STRIP_S"
if test "$ac_cv_path_prog_STRIP_S" = "$BUILD_STRIP -s"; then
   AC_MSG_RESULT(yes)
else
   AC_MSG_RESULT(no)
fi
AC_SUBST(BUILD_STRIP)dnl
AC_SUBST(BUILD_STRIP_S)dnl
])





AC_DEFUN(AC_BUILD_CYGSVGC_DLL,
[AC_MSG_CHECKING(whether necessary to build svgc.dll for target host)
AC_CACHE_VAL(ac_cv_build_svgc_dll,
[
case "$host_os" in
   linux*)
      dnl ==================== linux-gnu =======================
      ac_cv_build_svgc_dll=no
      ;;
   cygwin*)
      dnl ====================== cygwin ========================
      ac_cv_build_svgc_dll=yes
      ;;
   *)
      dnl ===================== default ========================
      ac_cv_build_svgc_dll=no
      ;;
esac
])dnl
if test "$ac_cv_build_svgc_dll" = yes; then
   AC_MSG_RESULT(yes)
   CYGSVGC_DLL="svgc.dll"
   CYGSVGC_DEF="svgc.def"
   LIBSVGC_LD_FLAG=-lsvgc.dll
else
   AC_MSG_RESULT(no)
   CYGSVGC_DLL=
   CYGSVGC_DEF=
   LIBSVGC_LD_FLAG=-lsvgc
fi
AC_SUBST(CYGSVGC_DLL)dnl
AC_SUBST(CYGSVGC_DEF)dnl
AC_SUBST(LIBSVGC_LD_FLAG)dnl
])


