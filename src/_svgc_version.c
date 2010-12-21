
/***************************************************************
  _SVGC_VERSION.C

       This file containt VERSION operation functions & data
       definitions.

       PART OF : SVGC - Simple SVG generator .

       COMPILE : -I. .

       NOTE    : NONE .

       Copyright (C) 2000 - 2004 by Andrew V.Kosteltsev.
       All Rights Reserved.
 ***************************************************************/
/*
  This file contant RUSSIAN letters( code-page: iso-8859-5 )
 ***************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif


#define _VERSION_C_AS_HEADER_
#include <version.c>
#undef  _VERSION_C_AS_HEADER_


int
_svgc_get_major_version()
{
   int    major, minor, micro;
   char  *tmp_version;

   tmp_version = (char *)_svgc_version.v_short;
   if( sscanf( tmp_version, "%d.%d.%d", &major, &minor, &micro) != 3 )
   {
      return( -1 );
   }

   return( major );
}

int
_svgc_get_minor_version()
{
   int    major, minor, micro;
   char  *tmp_version;

   tmp_version = (char *)_svgc_version.v_short;
   if( sscanf( tmp_version, "%d.%d.%d", &major, &minor, &micro) != 3 )
   {
      return( -1 );
   }

   return( minor );
}

int
_svgc_get_micro_version()
{
   int    major, minor, micro;
   char  *tmp_version;

   tmp_version = (char *)_svgc_version.v_short;
   if( sscanf( tmp_version, "%d.%d.%d", &major, &minor, &micro) != 3 )
   {
      return( -1 );
   }

   return( micro );
}
