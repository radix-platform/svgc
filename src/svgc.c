
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_STDLIB_H
#include <stdlib.h>     /* exit()   */
#endif

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif


#include <svgc.h>

int current_id = 3700;

/***************************************************************
  __svg_str_dup()

     Выделяет память для дубликата строки S и копирует в нее
     содержимое S.

     Возвращаемое значение:
        Указатель на созданный дубликат.
 ***************************************************************/
char *
__svg_str_dup( s )
   const char *s;
{
   int   len = 0;
   void *new = NULL;

   if( s == (char *)NULL ) return( (char *)NULL );

   len = (strlen( s ) + 1) * sizeof( char );
   new = malloc( len );

   if( new == (void *)NULL ) return( (char *)NULL );

   return( (char *)memcpy( new, (void *)s, len ) );
  
} /* End of __svg_str_dup() */


/***************************************************************
  POINTS:
 ***************************************************************/
void 
__svg_free_point( data )
   void *data;
{
   struct __svg_point *p = (struct __svg_point *)data;

   if( !p ) return;

   free( p );
}

void __svg_free_points( p )
   struct __svg_points *p;
{
   if( !p ) return;
   if( p->plist )
   {
      /* clear svgc_dlist */
      free_svgc_dlist( p->plist, __svg_free_point );
   }

   free( p );
}

struct __svg_points *
__svg_new_points()
{
   struct __svg_points *new = (struct __svg_points *)NULL;

   new = (struct __svg_points *)malloc( sizeof(struct __svg_points) );
   if( new == (struct __svg_points *)NULL ) return( (struct __svg_points *)NULL );
   memset( (void *)new, 0, sizeof(struct __svg_points) );

   new->plist = create_svgc_dlist();
   if( !new->plist )
   {
      free( new );
      return( (struct __svg_points *)NULL );
   }
   new->count = 0;

   return( (struct __svg_points *)new );
}

int __svg_add_point( ps, x, y )
   struct __svg_points *ps;
   double x, y;
{
   struct __svg_point *new = (struct __svg_point *)NULL;
   struct svgc_dlist_node *np = NULL;
   char                    buf[256];
   int                     error = -1; /* EINVAL */

   if( !ps || !ps->plist) return( error );
   error = 0;

   new = (struct __svg_point *)malloc( sizeof(struct __svg_point) );
   if( new == (struct __svg_point *)NULL ) return( error );
   memset( (void *)new, 0, sizeof(struct __svg_point) );

   new->x = x;
   new->y = y;
   error = ++ps->count;
   sprintf( buf, "%d", error );

   np = add_to_svgc_dlist_down( ps->plist, buf, (void *)new );
   if( !np ) error = 0;

   return( error );
}

/***************************************************************
  PATH CMDS:
 ***************************************************************/
void 
__svg_free_cmd( data )
   void *data;
{
   struct __svg_cmd *cmd = (struct __svg_cmd *)data;

   if( !cmd ) return;
   if( !cmd->points )
      __svg_free_points( cmd->points );

   free( cmd );
}

void __svg_free_cmds( cmds )
   struct __svg_cmds *cmds;
{
   if( !cmds ) return;
   if( cmds->plist )
   {
      /* clear svgc_dlist */
      free_svgc_dlist( cmds->plist, __svg_free_cmd );
   }

   free( cmds );
}

struct __svg_cmd *
__svg_new_cmd( cmd, ps )
   char                 cmd;
   struct __svg_points *ps;
{
   struct __svg_cmd *new = NULL;

   if( !ps ) return( new );

   new = (struct __svg_cmd *)malloc( sizeof( struct __svg_cmd ) );
   if( new == (struct __svg_cmd *)NULL ) return( (struct __svg_cmd *)NULL );
   memset( (void *)new, 0, sizeof( struct __svg_cmd ) );

   new->cmd    = cmd;
   new->points = ps;

   return( (struct __svg_cmd *)new );
}

struct __svg_cmds *
__svg_new_cmds()
{
   struct __svg_cmds *new = (struct __svg_cmds *)NULL;

   new = (struct __svg_cmds *)malloc( sizeof(struct __svg_cmds) );
   if( new == (struct __svg_cmds *)NULL ) return( (struct __svg_cmds *)NULL );
   memset( (void *)new, 0, sizeof(struct __svg_cmds) );

   new->plist = create_svgc_dlist();
   if( !new->plist )
   {
      free( new );
      return( (struct __svg_cmds *)NULL );
   }
   new->count = 0;

   return( (struct __svg_cmds *)new );
}

int __svg_add_cmd( cmds, cmd )
   struct __svg_cmds *cmds;
   struct __svg_cmd  *cmd;
{
   struct svgc_dlist_node *np = NULL;
   char                    buf[256];
   int                     error = -1; /* EINVAL */

   if( !cmd || !cmds || !cmds->plist) return( error );
   error = 0;

   error = ++cmds->count;
   sprintf( buf, "%d", error );

   np = add_to_svgc_dlist_down( cmds->plist, buf, (void *)cmd );
   if( !np ) error = 0;

   return( error );
}

/*
  Start a new sub-path at the given (x,y) coordinate. M (uppercase) indicates
  that absolute coordinates will follow; m (lowercase) indicates that relative 
  coordinates will follow. If a relative moveto (m) appears as the first 
  element of the path, then it is treated as a pair of absolute coordinates. 
  If a moveto is followed by multiple pairs of coordinates, the subsequent 
  pairs are treated as implicit lineto commands.

  Args: (x y)+
 */
int 
__svg_add_moveto_cmd( cmds, abs, x, y )
   struct __svg_cmds *cmds;
   int                abs; /* absolute:abs=1 or relative:abs=0 */
   double             x, y;
{
   struct __svg_points *points = NULL;
   struct __svg_cmd    *cmd    = NULL;
   char                 c      = 'm';
   int                  error  = -1; /* EINVAL */

   if( !cmds || !cmds->plist) return( error );
   error = 0;

   points = __svg_new_points();
   if( !points ) return( error );
   error = __svg_add_point( points, x, y );
   if( error < 1 ) return( error );
   if( abs ) c = 'M';
   cmd = __svg_new_cmd( c, points );
   error = __svg_add_cmd( cmds, cmd );

   return( error );
}

/*
  Close the current subpath by drawing a straight line from the current point 
  to current subpath's initial point.
 */
int 
__svg_add_closepath_cmd( cmds, abs )
   struct __svg_cmds *cmds;
   int                abs; /* absolute:abs=1 or relative:abs=0 */
{
   struct __svg_points *points = NULL;
   struct __svg_cmd    *cmd    = NULL;
   char                 c      = 'z';
   int                  error  = -1; /* EINVAL */

   if( !cmds || !cmds->plist) return( error );
   error = 0;

   points = __svg_new_points();
   if( !points ) return( error );
   error = __svg_add_point( points, 0.0, 0.0 );
   if( error < 1 ) return( error );
   if( abs ) c = 'Z';
   cmd = __svg_new_cmd( c, points );
   error = __svg_add_cmd( cmds, cmd );

   return( error );
}

/*
  Draw a line from the current point to the given (x,y) coordinate which 
  becomes the new current point. L (uppercase) indicates that absolute 
  coordinates will follow; l (lowercase) indicates that relative coordinates 
  will follow. A number of coordinates pairs may be specified to draw 
  a polyline. At the end of the command, the new current point is set to the 
  final set of coordinates provided.

  Args: (x y)+
 */
int 
__svg_add_lineto_cmd( cmds, abs, x, y )
   struct __svg_cmds *cmds;
   int                abs; /* absolute:abs=1 or relative:abs=0 */
   double             x, y;
{
   struct __svg_points *points = NULL;
   struct __svg_cmd    *cmd    = NULL;
   char                 c      = 'l';
   int                  error  = -1; /* EINVAL */

   if( !cmds || !cmds->plist) return( error );
   error = 0;

   points = __svg_new_points();
   if( !points ) return( error );
   error = __svg_add_point( points, x, y );
   if( error < 1 ) return( error );
   if( abs ) c = 'L';
   cmd = __svg_new_cmd( c, points );
   error = __svg_add_cmd( cmds, cmd );

   return( error );
}

/*
  Draws a horizontal line from the current point (cpx, cpy) to (x, cpy). 
  H (uppercase) indicates that absolute coordinates will follow; h (lowercase) 
  indicates that relative coordinates will follow. Multiple x values can 
  be provided (although usually this doesn't make sense). At the end of the 
  command, the new current point becomes (x, cpy) for the final value of x.

  Args: (x)+
 */
int 
__svg_add_hlineto_cmd( cmds, abs, x )
   struct __svg_cmds *cmds;
   int                abs; /* absolute:abs=1 or relative:abs=0 */
   double             x;
{
   struct __svg_points *points = NULL;
   struct __svg_cmd    *cmd    = NULL;
   char                 c      = 'h';
   int                  error  = -1; /* EINVAL */

   if( !cmds || !cmds->plist) return( error );
   error = 0;

   points = __svg_new_points();
   if( !points ) return( error );
   error = __svg_add_point( points, x, 0.0 );
   if( error < 1 ) return( error );
   if( abs ) c = 'H';
   cmd = __svg_new_cmd( c, points );
   error = __svg_add_cmd( cmds, cmd );

   return( error );
}

