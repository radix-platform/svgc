dnl #
dnl # /usr/share/aclocal/svgc.m4
dnl #
dnl # Configure paths for svgc
dnl # Andrew V.Kosteltsev

dnl ============================================================
dnl
dnl  Synopsis:
dnl     AC_CHECK_SVGC([MIN-VERSION [,                  # minimum svgc version, e.g. 1.0.1
dnl                           DEFAULT-WITH-SVGC [,     # default value for --with-svgc option
dnl                           DEFAULT-WITH-SVGC-TEST [,# default value for --with-svgc-test option
dnl                           EXTEND-VARS [,                  # whether CFLAGS/LDFLAGS/etc are extended
dnl                           ACTION-IF-FOUND [,              # action to perform if svgc was found
dnl                           ACTION-IF-NOT-FOUND             # action to perform if svgc was not found
dnl                          ]]]]]])
dnl  Examples:
dnl     AC_CHECK_SVGC(1.0.1)
dnl     AC_CHECK_SVGC(1.0.1,,,no,CFLAGS="$CFLAGS -DHAVE_SVGC $SVGC_CFLAGS")
dnl     AC_CHECK_SVGC(1.0.1,yes,yes,yes,CFLAGS="$CFLAGS -DHAVE_SVGC")
dnl
dnl
dnl  If you have to change prefix returned by svgc-config script or change
dnl  location of svgc-config, you may set environment variable SVGC_CONFIG,
dnl  for example:
dnl
dnl  # export SVGC_CONFIG="svgc-config --prefix=/usr/local"
dnl  # export SVGC_CONFIG="/usr/bin/svgc-config --prefix=/usr/local"
dnl
dnl ============================================================
dnl
dnl ============================================================
dnl  auxilliary macros
dnl ============================================================
AC_DEFUN([_AC_SVGC_ERROR], [dnl
AC_MSG_RESULT([*FAILED*])
cat <<EOT | sed -e 's/^[[ 	]]*/ | /' -e 's/>>/  /' 1>&2
$1
EOT
exit 1
])

AC_DEFUN([_AC_SVGC_VERBOSE], [dnl
if test ".$verbose" = .yes; then
    AC_MSG_RESULT([  $1])
fi
])

