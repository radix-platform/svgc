
/***************************************************************
  SVGC.H

       This file containt declarations of SVG C lib functions.

 ***************************************************************/
/*
  This file contant RUSSIAN letters( code-page: iso-8859-5 )
 ***************************************************************/

#ifndef __SVGC_H__
#define __SVGC_H__

#if defined( __cplusplus )
extern "C"
{
#endif

#include <svgc-dlist.h>

typedef struct __svg_point __svg_point;
struct __svg_point
{
   double x, y;
};

typedef struct __svg_points __svg_points;
struct __svg_points
{
   int                count;
   struct svgc_dlist *plist;
};

extern void __svg_free_point( void *data );
extern void __svg_free_points( struct __svg_points *p );
extern struct __svg_points *__svg_new_points( void );
extern int __svg_add_point( struct __svg_points *ps, double x, double y );


/***************************************************************
  PATH CMDS:
 ***************************************************************/

typedef struct __svg_cmd __svg_cmd;
struct __svg_cmd
{
   char                 cmd;
   struct __svg_points *points;
};

typedef struct __svg_cmds __svg_cmds;
struct __svg_cmds
{
   int                count;
   struct svgc_dlist *plist;
};

extern void __svg_free_cmd( void *data );
extern void __svg_free_cmds( struct __svg_cmds *cmds );
extern struct __svg_cmd *__svg_new_cmd( char cmd, struct __svg_points *ps );
extern struct __svg_cmds *__svg_new_cmds( void );
extern int __svg_add_cmd( struct __svg_cmds *cmds, struct __svg_cmd *cmd );

#define ABSOLUTE 1
#define RELATIVE 0
extern int 
__svg_add_moveto_cmd( struct __svg_cmds *cmds,
                      int                abs,
                      double             x, 
                      double             y );

extern int 
__svg_add_lineto_cmd( struct __svg_cmds *cmds,
                      int                abs,
                      double             x, 
                      double             y );

extern int 
__svg_add_hlineto_cmd( struct __svg_cmds *cmds,
                       int                abs,
                       double             x );

extern int 
__svg_add_vlineto_cmd( struct __svg_cmds *cmds,
                       int                abs,
                       double             y );

extern int 
__svg_add_curveto_cmd( struct __svg_cmds *cmds,
                       int                abs,
                       double             x1,
                       double             y1,
                       double             x2,
                       double             y2,
                       double             x,
                       double             y );
extern int 
__svg_add_short_curveto_cmd( struct __svg_cmds *cmds,
                             int                abs,
                             double             x2,
                             double             y2,
                             double             x,
                             double             y );
extern int 
__svg_add_quadro_curveto_cmd( struct __svg_cmds *cmds,
                              int                abs,
                              double             x1,
                              double             y1,
                              double             x,
                              double             y );
extern int 
__svg_add_short_quadro_curveto_cmd( struct __svg_cmds *cmds,
                                    int                abs,
                                    double             x,
                                    double             y );

extern int 
__svg_add_arc_cmd( struct __svg_cmds *cmds,
                   int                abs,
                   double             rx,
                   double             ry,
                   double             x_axis_rotation,
                   int                large_arc_flag,
                   int                sweep_flag,
                   double             x,
                   double             y );

extern int 
__svg_add_closepath_cmd( struct __svg_cmds *cmds,
                         int                abs );



/***************************************************************
  STYLE:
 ***************************************************************/
typedef struct __svg_rgb __svg_rgb;
struct __svg_rgb
{
   int r, g, b;
};


#define STROKE_DEFAULT    "black"

#define LINECAP_BUTT      "butt"
#define LINECAP_ROUND     "round"
#define LINECAP_SQUARE    "square"

#define LINECAP_DEFAULT   "round"


#define LINEJOIN_MITER    "miter"
#define LINEJOIN_ROUND    "round"
#define LINEJOIN_BEVEL    "bevel"

#define LINEJOIN_DEFAULT  "round"


#define DASHARRAY_DEFAULT "none"

typedef struct __svg_stroke __svg_stroke;
struct __svg_stroke
{
   struct __svg_rgb  stroke;
   char             *use_stroke;

   double            stroke_width;

   char             *stroke_linecap;
   char             *stroke_linejoin;
   char             *stroke_dasharray;
};


#define FILL_DEFAULT "none"

typedef struct __svg_fill __svg_fill;
struct __svg_fill
{
   struct __svg_rgb  fill;
   char             *use_fill;
};


#define STOP_COLOR_DEFAULT "black"

typedef struct __svg_stop_color __svg_stop_color;
struct __svg_stop_color
{
   struct __svg_rgb  stop_color;
   char             *use_stop_color;
};


#define STOP_OPACITY_DEFAULT 1.0

typedef struct __svg_stop_opacity __svg_stop_opacity;
struct __svg_stop_opacity
{
   double stop_opacity;
};


typedef struct __svg_style __svg_style;
struct __svg_style
{
   struct __svg_stroke       *stroke;
   struct __svg_fill         *fill;
   struct __svg_stop_color   *stop_color;
   struct __svg_stop_opacity *stop_opacity;
};


#define TEXT_FILL_DEFAULT          "black"
#define TEXT_FONT_FAMILY_DEFAULT   "Verdana"
#define TEXT_FONT_SIZE_DEFAULT     12


extern char *__svg_str_dup( const char *s );

extern struct __svg_stroke *__svg_new_stroke( void );
extern void __svg_free_stroke( struct __svg_stroke *s );
extern struct __svg_fill *__svg_new_fill( void );
extern void __svg_free_fill( struct __svg_fill *f );
extern struct __svg_stop_color *__svg_new_stop_color( void );
extern void __svg_free_stop_color( struct __svg_stop_color *sc );
extern struct __svg_stop_opacity *__svg_new_stop_opacity( void );
extern void __svg_free_stop_opacity( struct __svg_stop_opacity *so );

extern struct __svg_style *__svg_new_style( void );
extern void __svg_free_style( struct __svg_style *style );




/* NODE: */

#define SVG_SHEET        1
#define SVG_LINE         2
#define SVG_RECT         3
#define SVG_CIRCLE       4
#define SVG_ELLIPSE      5
#define SVG_POLYGON      6
#define SVG_POLYLINE     7
#define SVG_PATH         8
#define SVG_TEXT         9
#define SVG_IMAGE       10
#define SVG_L_GRADIENT  11
#define SVG_R_GRADIENT  12
#define SVG_STOP        14

typedef struct __svg_node __svg_node;
struct __svg_node
{
   int                 type;
   char               *id;
   struct __svg_style *style;
};

/* The MAIN svg nide is SHEET: */
typedef struct __svg_sheet __svg_sheet;
struct __svg_sheet
{
   int                 type;
   char               *id;
   struct __svg_style *style;

   struct svgc_dlist  *defs;

   double width, height;

   double   left_margin;
   double  right_margin;
   double    top_margin;
   double bottom_margin;
};


/***************************************************************
  SVG primitives:
 ***************************************************************/
typedef struct __svg_line __svg_line;
struct __svg_line
{
   int                 type;
   char               *id;
   struct __svg_style *style;

   double x1, y1;
   double x2, y2;
};

typedef struct __svg_rect __svg_rect;
struct __svg_rect
{
   int                 type;
   char               *id;
   struct __svg_style *style;

   double x, y;
   double width, height;
   double rx, ry;
};

typedef struct __svg_circle  __svg_circle;
struct __svg_circle
{
   int                 type;
   char               *id;
   struct __svg_style *style;

   double cx, cy, r;
};

typedef struct __svg_ellipse __svg_ellipse;
struct __svg_ellipse
{
   int                 type;
   char               *id;
   struct __svg_style *style;

   double  cx, cy, rx, ry;
};

typedef struct __svg_polygon __svg_polygon;
struct __svg_polygon
{
   int                 type;
   char               *id;
   struct __svg_style *style;

   struct __svg_points *points;
};

typedef struct __svg_polyline __svg_polyline;
struct __svg_polyline
{
   int                 type;
   char               *id;
   struct __svg_style *style;

   struct __svg_points *points;
};

typedef struct __svg_path __svg_path;
struct __svg_path
{
   int                 type;
   char               *id;
   struct __svg_style *style;

   struct __svg_cmds  *cmds;
};

typedef struct __svg_text __svg_text;
struct __svg_text
{
   int                 type;
   char               *id;
   struct __svg_style *style;

   double  x, y;
   char   *text;

   int     font_size;
   char   *font_family;
   char   *fill;
};

typedef struct __svg_image __svg_image;
struct __svg_image
{
   int                 type;
   char               *id;
   struct __svg_style *style;

   double  x, y;
   int     width, height;
   char   *href;
   char   *title;
};

/* NOTE: this is common gradient struct */
typedef struct __svg_gradient __svg_gradient;
struct __svg_gradient
{
   int                 type;
   char               *id;
   struct __svg_style *style;

   struct svgc_dlist  *stops;
};

/* NOTE: gradients are added into sheet list as defs */
typedef struct __svg_l_gradient __svg_l_gradient;
struct __svg_l_gradient
{
   int                 type;
   char               *id;
   struct __svg_style *style;

   struct svgc_dlist  *stops;

   char   *units;

   double  x1, y1;
   double  x2, y2;
};

/* NOTE: only for gradients */
typedef struct __svg_stop __svg_stop;
struct __svg_stop
{
   int                 type;
   char               *id;
   struct __svg_style *style;

   double offset;
};


/***************************************************************
  SCENE is the list of SVG primitives.
 ***************************************************************/
typedef struct __svg_scene __svg_scene;
struct __svg_scene
{
   struct svgc_dlist *plist;
};

extern void __svg_free_scene( struct __svg_scene *scene );
extern struct __svg_scene *__svg_new_scene( struct __svg_sheet *sheet );

/***************************************************************
  FREE and ADD SVG primitives.
 ***************************************************************/
extern void __svg_free_node( void *data );

/* SVG sheet should be created before SCENE */
extern struct __svg_node *__svg_new_sheet( double width,
                                           double height,
                                           double   left_margin,
                                           double  right_margin,
                                           double    top_margin,
                                           double bottom_margin );

extern int __svg_sheet_add_definition( void              *sheet,
                                       struct __svg_node *definition );

/***************************************************************
  Creation of SVG primirives:
 ***************************************************************/
extern struct __svg_node *__svg_new_line( double x1,
                                          double y1,
                                          double x2,
                                          double y2 );

extern struct __svg_node *__svg_new_rect( double x,
                                          double y,
                                          double width,
                                          double height,
                                          double rx,
                                          double ry );

extern struct __svg_node *__svg_new_circle( double cx,
                                            double cy,
                                            double r );

extern struct __svg_node *__svg_new_ellipse( double cx,
                                             double cy,
                                             double rx,
                                             double ry );

extern struct __svg_node *__svg_new_polygon ( struct __svg_points *ps );
extern struct __svg_node *__svg_new_polyline( struct __svg_points *ps );
extern struct __svg_node *__svg_new_path( struct __svg_cmds *cmds );

extern struct __svg_node *__svg_new_text( double      x,
                                          double      y,
                                          const char *text,
                                          const char *font_family,
                                          int         font_size );

extern struct __svg_node *__svg_new_image( double  x,
                                           double  y,
                                           int     width,
                                           int     height,
                                           char   *href,
                                           char   *title );

extern struct __svg_node *__svg_new_l_gradient( const char *units,
                                                double      x1,
                                                double      y1,
                                                double      x2,
                                                double      y2 );

extern struct __svg_node *__svg_new_stop( double offset );

extern int __svg_gradient_add_stop( void              *gradient,
                                    struct __svg_stop *stop );


/***************************************************************
  Adding some primitive to SCENE:
 ***************************************************************/
extern int
__svg_scene_add_node( struct __svg_scene *scene, struct __svg_node *node );


/***************************************************************
  Style controls:
 ***************************************************************/
extern int __svg_set_stroke( struct __svg_node *node, const char *stroke );
extern int __svg_set_stroke_rgb( struct __svg_node *node, int r, int g, int b );
extern int __svg_set_stroke_width( struct __svg_node *node, double width );
extern int __svg_set_stroke_linecap( struct __svg_node *node, const char *linecap );
extern int __svg_set_stroke_linejoin( struct __svg_node *node, const char *linejoin );
extern int __svg_set_stroke_dasharray( struct __svg_node *node, const char *dasharray );

extern int __svg_set_fill( struct __svg_node *node, const char *fill );
extern int __svg_set_fill_rgb( struct __svg_node *node, int r, int g, int b );

extern int __svg_set_stop_color( struct __svg_node *node, const char *stop_color );
extern int __svg_set_stop_color_rgb( struct __svg_node *node, int r, int g, int b );

extern int __svg_set_stop_opacity( struct __svg_node *node, double stop_opacity );

extern int __svg_set_text_text( struct __svg_node *node, const char *text );
extern int __svg_set_text_fill( struct __svg_node *node, const char *fill );
extern int __svg_set_text_fill_rgb( struct __svg_node *node, int r, int g, int b );
extern int __svg_set_text_font_family( struct __svg_node *node, const char *family );
extern int __svg_set_text_font_size( struct __svg_node *node, int size );


/* Set Global Default Values: */
extern void __svg_set_scene_default_style( void );
extern void __svg_set_scene_stroke( const char *stroke );
extern void __svg_set_scene_stroke_rgb( int r, int g, int b );
extern void __svg_set_scene_stroke_width( double width );
extern void __svg_set_scene_stroke_linecap( const char *linecap );
extern void __svg_set_scene_stroke_linejoin( const char *linejoin );
extern void __svg_set_scene_stroke_dasharray( const char *dasharray );

extern void __svg_set_scene_fill( const char *fill );
extern void __svg_set_scene_fill_rgb( int r, int g, int b );

extern void __svg_set_scene_stop_color( const char *stop_color );
extern void __svg_set_scene_stop_color_rgb( int r, int g, int b );

extern void __svg_set_scene_stop_opacity( double stop_opacity );

extern void __svg_set_scene_text_fill( const char *fill );
extern void __svg_set_scene_text_fill_rgb( int r, int g, int b );
extern void __svg_set_scene_text_font_family( const char *family );
extern void __svg_set_scene_text_font_size( int size );


/* Get node ID: */
extern const char *__svg_get_node_id( struct __svg_node *node );
extern char *__svg_create_node_url( struct __svg_node *node );


/***************************************************************
  OUTPUT:
 ***************************************************************/
extern void __svg_paint_scene( struct __svg_scene *scene,
                               int                 group,
                               char               *fname );


extern int _svgc_get_major_version( void );
extern int _svgc_get_minor_version( void );
extern int _svgc_get_micro_version( void );

#if defined( __cplusplus )
} /* extern "C" */
#endif

#endif /* __SVGC_H__ */