/*
  Draws a vertical line from the current point (cpx, cpy) to (cpx, y). 
  V (uppercase) indicates that absolute coordinates will follow; v (lowercase) 
  indicates that relative coordinates will follow. Multiple y values can be 
  provided (although usually this doesn't make sense). At the end of the 
  command, the new current point becomes (cpx, y) for the final value of y.

  Args: (y)+
 */
int 
__svg_add_vlineto_cmd( cmds, abs, y )
   struct __svg_cmds *cmds;
   int                abs; /* absolute:abs=1 or relative:abs=0 */
   double             y;
{
   struct __svg_points *points = NULL;
   struct __svg_cmd    *cmd    = NULL;
   char                 c      = 'v';
   int                  error  = -1; /* EINVAL */

   if( !cmds || !cmds->plist) return( error );
   error = 0;

   points = __svg_new_points();
   if( !points ) return( error );
   error = __svg_add_point( points, 0.0, y );
   if( error < 1 ) return( error );
   if( abs ) c = 'V';
   cmd = __svg_new_cmd( c, points );
   error = __svg_add_cmd( cmds, cmd );

   return( error );
}

/*
  Draws a cubic Bezier curve from the current point to (x,y) using (x1,y1) as 
  the control point at the beginning of the curve and (x2,y2) as the control 
  point at the end of the curve. C (uppercase) indicates that absolute 
  coordinates will follow; c (lowercase) indicates that relative coordinates 
  will follow. Multiple sets of coordinates may be specified to draw a
  polybezier. At the end of the command, the new current point becomes the
  final (x,y) coordinate pair used in the polybezier.

  Args: (x1 y1 x2 y2 x y)+
 */
int 
__svg_add_curveto_cmd( cmds, abs, x1, y1, x2, y2, x, y )
   struct __svg_cmds *cmds;
   int                abs; /* absolute:abs=1 or relative:abs=0 */
   double             x1, y1, x2, y2, x, y;
{
   struct __svg_points *points = NULL;
   struct __svg_cmd    *cmd    = NULL;
   char                 c      = 'c';
   int                  error  = -1; /* EINVAL */

   if( !cmds || !cmds->plist) return( error );
   error = 0;

   points = __svg_new_points();
   if( !points ) return( error );

   error = __svg_add_point( points, x1, y1 );
   if( error < 1 ) return( error );
   error = __svg_add_point( points, x2, y2 );
   if( error < 1 ) return( error );
   error = __svg_add_point( points, x, y );
   if( error < 1 ) return( error );

   if( abs ) c = 'C';
   cmd = __svg_new_cmd( c, points );
   error = __svg_add_cmd( cmds, cmd );

   return( error );
}

/*
  Draws a cubic Bezier curve from the current point to (x,y). The first control 
  point is assumed to be the reflection of the second control point on the 
  previous command relative to the current point. (If there is no previous 
  command or if the previous command was not an C, c, S or s, assume the first 
  control point is coincident with the current point.) (x2,y2) is the second 
  control point (i.e., the control point at the end of the curve). 
  S (uppercase) indicates that absolute coordinates will follow; s (lowercase) 
  indicates that relative coordinates will follow. Multiple sets of coordinates 
  may be specified to draw a polybezier. At the end of the command, the new 
  current point becomes the final (x,y) coordinate pair used in the polybezier.

  Args: (x2 y2 x y)+
 */
int 
__svg_add_short_curveto_cmd( cmds, abs, x2, y2, x, y )
   struct __svg_cmds *cmds;
   int                abs; /* absolute:abs=1 or relative:abs=0 */
   double             x2, y2, x, y;
{
   struct __svg_points *points = NULL;
   struct __svg_cmd    *cmd    = NULL;
   char                 c      = 's';
   int                  error  = -1; /* EINVAL */

   if( !cmds || !cmds->plist) return( error );
   error = 0;

   points = __svg_new_points();
   if( !points ) return( error );

   error = __svg_add_point( points, x2, y2 );
   if( error < 1 ) return( error );
   error = __svg_add_point( points, x, y );
   if( error < 1 ) return( error );

   if( abs ) c = 'S';
   cmd = __svg_new_cmd( c, points );
   error = __svg_add_cmd( cmds, cmd );

   return( error );
}

/*
  Draws a quadratic Bezier curve from the current point to (x,y) using (x1,y1) 
  as the control point. Q (uppercase) indicates that absolute coordinates will 
  follow; q (lowercase) indicates that relative coordinates will follow. 
  Multiple sets of coordinates may be specified to draw a polybezier. At the 
  end of the command, the new current point becomes the final (x,y) coordinate 
  pair used in the polybezier.

  Args: (x1 y1 x y)+
 */
int 
__svg_add_quadro_curveto_cmd( cmds, abs, x1, y1, x, y )
   struct __svg_cmds *cmds;
   int                abs; /* absolute:abs=1 or relative:abs=0 */
   double             x1, y1, x, y;
{
   struct __svg_points *points = NULL;
   struct __svg_cmd    *cmd    = NULL;
   char                 c      = 'q';
   int                  error  = -1; /* EINVAL */

   if( !cmds || !cmds->plist) return( error );
   error = 0;

   points = __svg_new_points();
   if( !points ) return( error );

   error = __svg_add_point( points, x1, y1 );
   if( error < 1 ) return( error );
   error = __svg_add_point( points, x, y );
   if( error < 1 ) return( error );

   if( abs ) c = 'Q';
   cmd = __svg_new_cmd( c, points );
   error = __svg_add_cmd( cmds, cmd );

   return( error );
}

/*
  Draws a quadratic Bezier curve from the current point to (x,y). The control 
  point is assumed to be the reflection of the control point on the previous 
  command relative to the current point. (If there is no previous command or if 
  the previous command was not a Q, q, T or t, assume the control point is 
  coincident with the current point.) T (uppercase) indicates that absolute 
  coordinates will follow; t (lowercase) indicates that relative coordinates 
  will follow. At the end of the command, the new current point becomes the 
  final (x,y) coordinate pair used in the polybezier.

  Args: (x y)+
 */
int 
__svg_add_short_quadro_curveto_cmd( cmds, abs, x, y )
   struct __svg_cmds *cmds;
   int                abs; /* absolute:abs=1 or relative:abs=0 */
   double             x, y;
{
   struct __svg_points *points = NULL;
   struct __svg_cmd    *cmd    = NULL;
   char                 c      = 't';
   int                  error  = -1; /* EINVAL */

   if( !cmds || !cmds->plist) return( error );
   error = 0;

   points = __svg_new_points();
   if( !points ) return( error );

   error = __svg_add_point( points, x, y );
   if( error < 1 ) return( error );

   if( abs ) c = 'T';
   cmd = __svg_new_cmd( c, points );
   error = __svg_add_cmd( cmds, cmd );

   return( error );
}

/*
  Draws an elliptical arc from the current point to (x, y). The size and 
  orientation of the ellipse are defined by two radii (rx, ry) and an 
  x-axis-rotation, which indicates how the ellipse as a whole is rotated 
  relative to the current coordinate system. The center (cx, cy) of the 
  ellipse is calculated automatically to satisfy the constraints imposed 
  by the other parameters. large-arc-flag and sweep-flag contribute to the 
  automatic calculations and help determine how the arc is drawn.

  Args: (rx ry x-axis-rotation large-arc-flag sweep-flag x y)+
 */
int 
__svg_add_arc_cmd( cmds, abs,
                   rx, ry,
                   x_axis_rotation,
                   large_arc_flag,
                   sweep_flag,
                   x, y )
   struct __svg_cmds *cmds;
   int                abs; /* absolute:abs=1 or relative:abs=0 */
   double             rx, ry;
   double             x_axis_rotation;
   int                large_arc_flag;
   int                sweep_flag;
   double             x, y;
{
   struct __svg_points *points = NULL;
   struct __svg_cmd    *cmd    = NULL;
   char                 c      = 'a';
   double               lf = 0.0, sf = 0.0;
   int                  error  = -1; /* EINVAL */

   if( !cmds || !cmds->plist) return( error );
   error = 0;

   points = __svg_new_points();
   if( !points ) return( error );

   error = __svg_add_point( points, rx, ry );
   if( error < 1 ) return( error );
   error = __svg_add_point( points, x_axis_rotation, 0.0 );
   if( error < 1 ) return( error );
   if( large_arc_flag ) lf = 1.0;
   if(     sweep_flag ) sf = 1.0;
   error = __svg_add_point( points, lf, sf );
   if( error < 1 ) return( error );
   error = __svg_add_point( points, x, y );
   if( error < 1 ) return( error );

   if( abs ) c = 'A';
   cmd = __svg_new_cmd( c, points );
   error = __svg_add_cmd( cmds, cmd );

   return( error );
}


/***************************************************************
  STYLE:
 ***************************************************************/

/***************************************************************
  Default values:
 ***************************************************************/
static struct __svg_rgb 
                     default_stroke_rgb       = { 0, 0, 0 };
