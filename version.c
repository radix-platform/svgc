/*
**  version.c -- Version Information for _svgc (syntax: C/C++)
**  [automatically generated and maintained by GNU shtool]
*/

#ifdef _VERSION_C_AS_HEADER_

#ifndef _VERSION_C_
#define _VERSION_C_

#define _SVGC_VERSION 0x100208

typedef struct {
    const int   v_hex;
    const char *v_short;
    const char *v_long;
    const char *v_tex;
    const char *v_gnu;
    const char *v_web;
    const char *v_sccs;
    const char *v_rcs;
} _svgc_version_t;

extern _svgc_version_t _svgc_version;

#endif /* _VERSION_C_ */

#else /* _VERSION_C_AS_HEADER_ */

#define _VERSION_C_AS_HEADER_
#include "version.c"
#undef  _VERSION_C_AS_HEADER_

_svgc_version_t _svgc_version = {
    0x100208,
    "1.0.8",
    "1.0.8 (15-Nov-2018)",
    "This is _svgc, Version 1.0.8 (15-Nov-2018)",
    "_svgc 1.0.8 (15-Nov-2018)",
    "_svgc/1.0.8",
    "@(#)_svgc 1.0.8 (15-Nov-2018)",
    "$Id: _svgc 1.0.8 (15-Nov-2018) $"
};

#endif /* _VERSION_C_AS_HEADER_ */

