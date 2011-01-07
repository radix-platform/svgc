
/* This file just defines the current version number of SVGC. */

#define RELEASE "stable"
#define VERSION "1.0.2"

/*
  Versioning for LIBTOOL:
  ======================
  
  NOTE:
     For Linux
        major=.`expr $current - $age`
	versuffix="$major.$age.$revision"
     For Cygwin 
        major=`expr $current - $age`
	versuffix="-$major"
     see: $(top_builddir)/libtool
 */
#define LT_CURRENT "1"
#define LT_REVISION "2"
#define LT_AGE "0"