static const char   *default_stroke           = STROKE_DEFAULT;
static double        default_stroke_width     = 1.0;
static const char   *default_stroke_linecap   = LINECAP_DEFAULT;
static const char   *default_stroke_linejoin  = LINEJOIN_DEFAULT;
static const char   *default_stroke_dasharray = DASHARRAY_DEFAULT;

static struct __svg_rgb 
                     default_fill_rgb         = { 255, 255, 255 };
static const char   *default_fill             = FILL_DEFAULT;

static struct __svg_rgb 
                     default_stop_color_rgb   = { 0, 0, 0 };
static const char   *default_stop_color       = STOP_COLOR_DEFAULT;

static double        default_stop_opacity     = STOP_OPACITY_DEFAULT;

/* static buffer for stroke: */
static char    stroke_buf[32];

/* static buffer for fill: */
static char    fill_buf[32];

/* static buffer for stop color: */
static char    stop_color_buf[32];

static const char   *default_text_font_family = TEXT_FONT_FAMILY_DEFAULT;
static int           default_text_font_size   = TEXT_FONT_SIZE_DEFAULT;
static struct __svg_rgb 
                     default_text_fill_rgb    = { 255, 255, 255 };
static const char   *default_text_fill        = TEXT_FILL_DEFAULT;
/* static buffer for text fill: */
static char          text_fill_buf[32];
static char          text_family_buf[128];


void __svg_set_scene_default_style()
{
   default_stroke_rgb.r = 0;
   default_stroke_rgb.g = 0;
   default_stroke_rgb.b = 0;

   default_stroke           = STROKE_DEFAULT;
   default_stroke_width     = 1.0;
   default_stroke_linecap   = LINECAP_DEFAULT;
   default_stroke_linejoin  = LINEJOIN_DEFAULT;
   default_stroke_dasharray = DASHARRAY_DEFAULT;


   default_fill_rgb.r = 255;
   default_fill_rgb.g = 255;
   default_fill_rgb.b = 255;

   default_fill = FILL_DEFAULT;

   default_stop_color_rgb.r = 0;
   default_stop_color_rgb.g = 0;
   default_stop_color_rgb.b = 0;

   default_stop_opacity = STOP_OPACITY_DEFAULT;

   default_text_fill_rgb.r = 255;
   default_text_fill_rgb.g = 255;
   default_text_fill_rgb.b = 255;

   default_text_fill        = TEXT_FILL_DEFAULT;
   default_text_font_family = TEXT_FONT_FAMILY_DEFAULT;
   default_text_font_size   = TEXT_FONT_SIZE_DEFAULT;
}

void __svg_set_scene_stroke( stroke )
   const char *stroke;
{
   default_stroke = stroke;
}
void __svg_set_scene_stroke_rgb( r, g, b )
   int r, g, b;
{
   default_stroke_rgb.r = r;
   default_stroke_rgb.g = g;
   default_stroke_rgb.b = b;

   sprintf( stroke_buf, "rgb(%03d,%03d,%03d)", r, g, b );

   default_stroke = stroke_buf;
}
void __svg_set_scene_stroke_width( width )
   double width;
{
   default_stroke_width = width;
}
void __svg_set_scene_stroke_linecap( linecap )
   const char *linecap;
{
   default_stroke_linecap = linecap;
}
void __svg_set_scene_stroke_linejoin( linejoin )
   const char *linejoin;
{
   default_stroke_linejoin = linejoin;
}
void __svg_set_scene_stroke_dasharray( dasharray )
   const char *dasharray;
{
   default_stroke_dasharray = dasharray;
}

void __svg_set_scene_fill( fill )
   const char *fill;
{
   default_fill = fill;
}
void __svg_set_scene_fill_rgb( r, g, b )
   int r, g, b;
{
   default_fill_rgb.r = r;
   default_fill_rgb.g = g;
   default_fill_rgb.b = b;

   sprintf( fill_buf, "rgb(%03d,%03d,%03d)", r, g, b );

   default_fill = fill_buf;
}

void __svg_set_scene_stop_color( stop_color )
   const char *stop_color;
{
   default_stop_color = stop_color;
}
void __svg_set_scene_stop_color_rgb( r, g, b )
   int r, g, b;
{
   default_stop_color_rgb.r = r;
   default_stop_color_rgb.g = g;
   default_stop_color_rgb.b = b;

   sprintf( stop_color_buf, "rgb(%03d,%03d,%03d)", r, g, b );

   default_stop_color = stop_color_buf;
}

void __svg_set_scene_stop_opacity( stop_opacity )
   double stop_opacity;
{
   default_stop_opacity = stop_opacity;
}

void __svg_set_scene_text_fill( fill )
   const char *fill;
{
   default_text_fill = fill;
}
void __svg_set_scene_text_fill_rgb( r, g, b )
   int r, g, b;
{
   default_text_fill_rgb.r = r;
   default_text_fill_rgb.g = g;
   default_text_fill_rgb.b = b;

   sprintf( text_fill_buf, "rgb(%03d,%03d,%03d)", r, g, b );

   default_text_fill = text_fill_buf;
}
void __svg_set_scene_text_font_family( family )
   const char *family;
{
   if( !family || !family[0] ||
       strlen( family ) > (sizeof( text_family_buf ) - 1) )
      return;

   sprintf( text_family_buf, "%s", family );

   default_text_font_family = text_family_buf;
}
void __svg_set_scene_text_font_size( size )
   int size;
{
   default_text_font_size = size;
}

void 
__svg_free_stroke( s )
   struct __svg_stroke *s;
{
   if( !s ) return;
   if( s->use_stroke ) free( s->use_stroke );
   if( s->stroke_linecap ) free( s->stroke_linecap );
   if( s->stroke_linejoin ) free( s->stroke_linejoin );
   if( s->stroke_dasharray ) free( s->stroke_dasharray );
   free( s );
}

struct __svg_stroke *
__svg_new_stroke()
{
   struct __svg_stroke *new = NULL;

   new = malloc( sizeof( struct __svg_stroke ) );

   if( new == (struct __svg_stroke *)NULL ) return( (struct __svg_stroke *)NULL );

   new->stroke.r = default_stroke_rgb.r;
   new->stroke.g = default_stroke_rgb.g;
   new->stroke.b = default_stroke_rgb.b;

   new->stroke_width = default_stroke_width;

   new->use_stroke       = __svg_str_dup( default_stroke );
   new->stroke_linecap   = __svg_str_dup( default_stroke_linecap );
   new->stroke_linejoin  = __svg_str_dup( default_stroke_linejoin );
   new->stroke_dasharray = __svg_str_dup( default_stroke_dasharray );

   return( (struct __svg_stroke *)new );
}

void 
__svg_free_fill( f )
   struct __svg_fill *f;
{
   if( !f ) return;
   if( f->use_fill ) free( f->use_fill );
   free( f );
}

struct __svg_fill *
__svg_new_fill()
{
   struct __svg_fill *new = NULL;

   new = malloc( sizeof( struct __svg_fill ) );

   if( new == (struct __svg_fill *)NULL ) return( (struct __svg_fill *)NULL );

   new->fill.r = default_fill_rgb.r;
   new->fill.g = default_fill_rgb.g;
   new->fill.b = default_fill_rgb.b;

   new->use_fill   = __svg_str_dup( default_fill );

   return( (struct __svg_fill *)new );
}


void 
__svg_free_stop_color( sc )
   struct __svg_stop_color *sc;
{
   if( !sc ) return;
   if( sc->use_stop_color ) free( sc->use_stop_color );
   free( sc );
}

struct __svg_stop_color *
__svg_new_stop_color()
{
   struct __svg_stop_color *new = NULL;

   new = malloc( sizeof( struct __svg_stop_color ) );

   if( new == (struct __svg_stop_color *)NULL ) return( (struct __svg_stop_color *)NULL );

   new->stop_color.r = default_stop_color_rgb.r;
   new->stop_color.g = default_stop_color_rgb.g;
   new->stop_color.b = default_stop_color_rgb.b;

   new->use_stop_color   = __svg_str_dup( default_stop_color );

   return( (struct __svg_stop_color *)new );
}


void 
__svg_free_stop_opacity( so )
   struct __svg_stop_opacity *so;
{
   if( !so ) return;
   free( so );
}

struct __svg_stop_opacity *
__svg_new_stop_opacity()
{
   struct __svg_stop_opacity *new = NULL;

   new = malloc( sizeof( struct __svg_stop_opacity ) );

   if( new == (struct __svg_stop_opacity *)NULL ) return( (struct __svg_stop_opacity *)NULL );

   new->stop_opacity = default_stop_opacity;

   return( (struct __svg_stop_opacity *)new );
}


void 
__svg_free_style( style )
   struct __svg_style *style;
{
   if( !style ) return;
   if( style->stroke       ) __svg_free_stroke       ( style->stroke       );
   if( style->fill         ) __svg_free_fill         ( style->fill         );
   if( style->stop_color   ) __svg_free_stop_color   ( style->stop_color   );
   if( style->stop_opacity ) __svg_free_stop_opacity ( style->stop_opacity );
   free( style );
}