dnl ============================================================
dnl  the user macro
dnl ============================================================
AC_DEFUN([AC_CHECK_SVGC], [dnl
dnl
dnl ============================================================
dnl  prerequisites
dnl ============================================================
AC_REQUIRE([AC_PROG_CC])dnl
AC_REQUIRE([AC_PROG_CPP])dnl
dnl
dnl ============================================================
dnl  set SVGC_CONFIG variable
dnl ============================================================
if test -z "$SVGC_CONFIG"; then
  SVGC_CONFIG='svgc-config'
fi
dnl
SVGC_CPPFLAGS=''
SVGC_CFLAGS=''
SVGC_LDFLAGS=''
SVGC_LIBS=''
AC_SUBST(SVGC_CPPFLAGS)
AC_SUBST(SVGC_CFLAGS)
AC_SUBST(SVGC_LDFLAGS)
AC_SUBST(SVGC_LIBS)
dnl
dnl ============================================================
dnl  command line options
dnl ============================================================
AC_MSG_CHECKING(for svgc)
_AC_SVGC_VERBOSE([])
AC_ARG_WITH(svgc,dnl
[  --with-svgc[=ARG]       Build with svgc Library  (default=]ifelse([$2],,yes,$2)[)],dnl
,dnl
with_svgc="ifelse([$2],,yes,$2)"
)dnl
AC_ARG_WITH(svgc-test,dnl
[  --with-svgc-test      Perform svgc Sanity Test (default=]ifelse([$3],,yes,$3)[)],dnl
,dnl
with_svgc_test="ifelse([$3],,yes,$3)"
)dnl
_AC_SVGC_VERBOSE([+ Command Line Options:])
_AC_SVGC_VERBOSE([    o --with-svgc=$with_svgc])
_AC_SVGC_VERBOSE([    o --with-svgc-test=$with_svgc_test])
dnl
dnl ============================================================
dnl  configuration
dnl ============================================================
if test ".$with_svgc" != .no; then
    svgc_subdir=no
    svgc_subdir_opts=''
    case "$with_svgc" in
        subdir:* )
            svgc_subdir=yes
            changequote(, )dnl
            svgc_subdir_opts=`echo $with_svgc | sed -e 's/^subdir:[^ 	]*[ 	]*//'`
            with_svgc=`echo $with_svgc | sed -e 's/^subdir:\([^ 	]*\).*$/\1/'`
            changequote([, ])dnl
            ;;
    esac
    svgc_version=""
    svgc_location=""
    svgc_type=""
    svgc_cppflags=""
    svgc_cflags=""
    svgc_ldflags=""
    svgc_libs=""
    if test ".$with_svgc" = .yes; then
        #   via config script in $PATH
        changequote(, )dnl
        svgc_version=`($SVGC_CONFIG --version) 2>/dev/null |\
                      sed -e 's/^.*\([0-9]\.[0-9]*[ab.][0-9]*\).*$/\1/'`
        changequote([, ])dnl
        if test ".$svgc_version" != .; then
            svgc_location=`$SVGC_CONFIG --prefix`
            svgc_type='installed'
            svgc_cppflags=`$SVGC_CONFIG --cppflags`
            svgc_cflags=`$SVGC_CONFIG --cflags`
            svgc_ldflags=`$SVGC_CONFIG --ldflags`
            svgc_libs=`$SVGC_CONFIG --libs`
        fi
    elif test -d "$with_svgc"; then
        with_svgc=`echo $with_svgc | sed -e 's;/*$;;'`
        svgc_found=no
        #   via config script under a specified directory
        #   (a standard installation, but not a source tree)
        if test ".$svgc_found" = .no; then
            for _dir in $with_svgc/bin $with_svgc; do
                if test -f "$_dir/svgc-config"; then
                    test -f "$_dir/svgc-config.in" && continue # svgc-config in source tree!
                    changequote(, )dnl
                    svgc_version=`($_dir/svgc-config --version) 2>/dev/null |\
                                  sed -e 's/^.*\([0-9]\.[0-9]*[ab.][0-9]*\).*$/\1/'`
                    changequote([, ])dnl
                    if test ".$svgc_version" != .; then
                        svgc_location=`$_dir/svgc-config --prefix`
                        svgc_type="installed"
                        svgc_cppflags=`$_dir/svgc-config --cppflags`
                        svgc_cflags=`$_dir/svgc-config --cflags`
                        svgc_ldflags=`$_dir/svgc-config --ldflags`
                        svgc_libs=`$_dir/svgc-config --libs`
                        svgc_found=yes
                        break
                    fi
                fi
            done
        fi
    fi
    _AC_SVGC_VERBOSE([+ Determined Location:])
    _AC_SVGC_VERBOSE([    o path: $svgc_location])
    _AC_SVGC_VERBOSE([    o type: $svgc_type])
    if test ".$svgc_version" = .; then
        if test ".$with_svgc" != .yes; then
             _AC_SVGC_ERROR([dnl
             Unable to locate svgc under $with_svgc.
             Please specify the correct path to either a svgc installation tree
             (use --with-svgc=DIR if you used --prefix=DIR for installing svgc in
             the past).])
        else
             _AC_SVGC_ERROR([dnl
             Unable to locate svgc in any system-wide location (see \$PATH).
             Please specify the correct path to either a svgc installation tree
             (use --with-svgc=DIR if you used --prefix=DIR for installing svgc in
             the past, or set the SVGC_CONFIG environment variable to the full path
             to svgc-config).])
        fi
    fi
    dnl ========================================================
    dnl  Check whether the found version is sufficiently new
    dnl ========================================================
    _req_version="ifelse([$1],,1.0.0,$1)"
    for _var in svgc_version _req_version; do
        eval "_val=\"\$${_var}\""
        _major=`echo $_val | sed 's/\([[0-9]]*\)\.\([[0-9]]*\)\([[ab.]]\)\([[0-9]]*\)/\1/'`
        _minor=`echo $_val | sed 's/\([[0-9]]*\)\.\([[0-9]]*\)\([[ab.]]\)\([[0-9]]*\)/\2/'`
        _rtype=`echo $_val | sed 's/\([[0-9]]*\)\.\([[0-9]]*\)\([[ab.]]\)\([[0-9]]*\)/\3/'`
        _micro=`echo $_val | sed 's/\([[0-9]]*\)\.\([[0-9]]*\)\([[ab.]]\)\([[0-9]]*\)/\4/'`
        case $_rtype in
            "a" ) _rtype=0 ;;
            "b" ) _rtype=1 ;;
            "." ) _rtype=2 ;;
        esac
        _hex=`echo dummy | awk '{ printf("%d%02d%1d%02d", major, minor, rtype, micro); }' \
              "major=$_major" "minor=$_minor" "rtype=$_rtype" "micro=$_micro"`
        eval "${_var}_hex=\"\$_hex\""
    done
    _AC_SVGC_VERBOSE([+ Determined Versions:])
    _AC_SVGC_VERBOSE([    o existing: $svgc_version -> 0x$svgc_version_hex])
    _AC_SVGC_VERBOSE([    o required: $_req_version -> 0x$_req_version_hex])
    _ok=0
    if test ".$svgc_version_hex" != .; then
        if test ".$_req_version_hex" != .; then
            if test $svgc_version_hex -ge $_req_version_hex; then
                _ok=1
            fi
        fi
    fi
    if test ".$_ok" = .0; then
        _AC_SVGC_ERROR([dnl
        Found svgc version $svgc_version, but required at least version $_req_version.
        Upgrade svgc under $svgc_location to $_req_version or higher first, please.])
    fi
    dnl ========================================================
    dnl  Perform svgc Sanity Compile Check
    dnl ========================================================
    if test ".$with_svgc_test" = .yes; then
        _ac_save_CPPFLAGS="$CPPFLAGS"
        _ac_save_CFLAGS="$CFLAGS"
        _ac_save_LDFLAGS="$LDFLAGS"
        _ac_save_LIBS="$LIBS"
        CPPFLAGS="$CPPFLAGS $svgc_cppflags"
        CFLAGS="$CFLAGS $svgc_cflags"
        LDFLAGS="$LDFLAGS $svgc_ldflags"
        LIBS="$LIBS $svgc_libs"
        _AC_SVGC_VERBOSE([+ Test Build Environment:])
        _AC_SVGC_VERBOSE([    o CPPFLAGS=\"$CPPFLAGS\"])
        _AC_SVGC_VERBOSE([    o CFLAGS=\"$CFLAGS\"])
        _AC_SVGC_VERBOSE([    o LDFLAGS=\"$LDFLAGS\"])
        _AC_SVGC_VERBOSE([    o LIBS=\"$LIBS\"])
        cross_compile=no
        define(_code1, [dnl

#include <stdlib.h>  /*   exit( 3 ) */
#include <stdio.h>   /* printf( 3 ) */

#include <svgc.h>

        ])
        define(_code2, [dnl

struct __svg_sheet   *sheet;
struct __svg_line    *line;
struct __svg_cmd     *cmd;
struct __svg_cmds    *cmds;

struct __svg_scene *scene;

double width  = 500.0;
double height = 400.0;
double lm = 10.0;
double rm = 10.0;
double tm = 10.0;
double bm = 10.0;

int main( void )
{
   FILE  *fp;

   sheet = (struct __svg_sheet *)__svg_new_sheet( width, height,
                                                  lm, rm, tm, bm );
   if( !sheet )
   {
      printf( "Unable to create sheet\n" );
   }
   (void)__svg_set_stroke_width( (struct __svg_node *)sheet, 0.5 );

   scene = __svg_new_scene( sheet );
   if( !scene )
   {
      printf( "Unable to create scene\n" );
   }

   __svg_set_scene_stroke( "black" );
   __svg_set_scene_stroke_width( 0.5 );
   __svg_set_scene_stroke_linecap( LINECAP_SQUARE );


   line = (struct __svg_line *)__svg_new_line( lm, 200, width - rm, 200 );
   (void)__svg_scene_add_node( scene, (struct __svg_node *)line );


   __svg_paint_scene( scene, 1, "test.svg" );

   __svg_free_scene( scene );

   if( !(fp = fopen( "conftestval", "w" )) )
   {
      return( 1 );
   }

   fprintf( fp, "yes" );
   fclose( fp );

   unlink( "test.svg" );

   return( 0 );
}
        ])
        _AC_SVGC_VERBOSE([+ Performing Sanity Checks:])
        _AC_SVGC_VERBOSE([    o pre-processor test])
        AC_TRY_CPP(_code1, _ok=yes, _ok=no)
        if test ".$_ok" != .yes; then
            _AC_SVGC_ERROR([dnl
            Found svgc $svgc_version under $svgc_location, but
            was unable to perform a sanity pre-processor check. This means
            the svgc header svgc.h was not found.
            We used the following build environment:
            >> CPP="$CPP"
            >> CPPFLAGS="$CPPFLAGS"
            See config.log for possibly more details.])
        fi
        _AC_SVGC_VERBOSE([    o link check])
        AC_TRY_LINK(_code1, _code2, _ok=yes, _ok=no)
        if test ".$_ok" != .yes; then
            _AC_SVGC_ERROR([dnl
            Found svgc $svgc_version under $svgc_location, but
            was unable to perform a sanity linker check. This means
            the svgc library libsvgc.a was not found.
            We used the following build environment:
            >> CC="$CC"
            >> CFLAGS="$CFLAGS"
            >> LDFLAGS="$LDFLAGS"
            >> LIBS="$LIBS"
            See config.log for possibly more details.])
        fi
        _AC_SVGC_VERBOSE([    o run-time check])
        AC_TRY_RUN(_code1 _code2, _ok=`cat conftestval`, _ok=no, _ok=no)
        if test ".$_ok" != .yes; then
            if test ".$_ok" = .no; then
                _AC_SVGC_ERROR([dnl
                Found svgc $_svgc_version under $svgc_location, but
                was unable to perform a sanity execution check. This usually
                means that the svgc shared library libsvgc.so is present
                but \$LD_LIBRARY_PATH is incomplete to execute a svgc test.
                In this case either disable this test via --without-svgc-test,
                or extend \$LD_LIBRARY_PATH.
                We used the following build environment:
                >> CC="$CC"
                >> CFLAGS="$CFLAGS"
                >> LDFLAGS="$LDFLAGS"
                >> LIBS="$LIBS"
                See config.log for possibly more details.])
            else
                _AC_SVGC_ERROR([dnl
                Found svgc $svgc_version under $svgc_location, but
                was unable to perform a sanity run-time check. This usually
                means that the svgc library failed to work and possibly
                caused a core dump in the test program. In this case it
                is strongly recommended that you re-install svgc.
                We used the following build environment:
                >> CC="$CC"
                >> CFLAGS="$CFLAGS"
                >> LDFLAGS="$LDFLAGS"
                >> LIBS="$LIBS"
                See config.log for possibly more details.])
            fi
        fi
        _extendvars="ifelse([$4],,yes,$4)"
        if test ".$_extendvars" != .yes; then
            CPPFLAGS="$_ac_save_CPPFLAGS"
            CFLAGS="$_ac_save_CFLAGS"
            LDFLAGS="$_ac_save_LDFLAGS"
            LIBS="$_ac_save_LIBS"
        fi
    else
        _extendvars="ifelse([$4],,yes,$4)"
        if test ".$_extendvars" = .yes; then
            if test ".$svgc_subdir" = .yes; then
                CPPFLAGS="$CPPFLAGS $svgc_cppflags"
                CFLAGS="$CFLAGS $svgc_cflags"
                LDFLAGS="$LDFLAGS $svgc_ldflags"
                LIBS="$LIBS $svgc_libs"
            fi
        fi
    fi
    SVGC_CPPFLAGS="$svgc_cppflags"
    SVGC_CFLAGS="$svgc_cflags"
    SVGC_LDFLAGS="$svgc_ldflags"
    SVGC_LIBS="$svgc_libs"
    AC_SUBST(SVGC_CPPFLAGS)
    AC_SUBST(SVGC_CFLAGS)
    AC_SUBST(SVGC_LDFLAGS)
    AC_SUBST(SVGC_LIBS)
    _AC_SVGC_VERBOSE([+ Final Results:])
    _AC_SVGC_VERBOSE([    o SVGC_CPPFLAGS=\"$SVGC_CPPFLAGS\"])
    _AC_SVGC_VERBOSE([    o SVGC_CFLAGS=\"$SVGC_CFLAGS\"])
    _AC_SVGC_VERBOSE([    o SVGC_LDFLAGS=\"$SVGC_LDFLAGS\"])
    _AC_SVGC_VERBOSE([    o SVGC_LIBS=\"$SVGC_LIBS\"])
fi
if test ".$with_svgc" != .no; then
    AC_MSG_RESULT([version $svgc_version, $svgc_type under $svgc_location])
    ifelse([$5], , :, [$5])
else
    AC_MSG_RESULT([no])
    ifelse([$6], , :, [$6])
fi
])