struct __svg_style *
__svg_new_style()
{
   struct __svg_style  *new    = NULL;

   new = malloc( sizeof( struct __svg_style ) );

   if( new == (struct __svg_style *)NULL ) return( (struct __svg_style *)NULL );

   new->stroke       = __svg_new_stroke();
   new->fill         = __svg_new_fill();
   new->stop_color   = __svg_new_stop_color();
   new->stop_opacity = __svg_new_stop_opacity();
   if( !new->stroke || !new->fill || !new->stop_color || !new->stop_opacity )
   {
      __svg_free_style( new );
      return( (struct __svg_style *)NULL );
   }
   return( (struct __svg_style *)new );
}


void 
__svg_free_node( data )
   void *data;
{
   struct __svg_node *node = (struct __svg_node *)data;

   if( !node ) return;
   if( node->id ) free( node->id );
   if( node->style ) __svg_free_style( node->style );

   if( node->type == SVG_SHEET  )
   {
      if( ((struct __svg_sheet *)node)->defs )
      {
         /* clear svgc_dlist */
         free_svgc_dlist( ((struct __svg_sheet *)node)->defs, __svg_free_node );
      }
   }

   if( node->type == SVG_POLYGON )
      __svg_free_points( ((struct __svg_polygon *)node)->points );

   if( node->type == SVG_POLYLINE  )
      __svg_free_points( ((struct __svg_polyline *)node)->points );

   if( node->type == SVG_PATH  )
      __svg_free_cmds( ((struct __svg_path *)node)->cmds );

   if( node->type == SVG_TEXT  )
   {
      if( ((struct __svg_text *)node)->text )
         free( ((struct __svg_text *)node)->text );
      if( ((struct __svg_text *)node)->font_family )
         free( ((struct __svg_text *)node)->font_family );
      if( ((struct __svg_text *)node)->fill )
         free( ((struct __svg_text *)node)->fill );
   }

   if( node->type == SVG_IMAGE  )
   {
      if( ((struct __svg_image *)node)->href )
         free( ((struct __svg_image *)node)->href );
      if( ((struct __svg_image *)node)->title )
         free( ((struct __svg_image *)node)->title );
   }

   if( node->type == SVG_L_GRADIENT  )
   {
      if( ((struct __svg_l_gradient *)node)->units )
         free( ((struct __svg_l_gradient *)node)->units );
      if( ((struct __svg_l_gradient *)node)->spread )
         free( ((struct __svg_l_gradient *)node)->spread );
      if( ((struct __svg_l_gradient *)node)->stops )
      {
         /* clear svgc_dlist */
         free_svgc_dlist( ((struct __svg_l_gradient *)node)->stops, __svg_free_node );
      }
   }

   if( node->type == SVG_R_GRADIENT  )
   {
      if( ((struct __svg_r_gradient *)node)->units )
         free( ((struct __svg_r_gradient *)node)->units );
      if( ((struct __svg_r_gradient *)node)->spread )
         free( ((struct __svg_r_gradient *)node)->spread );
      if( ((struct __svg_r_gradient *)node)->stops )
      {
         /* clear svgc_dlist */
         free_svgc_dlist( ((struct __svg_r_gradient *)node)->stops, __svg_free_node );
      }
   }

   free( node );
}


struct __svg_node *
__svg_new_sheet( width, height,
                   left_margin,
                  right_margin,
                    top_margin,
                 bottom_margin )
   double width, height;
   double   left_margin;
   double  right_margin;
   double    top_margin;
   double bottom_margin;
{
   struct __svg_sheet *new = NULL;
   char                buf[256];

   new = (struct __svg_sheet *)malloc( sizeof( struct __svg_sheet ) );
   if( new == (struct __svg_sheet *)NULL ) return( (struct __svg_node *)NULL );
   memset( (void *)new, 0, sizeof( struct __svg_sheet ) );
   new->type  = SVG_SHEET;
   sprintf( buf, "svg%04d", current_id++ );
   new->id    = __svg_str_dup( buf );
   new->style = __svg_new_style();

   if( !new->id || !new->style ) return( (struct __svg_node *)NULL );

   new->width  =  width;
   new->height = height;
   new->left_margin   =   left_margin;
   new->right_margin  =  right_margin;
   new->top_margin    =    top_margin;
   new->bottom_margin = bottom_margin;

   new->defs = create_svgc_dlist();
   if( !new->defs )
   {
      if( new->id )    free( new->id );
      if( new->style ) __svg_free_style( new->style );;
      free( new );
      return( (struct __svg_node *)NULL );
   }

   return( (struct __svg_node *)new );
}

int
__svg_sheet_add_definition( sheet, definition )
   void               *sheet;
   struct __svg_node  *definition;
{
   struct __svg_sheet *ps = (struct __svg_sheet *)sheet;
   struct svgc_dlist_node *np = NULL;
   int error = -1; /* EINVAL */

   if( !ps || !ps->defs || !definition ) return( error );
   error = 0;

   if( !ps->defs ) return( error );

   np = add_to_svgc_dlist_down( ps->defs, definition->id, (void *)definition );
   if( np ) error = 1;

   return( error );
}


struct __svg_node *
__svg_new_line( x1, y1, x2, y2 )
   double x1, y1;
   double x2, y2;
{
   struct __svg_line *new = NULL;
   char               buf[256];

   new = (struct __svg_line *)malloc( sizeof( struct __svg_line ) );
   if( new == (struct __svg_line *)NULL ) return( (struct __svg_node *)NULL );
   memset( (void *)new, 0, sizeof( struct __svg_line ) );
   new->type  = SVG_LINE;
   sprintf( buf, "line%04d", current_id++ );
   new->id    = __svg_str_dup( buf );
   new->style = __svg_new_style();

   if( !new->id || !new->style ) return( (struct __svg_node *)NULL );

   new->x1 = x1;
   new->y1 = y1;
   new->x2 = x2;
   new->y2 = y2;

   return( (struct __svg_node *)new );
}

struct __svg_node *
__svg_new_rect( x, y, width, height, rx, ry )
   double x, y;
   double width, height;
   double rx, ry;
{
   struct __svg_rect *new = NULL;
   char               buf[256];

   new = (struct __svg_rect *)malloc( sizeof( struct __svg_rect ) );
   if( new == (struct __svg_rect *)NULL ) return( (struct __svg_node *)NULL );
   memset( (void *)new, 0, sizeof( struct __svg_rect ) );
   new->type  = SVG_RECT;
   sprintf( buf, "rect%04d", current_id++ );
   new->id    = __svg_str_dup( buf );
   new->style = __svg_new_style();

   if( !new->id || !new->style ) return( (struct __svg_node *)NULL );

   if( rx < 0.0 ) rx = 0.0;
   if( ry < 0.0 ) ry = 0.0;

   new->x = x;
   new->y = y;
   new->width  =  width;
   new->height = height;
   new->rx = rx;
   new->ry = ry;

   return( (struct __svg_node *)new );
}

struct __svg_node *
__svg_new_circle( cx, cy, r )
   double cx, cy;
   double r;
{
   struct __svg_circle *new = NULL;
   char                 buf[256];

   new = (struct __svg_circle *)malloc( sizeof( struct __svg_circle ) );
   if( new == (struct __svg_circle *)NULL ) return( (struct __svg_node *)NULL );
   memset( (void *)new, 0, sizeof( struct __svg_circle ) );
   new->type  = SVG_CIRCLE;
   sprintf( buf, "circle%04d", current_id++ );
   new->id    = __svg_str_dup( buf );
   new->style = __svg_new_style();

   if( !new->id || !new->style ) return( (struct __svg_node *)NULL );

   new->cx = cx;
   new->cy = cy;
   new->r = r;

   return( (struct __svg_node *)new );
}

struct __svg_node *
__svg_new_ellipse( cx, cy, rx, ry )
   double cx, cy;
   double rx, ry;
{
   struct __svg_ellipse *new = NULL;
   char                  buf[256];

   new = (struct __svg_ellipse *)malloc( sizeof( struct __svg_ellipse ) );
   if( new == (struct __svg_ellipse *)NULL ) return( (struct __svg_node *)NULL );
   memset( (void *)new, 0, sizeof( struct __svg_ellipse ) );
   new->type  = SVG_ELLIPSE;
   sprintf( buf, "ellipse%04d", current_id++ );
   new->id    = __svg_str_dup( buf );
   new->style = __svg_new_style();

   if( !new->id || !new->style ) return( (struct __svg_node *)NULL );

   new->cx = cx;
   new->cy = cy;
   new->rx = rx;
   new->ry = ry;

   return( (struct __svg_node *)new );
}

struct __svg_node *
__svg_new_polygon( ps )
   struct __svg_points *ps;
{
   struct __svg_polygon *new = NULL;
   char                  buf[256];

   if( !ps ) return( (struct __svg_node *)new );

   new = (struct __svg_polygon *)malloc( sizeof( struct __svg_polygon ) );
   if( new == (struct __svg_polygon *)NULL ) return( (struct __svg_node *)NULL );
   memset( (void *)new, 0, sizeof( struct __svg_polygon ) );
   new->type  = SVG_POLYGON;
   sprintf( buf, "polygon%04d", current_id++ );
   new->id    = __svg_str_dup( buf );
   new->style = __svg_new_style();

   if( !new->id || !new->style ) return( (struct __svg_node *)NULL );

   new->points = ps;

   return( (struct __svg_node *)new );
}

struct __svg_node *
__svg_new_polyline( ps )
   struct __svg_points *ps;
{
   struct __svg_polyline *new = NULL;
   char                   buf[256];

   if( !ps ) return( (struct __svg_node *)new );

   new = (struct __svg_polyline *)malloc( sizeof( struct __svg_polyline ) );
   if( new == (struct __svg_polyline *)NULL ) return( (struct __svg_node *)NULL );
   memset( (void *)new, 0, sizeof( struct __svg_polyline ) );
   new->type  = SVG_POLYLINE;
   sprintf( buf, "polyline%04d", current_id++ );
   new->id    = __svg_str_dup( buf );
   new->style = __svg_new_style();

   if( !new->id || !new->style ) return( (struct __svg_node *)NULL );

   new->points = ps;

   return( (struct __svg_node *)new );
}

struct __svg_node *
__svg_new_path( cmds )
   struct __svg_cmds *cmds;
{
   struct __svg_path *new = NULL;
   char               buf[256];

   if( !cmds ) return( (struct __svg_node *)new );

   new = (struct __svg_path *)malloc( sizeof( struct __svg_path ) );
   if( new == (struct __svg_path *)NULL ) return( (struct __svg_node *)NULL );
   memset( (void *)new, 0, sizeof( struct __svg_path ) );
   new->type  = SVG_PATH;
   sprintf( buf, "path%04d", current_id++ );
   new->id    = __svg_str_dup( buf );
   new->style = __svg_new_style();

   if( !new->id || !new->style ) return( (struct __svg_node *)NULL );

   new->cmds = cmds;

   return( (struct __svg_node *)new );
}

struct __svg_node *
__svg_new_text( x, y, text, font_family, font_size )
   double      x, y;
   const char *text, *font_family;
   int         font_size;
{
   struct __svg_text *new = NULL;
   char               buf[256];

   if( !text || !text[0] ) return( (struct __svg_node *)new );

   new = (struct __svg_text *)malloc( sizeof( struct __svg_text ) );
   if( new == (struct __svg_text *)NULL ) return( (struct __svg_node *)NULL );
   memset( (void *)new, 0, sizeof( struct __svg_text ) );
   new->type  = SVG_TEXT;
   sprintf( buf, "text%04d", current_id++ );
   new->id    = __svg_str_dup( buf );
   new->style = __svg_new_style();

   if( !new->id || !new->style ) return( (struct __svg_node *)NULL );

   new->x    = x;
   new->y    = y;
   new->text = __svg_str_dup( text );

   if( !font_family || !font_family[0] )
      new->font_family = __svg_str_dup( default_text_font_family );
   else
      new->font_family = __svg_str_dup( font_family );

   if( !font_size )
      new->font_size = default_text_font_size;
   else
      new->font_size = font_size;

   new->fill = __svg_str_dup( default_text_fill );

   return( (struct __svg_node *)new );
}

struct __svg_node *
__svg_new_image( x, y, width, height, href, title )
   double x, y;
   int    width, height;
   char   *href;
   char   *title;
{
   struct __svg_image *new = NULL;
   char                buf[256];

   if( !href || !href[0] ) return( (struct __svg_node *)new );

   new = (struct __svg_image *)malloc( sizeof( struct __svg_image ) );
   if( new == (struct __svg_image *)NULL ) return( (struct __svg_node *)NULL );
   memset( (void *)new, 0, sizeof( struct __svg_image ) );
   new->type  = SVG_IMAGE;
   sprintf( buf, "image%04d", current_id++ );
   new->id    = __svg_str_dup( buf );
   new->style = __svg_new_style();

   if( !new->id || !new->style ) return( (struct __svg_node *)NULL );

   new->x = x;
   new->y = y;
   new->width  = width;
   new->height = height;

   new->href = __svg_str_dup( href );

   if( title )
      new->title = __svg_str_dup( title );
   else
      new->title = __svg_str_dup( "image" );

   return( (struct __svg_node *)new );
}

struct __svg_node *
__svg_new_l_gradient( units, spread, x1, y1, x2, y2 )
   const char *units;
   const char *spread;
   double      x1, y1;
   double      x2, y2;
{
   struct __svg_l_gradient *new = NULL;
   char                     buf[256];

   new = (struct __svg_l_gradient *)malloc( sizeof( struct __svg_l_gradient ) );
   if( new == (struct __svg_l_gradient *)NULL ) return( (struct __svg_node *)NULL );
   memset( (void *)new, 0, sizeof( struct __svg_l_gradient ) );
   new->type  = SVG_L_GRADIENT;
   sprintf( buf, "lg%04d", current_id++ );
   new->id    = __svg_str_dup( buf );
   new->style = __svg_new_style();

   if( !new->id || !new->style ) return( (struct __svg_node *)NULL );

   if( !units || !units[0] ) new->units = __svg_str_dup( "objectBoundingBox" );
   else                      new->units = __svg_str_dup( units );

   if( !spread || !spread[0] ) new->spread = __svg_str_dup( "pad" );
   else                        new->spread = __svg_str_dup( spread );

   new->x1 = x1;
   new->y1 = y1;
   new->x2 = x2;
   new->y2 = y2;

   new->stops = create_svgc_dlist();
   if( !new->stops )
   {
      if( new->id )    free( new->id );
      if( new->style ) __svg_free_style( new->style );;
      if( new->units ) free( new->units );
      free( new );
      return( (struct __svg_node *)NULL );
   }

   return( (struct __svg_node *)new );
}

struct __svg_node *
__svg_new_r_gradient( units, spread, cx, cy, r, fx, fy )
   const char *units;
   const char *spread;
   double      cx, cy;
   double      r;
   double      fx, fy;
{
   struct __svg_r_gradient *new = NULL;
   char                     buf[256];

   new = (struct __svg_r_gradient *)malloc( sizeof( struct __svg_r_gradient ) );
   if( new == (struct __svg_r_gradient *)NULL ) return( (struct __svg_node *)NULL );
   memset( (void *)new, 0, sizeof( struct __svg_r_gradient ) );
   new->type  = SVG_R_GRADIENT;
   sprintf( buf, "rg%04d", current_id++ );
   new->id    = __svg_str_dup( buf );
   new->style = __svg_new_style();

   if( !new->id || !new->style ) return( (struct __svg_node *)NULL );

   if( !units || !units[0] ) new->units = __svg_str_dup( "objectBoundingBox" );
   else                      new->units = __svg_str_dup( units );

   if( !spread || !spread[0] ) new->spread = __svg_str_dup( "pad" );
   else                        new->spread = __svg_str_dup( spread );

   new->cx = cx;
   new->cy = cy;
   new->r = r;
   new->fx = fx;
   new->fy = fy;

   new->stops = create_svgc_dlist();
   if( !new->stops )
   {
      if( new->id )    free( new->id );
      if( new->style ) __svg_free_style( new->style );;
      if( new->units ) free( new->units );
      free( new );
      return( (struct __svg_node *)NULL );
   }

   return( (struct __svg_node *)new );
}

int
__svg_gradient_add_stop( gradient, stop )
   void               *gradient;
   struct __svg_stop  *stop;
{
   struct __svg_gradient *pg = (struct __svg_gradient *)gradient;
   struct svgc_dlist_node *np = NULL;
   int error = -1; /* EINVAL */

   if( !pg || !pg->stops || !stop ) return( error );
   error = 0;

   if( !pg->stops ) return( error );

   np = add_to_svgc_dlist_down( pg->stops, stop->id, (void *)stop );
   if( np ) error = 1;

   return( error );
}

struct __svg_node *
__svg_new_stop( offset )
   double  offset;
{
   struct __svg_stop *new = NULL;
   char               buf[256];

   new = (struct __svg_stop *)malloc( sizeof( struct __svg_stop ) );
   if( new == (struct __svg_stop *)NULL ) return( (struct __svg_node *)NULL );
   memset( (void *)new, 0, sizeof( struct __svg_stop ) );
   new->type  = SVG_STOP;
   sprintf( buf, "stop%04d", current_id++ );
   new->id    = __svg_str_dup( buf );
   new->style = __svg_new_style();

   if( !new->id || !new->style ) return( (struct __svg_node *)NULL );

   if( offset > 1.0 || offset < 0.0   ) offset = 0.0;
   new->offset = offset;

   return( (struct __svg_node *)new );
}

/***************************************************************
  Style controls:
 ***************************************************************/
int __svg_set_stroke( node, stroke )
   struct __svg_node *node;
   const char        *stroke;
{
   int error = -1; /* EINVAL */

   if( !node ) return( error );
   error = 0;

   node->style->stroke->stroke.r = 0;
   node->style->stroke->stroke.g = 0;
   node->style->stroke->stroke.b = 0;

   if( node->style->stroke->use_stroke )
      free( node->style->stroke->use_stroke );
   node->style->stroke->use_stroke = __svg_str_dup( stroke );
   if( node->style->stroke->use_stroke ) error = 1;

   return( error );
}

int __svg_set_stroke_rgb( node, r, g, b )
   struct __svg_node *node;
   int                r, g, b;
{
   char buf[256];
   int error = -1; /* EINVAL */

   if( !node ) return( error );
   error = 0;

   node->style->stroke->stroke.r = r;
   node->style->stroke->stroke.g = g;
   node->style->stroke->stroke.b = b;

   sprintf( buf, "rgb(%03d,%03d,%03d)", r, g, b );

   if( node->style->stroke->use_stroke )
      free( node->style->stroke->use_stroke );
   node->style->stroke->use_stroke = __svg_str_dup( buf );
   if( node->style->stroke->use_stroke ) error = 1;

   return( error );
}

int __svg_set_stroke_width( node, width )
   struct __svg_node *node;
   double             width;
{
   int error = -1; /* EINVAL */

   if( !node ) return( error );
   error = 0;

   node->style->stroke->stroke_width = width;
   return( error );
}

int __svg_set_stroke_linecap( node, linecap )
   struct __svg_node *node;
   const char        *linecap;
{
   int error = -1; /* EINVAL */

   if( !node ) return( error );
   error = 0;

   if( node->style->stroke->stroke_linecap )
      free( node->style->stroke->stroke_linecap );
   node->style->stroke->stroke_linecap = __svg_str_dup( linecap );
   if( node->style->stroke->stroke_linecap ) error = 1;

   return( error );
}

int __svg_set_stroke_linejoin( node, linejoin )
   struct __svg_node *node;
   const char        *linejoin;
{
   int error = -1; /* EINVAL */

   if( !node ) return( error );
   error = 0;

   if( node->style->stroke->stroke_linejoin )
      free( node->style->stroke->stroke_linejoin );
   node->style->stroke->stroke_linejoin = __svg_str_dup( linejoin );
   if( node->style->stroke->stroke_linejoin ) error = 1;

   return( error );
}

int __svg_set_stroke_dasharray( node, dasharray )
   struct __svg_node *node;
   const char        *dasharray;
{
   int error = -1; /* EINVAL */

   if( !node ) return( error );
   error = 0;

   if( node->style->stroke->stroke_dasharray )
      free( node->style->stroke->stroke_dasharray );
   node->style->stroke->stroke_dasharray = __svg_str_dup( dasharray );
   if( node->style->stroke->stroke_dasharray ) error = 1;

   return( error );
}


int __svg_set_fill( node, fill )
   struct __svg_node *node;
   const char        *fill;
{
   int error = -1; /* EINVAL */

   if( !node ) return( error );
   error = 0;

   node->style->fill->fill.r = 0;
   node->style->fill->fill.g = 0;
   node->style->fill->fill.b = 0;

   if( node->style->fill->use_fill )
      free( node->style->fill->use_fill );
   node->style->fill->use_fill = __svg_str_dup( fill );
   if( node->style->fill->use_fill ) error = 1;

   return( error );
}

int __svg_set_fill_rgb( node, r, g, b )
   struct __svg_node *node;
   int                r, g, b;
{
   char buf[256];
   int  error = -1; /* EINVAL */

   if( !node ) return( error );
   error = 0;

   node->style->fill->fill.r = r;
   node->style->fill->fill.g = g;
   node->style->fill->fill.b = b;

   sprintf( buf, "rgb(%03d,%03d,%03d)", r, g, b );

   if( node->style->fill->use_fill )
      free( node->style->fill->use_fill );
   node->style->fill->use_fill = __svg_str_dup( buf );
   if( node->style->fill->use_fill ) error = 1;

   return( error );
}


int __svg_set_stop_color( node, stop_color )
   struct __svg_node *node;
   const char        *stop_color;
{
   int error = -1; /* EINVAL */

   if( !node ) return( error );
   error = 0;

   node->style->stop_color->stop_color.r = 0;
   node->style->stop_color->stop_color.g = 0;
   node->style->stop_color->stop_color.b = 0;

   if( node->style->stop_color->use_stop_color )
      free( node->style->stop_color->use_stop_color );
   node->style->stop_color->use_stop_color = __svg_str_dup( stop_color );
   if( node->style->stop_color->use_stop_color ) error = 1;

   return( error );
}

int __svg_set_stop_color_rgb( node, r, g, b )
   struct __svg_node *node;
   int                r, g, b;
{
   char buf[256];
   int  error = -1; /* EINVAL */

   if( !node ) return( error );
   error = 0;

   node->style->stop_color->stop_color.r = r;
   node->style->stop_color->stop_color.g = g;
   node->style->stop_color->stop_color.b = b;

   sprintf( buf, "rgb(%03d,%03d,%03d)", r, g, b );

   if( node->style->stop_color->use_stop_color )
      free( node->style->stop_color->use_stop_color );
   node->style->stop_color->use_stop_color = __svg_str_dup( buf );
   if( node->style->stop_color->use_stop_color ) error = 1;

   return( error );
}


int __svg_set_stop_opacity( node, stop_opacity )
   struct __svg_node *node;
   double             stop_opacity;
{
   int error = -1; /* EINVAL */

   if( !node ) return( error );
   error = 0;

   if( stop_opacity > 1.0 ||
       stop_opacity < 0.0   ) stop_opacity = default_stop_opacity;
   node->style->stop_opacity->stop_opacity = stop_opacity;

   return( error );
}


int __svg_set_text( node, text )
   struct __svg_node *node;
   const char        *text;
{
   int error = -1; /* EINVAL */

   if( !node || !text || !text[0] ) return( error );
   error = 0;

   if( ((__svg_text *)node)->text )
      free( ((__svg_text *)node)->text );
   ((__svg_text *)node)->text = __svg_str_dup( text );
   if( !((__svg_text *)node)->text ) error = 1;

   return( error );
}

int __svg_set_text_fill( node, fill )
   struct __svg_node *node;
   const char        *fill;
{
   int error = -1; /* EINVAL */

   if( !node || !fill || !fill[0] ) return( error );
   error = 0;

   if( ((__svg_text *)node)->fill )
      free( ((__svg_text *)node)->fill );
   ((__svg_text *)node)->fill = __svg_str_dup( fill );
   if( !((__svg_text *)node)->fill ) error = 1;

   return( error );
}

int __svg_set_text_fill_rgb( node, r, g, b )
   struct __svg_node *node;
   int                r, g, b;
{
   char buf[256];
   int  error = -1; /* EINVAL */

   if( !node ) return( error );
   error = 0;

   sprintf( buf, "rgb(%03d,%03d,%03d)", r, g, b );

   if( ((__svg_text *)node)->fill )
      free( ((__svg_text *)node)->fill );
   ((__svg_text *)node)->fill = __svg_str_dup( buf );
   if( !((__svg_text *)node)->fill ) error = 1;

   return( error );
}

int __svg_set_text_font_family( node, family )
   struct __svg_node *node;
   const char        *family;
{
   int error = -1; /* EINVAL */

   if( !node || !family || !family[0] ) return( error );
   error = 0;

   if( ((__svg_text *)node)->font_family )
      free( ((__svg_text *)node)->font_family );
   ((__svg_text *)node)->font_family = __svg_str_dup( family );
   if( !((__svg_text *)node)->font_family ) error = 1;

   return( error );
}


int __svg_set_text_font_size( node, size )
   struct __svg_node *node;
   int                size;
{
   int  error = -1; /* EINVAL */

   if( !node ) return( error );
   error = 0;

   ((__svg_text *)node)->font_size = size;
   error = 1;

   return( error );
}



void __svg_free_scene( scene )
   struct __svg_scene *scene;
{
   if( !scene ) return;
   if( scene->plist )
   {
      /* clear svgc_dlist */
      free_svgc_dlist( scene->plist, __svg_free_node );
   }

   free( scene );
}


struct __svg_scene *
__svg_new_scene( sheet )
   struct __svg_sheet *sheet;
{
   struct __svg_scene     *new = (struct __svg_scene *)NULL;
   struct svgc_dlist_node *np = NULL;

   new = (struct __svg_scene *)malloc( sizeof(struct __svg_scene) );
   if( new == (struct __svg_scene *)NULL ) return( (struct __svg_scene *)NULL );
   memset( (void *)new, 0, sizeof(struct __svg_scene) );

   new->plist = create_svgc_dlist();
   if( !new->plist )
   {
      free( new );
      return( (struct __svg_scene *)NULL );
   }

   np = add_to_svgc_dlist_down( new->plist, sheet->id, (void *)sheet );

   return( (struct __svg_scene *)new );
}

int
__svg_scene_add_node( scene, node )
   struct __svg_scene *scene;
   struct __svg_node  *node;
{
   struct svgc_dlist_node *np = NULL;
   int error = -1; /* EINVAL */

   if( !scene || !scene->plist || !node ) return( error );
   error = 0;

   if( !scene->plist ) return( error );

   np = add_to_svgc_dlist_down( scene->plist, node->id, (void *)node );
   if( np ) error = 1;

   return( error );
}


/* Get node ID: */
const char *
__svg_get_node_id( node )
   struct __svg_node *node;
{
   const char *error = (const char *)NULL;

   if( !node->id || !node->id[0] ) return( error );
   
   return( (const char *)node->id );
}

/* Create node URL: */
char *
__svg_create_node_url( node )
   struct __svg_node *node;
{
   int   len;
   char *url = (char *)NULL;

   if( !node->id || !node->id[0] ) return( url );

   len = strlen( node->id ) + 7;
   url = (char *)malloc( len );
   if( !url ) return( url );

   sprintf( url, "url(#%s)", node->id );

   return( url );
}


/***************************************************************
  OUTPUT:
 ***************************************************************/
int   output_group = 0;
char *output_file_name;
FILE *output_file;

void 
__svg_paint_point
( data )
   void *data;
{
   struct __svg_point *p = (struct __svg_point *)data;

   if( !p ) return;

   fprintf( output_file, " %0.3f,%0.3f", p->x, p->y );
}

void 
__svg_paint_point_x
( data )
   void *data;
{
   struct __svg_point *p = (struct __svg_point *)data;

   if( !p ) return;

   fprintf( output_file, " %0.3f", p->x );
}

void 
__svg_paint_point_y
( data )
   void *data;
{
   struct __svg_point *p = (struct __svg_point *)data;

   if( !p ) return;

   fprintf( output_file, " %0.3f", p->y );
}

void 
__svg_paint_point_bool
( data )
   void *data;
{
   struct __svg_point *p = (struct __svg_point *)data;

   if( !p ) return;

   if( p->x < 0.5 ) fprintf( output_file, " 0," );
   else             fprintf( output_file, " 1," );
   if( p->y < 0.5 ) fprintf( output_file,  "0"  );
   else             fprintf( output_file,  "1"  );
}



void 
__svg_paint_cmd
( data )
   void *data;
{
   struct __svg_cmd   *cmd = (struct __svg_cmd *)data;

   if( !cmd ) return;
   switch( cmd->cmd )
   {
      case 'M':
      case 'm':
         fprintf( output_file, "%c", cmd->cmd );
         __svg_paint_point( lookup_svgc_dlist_node_down( cmd->points->plist, "1" )->data );
         break;

      case 'L':
      case 'l':
         fprintf( output_file, " %c", cmd->cmd );
         __svg_paint_point( lookup_svgc_dlist_node_down( cmd->points->plist, "1" )->data );
         break;

      case 'H':
      case 'h':
         fprintf( output_file, "%c", cmd->cmd );
         __svg_paint_point_x( lookup_svgc_dlist_node_down( cmd->points->plist, "1" )->data );
         break;

      case 'V':
      case 'v':
         fprintf( output_file, "%c", cmd->cmd );
         __svg_paint_point_y( lookup_svgc_dlist_node_down( cmd->points->plist, "1" )->data );
         break;

      case 'C':
      case 'c':
         fprintf( output_file, " %c", cmd->cmd );
         __svg_paint_point( lookup_svgc_dlist_node_down( cmd->points->plist, "1" )->data );
         __svg_paint_point( lookup_svgc_dlist_node_down( cmd->points->plist, "2" )->data );
         __svg_paint_point( lookup_svgc_dlist_node_down( cmd->points->plist, "3" )->data );
         break;

      case 'S':
      case 's':
         fprintf( output_file, " %c", cmd->cmd );
         __svg_paint_point( lookup_svgc_dlist_node_down( cmd->points->plist, "1" )->data );
         __svg_paint_point( lookup_svgc_dlist_node_down( cmd->points->plist, "2" )->data );
         break;

      case 'Q':
      case 'q':
         fprintf( output_file, " %c", cmd->cmd );
         __svg_paint_point( lookup_svgc_dlist_node_down( cmd->points->plist, "1" )->data );
         __svg_paint_point( lookup_svgc_dlist_node_down( cmd->points->plist, "2" )->data );
         break;

      case 'T':
      case 't':
         fprintf( output_file, " %c", cmd->cmd );
         __svg_paint_point( lookup_svgc_dlist_node_down( cmd->points->plist, "1" )->data );
         break;

      case 'A':
      case 'a':
         fprintf( output_file, " %c", cmd->cmd );
         __svg_paint_point( lookup_svgc_dlist_node_down( cmd->points->plist, "1" )->data );
         __svg_paint_point_x( lookup_svgc_dlist_node_down( cmd->points->plist, "2" )->data );
         __svg_paint_point_bool( lookup_svgc_dlist_node_down( cmd->points->plist, "3" )->data );
         __svg_paint_point( lookup_svgc_dlist_node_down( cmd->points->plist, "4" )->data );
         break;

      case 'Z':
      case 'z':
         fprintf( output_file, " %c", cmd->cmd );
         break;
      default:
         break;
   }
}


void 
__svg_paint_stop
( data )
   void *data;
{
   struct __svg_node *node = (struct __svg_node *)data;

   if( !node ) return;


   fprintf( output_file, 
"      <stop id=\"%s\" offset=\"%0.3f\" style=\"stop-color:%s;stop-opacity:%0.3f;\" />\n",
         node->id, 
         ((struct __svg_stop *)node)->offset,
         node->style->stop_color->use_stop_color,
         node->style->stop_opacity->stop_opacity );
}


void 
__svg_paint_l_gradient
( data )
   void *data;
{
   struct __svg_node *node = (struct __svg_node *)data;

   if( !node ) return;

   fprintf( output_file, 
"    <linearGradient\n"
"          id=\"%s\" gradientUnits=\"%s\" spreadMethod=\"%s\"\n"
"          x1=\"%0.3f\"\n"
"          y1=\"%0.3f\"\n"
"          x2=\"%0.3f\"\n"
"          y2=\"%0.3f\">\n",
         node->id, 
         ((struct __svg_l_gradient *)node)->units,
         ((struct __svg_l_gradient *)node)->spread,
         ((struct __svg_l_gradient *)node)->x1,
         ((struct __svg_l_gradient *)node)->y1,
         ((struct __svg_l_gradient *)node)->x2,
         ((struct __svg_l_gradient *)node)->y2 );

   if( ((struct __svg_l_gradient *)node)->stops )
   {
      /* paint svgc_dlist */
      for_each_svgc_dlist_node_data( 
         ((struct __svg_l_gradient *)node)->stops, __svg_paint_stop );
   }

   fprintf( output_file, 
"    </linearGradient>\n" );
}

void 
__svg_paint_r_gradient
( data )
   void *data;
{
   struct __svg_node *node = (struct __svg_node *)data;

   if( !node ) return;

   fprintf( output_file, 
"    <radialGradient\n"
"          id=\"%s\" gradientUnits=\"%s\" spreadMethod=\"%s\"\n"
"          cx=\"%0.3f\"\n"
"          cy=\"%0.3f\"\n"
"           r=\"%0.3f\"\n"
"          fx=\"%0.3f\"\n"
"          fy=\"%0.3f\">\n",
         node->id, 
         ((struct __svg_r_gradient *)node)->units,
         ((struct __svg_r_gradient *)node)->spread,
         ((struct __svg_r_gradient *)node)->cx,
         ((struct __svg_r_gradient *)node)->cy,
         ((struct __svg_r_gradient *)node)->r,
         ((struct __svg_r_gradient *)node)->fx,
         ((struct __svg_r_gradient *)node)->fy );

   if( ((struct __svg_r_gradient *)node)->stops )
   {
      /* paint svgc_dlist */
      for_each_svgc_dlist_node_data( 
         ((struct __svg_r_gradient *)node)->stops, __svg_paint_stop );
   }

   fprintf( output_file, 
"    </radialGradient>\n" );
}


void 
__svg_paint_defs( data )
   void *data;
{
   struct __svg_node *node = (struct __svg_node *)data;

   if( !node ) return;

   switch( node->type )
   {
      default:
         fprintf( output_file, "<node type=\"unknown\" />\n" );
         break;

      case SVG_L_GRADIENT:
         __svg_paint_l_gradient( data );
         break;

      case SVG_R_GRADIENT:
         __svg_paint_r_gradient( data );
         break;
   }
}


void 
__svg_paint_node( data )
   void *data;
{
   struct __svg_node *node = (struct __svg_node *)data;
   char              *group;

   if( !node ) return;

   switch( node->type )
   {
      default:
         fprintf( output_file, "<node type=\"unknown\" />\n" );
         break;

      case SVG_SHEET    :
         if( output_group ) group = "  <g id=\"layer0\">\n";
         else               group = "";
         fprintf( output_file, 
"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n" 
"<!-- Created with __svg_lib (http://www._kxLab.com/) -->\n"
"<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\n"
"  \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n"
"<svg\n"
"    xmlns:svg=\"http://www.w3.org/2000/svg\"\n"
"  xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n"
"        xmlns=\"http://www.w3.org/2000/svg\"\n"
"      viewBox=\"0 0 %d %d\"\n"
"      version=\"1.1\">\n"
"  <defs>\n"
"    <sheet      id=\"%s\"\n"
"       left_margin=\"%d\"\n"
"      right_margin=\"%d\"\n"
"        top_margin=\"%d\"\n"
"     bottom_margin=\"%d\" />\n",
         (int)((struct __svg_sheet *)node)->width,
         (int)((struct __svg_sheet *)node)->height,
         node->id, 
         (int)((struct __svg_sheet *)node)->left_margin,
         (int)((struct __svg_sheet *)node)->right_margin,
         (int)((struct __svg_sheet *)node)->top_margin,
         (int)((struct __svg_sheet *)node)->bottom_margin );

         if( ((struct __svg_sheet *)node)->defs )
         {
            /* paint svgc_dlist */
            for_each_svgc_dlist_node_data( 
               ((struct __svg_sheet *)node)->defs, __svg_paint_defs );
         }

         fprintf( output_file, 
"  </defs>\n%s",
         group );
         break;

      case SVG_LINE     :
         fprintf( output_file, 
"    <line\n"
"          id=\"%s\"\n"
"          x1=\"%0.3f\"\n"
"          y1=\"%0.3f\"\n"
"          x2=\"%0.3f\"\n"
"          y2=\"%0.3f\"\n"
"       style=\"stroke:%s;stroke-width:%0.3f;stroke-linecap:%s;stroke-linejoin:%s;stroke-dasharray:%s;\" />\n",
         node->id, 
         ((struct __svg_line *)node)->x1,
         ((struct __svg_line *)node)->y1,
         ((struct __svg_line *)node)->x2,
         ((struct __svg_line *)node)->y2,
         node->style->stroke->use_stroke,
         node->style->stroke->stroke_width,
         node->style->stroke->stroke_linecap,
         node->style->stroke->stroke_linejoin,
         node->style->stroke->stroke_dasharray );
         break;

         break;
      case SVG_RECT     :
         fprintf( output_file, 
"    <rect\n"
"          id=\"%s\"\n"
"           x=\"%0.3f\"\n"
"           y=\"%0.3f\"\n"
"       width=\"%0.3f\"\n"
"      height=\"%0.3f\"\n"
"          rx=\"%0.3f\"\n"
"          ry=\"%0.3f\"\n"
"       style=\"fill:%s;stroke:%s;stroke-width:%0.3f;stroke-linecap:%s;stroke-linejoin:%s;stroke-dasharray:%s;\" />\n",
         node->id, 
         ((struct __svg_rect *)node)->x,
         ((struct __svg_rect *)node)->y,
         ((struct __svg_rect *)node)->width,
         ((struct __svg_rect *)node)->height,
         ((struct __svg_rect *)node)->rx,
         ((struct __svg_rect *)node)->ry,
         node->style->fill->use_fill,
         node->style->stroke->use_stroke,
         node->style->stroke->stroke_width,
         node->style->stroke->stroke_linecap,
         node->style->stroke->stroke_linejoin,
         node->style->stroke->stroke_dasharray );
         break;
         break;

      case SVG_CIRCLE   :
         fprintf( output_file, 
"    <circle\n"
"          id=\"%s\"\n"
"          cx=\"%0.3f\"\n"
"          cy=\"%0.3f\"\n"
"           r=\"%0.3f\"\n"
"       style=\"fill:%s;stroke:%s;stroke-width:%0.3f;stroke-linecap:%s;stroke-linejoin:%s;stroke-dasharray:%s;\" />\n",
         node->id, 
         ((struct __svg_circle *)node)->cx,
         ((struct __svg_circle *)node)->cy,
         ((struct __svg_circle *)node)->r,
         node->style->fill->use_fill,
         node->style->stroke->use_stroke,
         node->style->stroke->stroke_width,
         node->style->stroke->stroke_linecap,
         node->style->stroke->stroke_linejoin,
         node->style->stroke->stroke_dasharray );
         break;

      case SVG_ELLIPSE  :
         fprintf( output_file, 
"    <ellipse\n"
"          id=\"%s\"\n"
"          cx=\"%0.3f\"\n"
"          cy=\"%0.3f\"\n"
"          rx=\"%0.3f\"\n"
"          ry=\"%0.3f\"\n"
"       style=\"fill:%s;stroke:%s;stroke-width:%0.3f;stroke-linecap:%s;stroke-linejoin:%s;stroke-dasharray:%s;\" />\n",
         node->id, 
         ((struct __svg_ellipse *)node)->cx,
         ((struct __svg_ellipse *)node)->cy,
         ((struct __svg_ellipse *)node)->rx,
         ((struct __svg_ellipse *)node)->ry,
         node->style->fill->use_fill,
         node->style->stroke->use_stroke,
         node->style->stroke->stroke_width,
         node->style->stroke->stroke_linecap,
         node->style->stroke->stroke_linejoin,
         node->style->stroke->stroke_dasharray );
         break;

      case SVG_POLYGON  :
         fprintf( output_file, 
"    <polygon\n"
"          id=\"%s\"\n"
"      points=\"",
         node->id );
         /* print points */
         for_each_svgc_dlist_node_data( ((struct __svg_polygon *)node)->points->plist, __svg_paint_point );
         fprintf( output_file, 
"\"\n"
"       style=\"fill:%s;stroke:%s;stroke-width:%0.3f;stroke-linecap:%s;stroke-linejoin:%s;stroke-dasharray:%s;\" />\n",
         node->style->fill->use_fill,
         node->style->stroke->use_stroke,
         node->style->stroke->stroke_width,
         node->style->stroke->stroke_linecap,
         node->style->stroke->stroke_linejoin,
         node->style->stroke->stroke_dasharray );
         break;

      case SVG_POLYLINE :
         fprintf( output_file, 
"    <polyline\n"
"          id=\"%s\"\n"
"      points=\"",
         node->id );
         /* print points */
         for_each_svgc_dlist_node_data( ((struct __svg_polygon *)node)->points->plist, __svg_paint_point );
         fprintf( output_file, 
"\"\n"
"       style=\"fill:%s;stroke:%s;stroke-width:%0.3f;stroke-linecap:%s;stroke-linejoin:%s;stroke-dasharray:%s;\" />\n",
         node->style->fill->use_fill,
         node->style->stroke->use_stroke,
         node->style->stroke->stroke_width,
         node->style->stroke->stroke_linecap,
         node->style->stroke->stroke_linejoin,
         node->style->stroke->stroke_dasharray );
         break;

      case SVG_PATH     :
         fprintf( output_file, 
"    <path\n"
"          id=\"%s\"\n"
"           d=\"",
         node->id );
         /* print points */
         for_each_svgc_dlist_node_data( ((struct __svg_path *)node)->cmds->plist, __svg_paint_cmd );
         fprintf( output_file, 
"\"\n"
"       style=\"fill:%s;stroke:%s;stroke-width:%0.3f;stroke-linecap:%s;stroke-linejoin:%s;stroke-dasharray:%s;\" />\n",
         node->style->fill->use_fill,
         node->style->stroke->use_stroke,
         node->style->stroke->stroke_width,
         node->style->stroke->stroke_linecap,
         node->style->stroke->stroke_linejoin,
         node->style->stroke->stroke_dasharray );
         break;

      case SVG_TEXT     :
         fprintf( output_file, 
"    <text\n"
"          id=\"%s\"\n"
"           x=\"%0.3f\"\n"
"           y=\"%0.3f\"\n"
" font-family=\"%s\"\n"
"   font-size=\"%d\"\n"
"        fill=\"%s\" >\n"
"      %s\n"
"    </text>\n",
         node->id, 
         ((struct __svg_text *)node)->x,
         ((struct __svg_text *)node)->y,
         ((struct __svg_text *)node)->font_family,
         ((struct __svg_text *)node)->font_size,
         ((struct __svg_text *)node)->fill,
         ((struct __svg_text *)node)->text );
         break;

      case SVG_IMAGE    :
         fprintf( output_file,
"    <image\n"
"          id=\"%s\"\n"
"           x=\"%0.3f\"\n"
"           y=\"%0.3f\"\n"
"       width=\"%dpx\"\n"
"      height=\"%dpx\"\n"
"  xlink:href=\"%s\">\n",
         node->id, 
         ((struct __svg_image *)node)->x,
         ((struct __svg_image *)node)->y,
         ((struct __svg_image *)node)->width,
         ((struct __svg_image *)node)->height,
         ((struct __svg_image *)node)->href );
         fprintf( output_file,
"      <title>%s</title>\n",
         ((struct __svg_image *)node)->title );
         fprintf( output_file,
"    </image>\n" );
         break;
   }
}


void __svg_paint_scene( scene, group, fname )
   struct __svg_scene *scene;
   int                 group;
   char               *fname;
{
   int save_group = output_group;

   if( !scene ) return;

   if( group ) output_group = 1;
   else        output_group = 0;

   if( !fname || !fname[0] )
   {
      output_file_name = "stdout";
      output_file      = stdout;
   }
   else
   {
      output_file_name = fname;
      /* open file output_file */
      if( (output_file = fopen( output_file_name, "w+" )) == NULL )
      {
         fprintf( stderr, "ERROR: Cannot open file: %s\n", output_file_name );
         output_group = save_group;
         exit( 1 );
      }
   }

   if( scene->plist )
   {
      /* paint svgc_dlist */
      for_each_svgc_dlist_node_data( scene->plist, __svg_paint_node );
   }

   if( output_group )
      fprintf( output_file, "  </g>\n" );
   fprintf( output_file, "</svg>\n" );

   output_group = save_group;

   fclose( output_file );
}

