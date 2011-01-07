

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <math.h>

#include <svgc.h>

struct __svg_sheet   *sheet;
struct __svg_line    *line;
struct __svg_rect    *rect;
struct __svg_circle  *circle;
struct __svg_ellipse *ellipse;
struct __svg_text    *text;

struct __svg_points *points;
struct __svg_polygon *polygon;
struct __svg_polyline *polyline;

struct __svg_cmd *cmd;
struct __svg_cmds *cmds;
struct __svg_path *path;


struct __svg_scene *scene;

void arrow( double sx0, double sy0, double f0 )
{
   double sx1, sy1, sx2, sy2, sx3, sy3, sx4, sy4;
   double px1, py1, px2, py2, px3, py3, px4, py4;

   sx1 = sx0 - 3.0;
   sy1 = sy0 + 3.0;
   sx2 = sx0;
   sy2 = sy0 - 6.0;
   sx3 = sx0 + 3.0;
   sy3 = sy0 + 3.0;
   sx4 = sx0;
   sy4 = sy0 + 1.0;

   px1 = sx0 + ( sx1 - sx0 )*cos( f0 ) - ( sy1 - sy0 )*sin( f0 );
   py1 = sy0 + ( sx1 - sx0 )*sin( f0 ) + ( sy1 - sy0 )*cos( f0 );
   px2 = sx0 + ( sx2 - sx0 )*cos( f0 ) - ( sy2 - sy0 )*sin( f0 );
   py2 = sy0 + ( sx2 - sx0 )*sin( f0 ) + ( sy2 - sy0 )*cos( f0 );
   px3 = sx0 + ( sx3 - sx0 )*cos( f0 ) - ( sy3 - sy0 )*sin( f0 );
   py3 = sy0 + ( sx3 - sx0 )*sin( f0 ) + ( sy3 - sy0 )*cos( f0 );
   px4 = sx0 + ( sx4 - sx0 )*cos( f0 ) - ( sy4 - sy0 )*sin( f0 );
   py4 = sy0 + ( sx4 - sx0 )*sin( f0 ) + ( sy4 - sy0 )*cos( f0 );

   cmds = __svg_new_cmds();
   __svg_add_moveto_cmd( cmds, ABSOLUTE, px1, py1 );
   __svg_add_lineto_cmd( cmds, ABSOLUTE, px2, py2 );
   __svg_add_lineto_cmd( cmds, ABSOLUTE, px3, py3 );
   __svg_add_short_curveto_cmd( cmds, ABSOLUTE, px4, py4, px1, py1 );
   __svg_add_closepath_cmd( cmds, ABSOLUTE );
   path = (struct __svg_path *)__svg_new_path( cmds );
   (void)__svg_set_stroke_rgb( (struct __svg_node *)path, 0, 0, 0 );
   (void)__svg_set_stroke_width( (struct __svg_node *)path, 1.0 );
   (void)__svg_set_stroke_linecap( (struct __svg_node *)path, "square" );
   (void)__svg_set_stroke_linejoin( (struct __svg_node *)path, "miter" );
   (void)__svg_set_fill_rgb( (struct __svg_node *)path, 255, 255, 255 );

   (void)__svg_scene_add_node( scene, (struct __svg_node *)path );
}

void small_arrow( double sx0, double sy0, double f0 )
{
   double sx1, sy1, sx2, sy2, sx3, sy3, sx4, sy4;
   double px1, py1, px2, py2, px3, py3, px4, py4;

   sx1 = sx0 - 2.0;
   sy1 = sy0 + 2.0;
   sx2 = sx0;
   sy2 = sy0 - 4.0;
   sx3 = sx0 + 2.0;
   sy3 = sy0 + 2.0;
   sx4 = sx0;
   sy4 = sy0 + 1.0;

   px1 = sx0 + ( sx1 - sx0 )*cos( f0 ) - ( sy1 - sy0 )*sin( f0 );
   py1 = sy0 + ( sx1 - sx0 )*sin( f0 ) + ( sy1 - sy0 )*cos( f0 );
   px2 = sx0 + ( sx2 - sx0 )*cos( f0 ) - ( sy2 - sy0 )*sin( f0 );
   py2 = sy0 + ( sx2 - sx0 )*sin( f0 ) + ( sy2 - sy0 )*cos( f0 );
   px3 = sx0 + ( sx3 - sx0 )*cos( f0 ) - ( sy3 - sy0 )*sin( f0 );
   py3 = sy0 + ( sx3 - sx0 )*sin( f0 ) + ( sy3 - sy0 )*cos( f0 );
   px4 = sx0 + ( sx4 - sx0 )*cos( f0 ) - ( sy4 - sy0 )*sin( f0 );
   py4 = sy0 + ( sx4 - sx0 )*sin( f0 ) + ( sy4 - sy0 )*cos( f0 );

   cmds = __svg_new_cmds();
   __svg_add_moveto_cmd( cmds, ABSOLUTE, px1, py1 );
   __svg_add_lineto_cmd( cmds, ABSOLUTE, px2, py2 );
   __svg_add_lineto_cmd( cmds, ABSOLUTE, px3, py3 );
   __svg_add_short_curveto_cmd( cmds, ABSOLUTE, px4, py4, px1, py1 );
   __svg_add_closepath_cmd( cmds, ABSOLUTE );
   path = (struct __svg_path *)__svg_new_path( cmds );
   (void)__svg_set_stroke_rgb( (struct __svg_node *)path, 0, 0, 0 );
   (void)__svg_set_stroke_width( (struct __svg_node *)path, 0.5 );
   (void)__svg_set_stroke_linecap( (struct __svg_node *)path, "square" );
   (void)__svg_set_stroke_linejoin( (struct __svg_node *)path, "miter" );
   (void)__svg_set_fill_rgb( (struct __svg_node *)path, 255, 255, 255 );

   (void)__svg_scene_add_node( scene, (struct __svg_node *)path );
}


int main( void )
{
   int i;
   double x0 = 100.0,
          y0 = 30.0,
          r0 = 250.0,
          a0 = 60.0 * M_PI / 180.0,
          xM, yM;

   sheet = (struct __svg_sheet *)__svg_new_sheet( 350, 380, 10, 10, 10, 10 );
   if( !sheet )
   {
      printf( "Unable to create sheet\n" );
   }
   (void)__svg_set_stroke_width( (struct __svg_node *)sheet, 1024.1236 );

   scene = __svg_new_scene( sheet );
   if( !scene )
   {
      printf( "Unable to create scene\n" );
   }

   __svg_set_scene_stroke( "black" );
   __svg_set_scene_stroke_width( 1.0 );
   __svg_set_scene_stroke_linecap( LINECAP_SQUARE );


   rect = (struct __svg_rect *)__svg_new_rect( x0-25.0, y0-1.0, 50, 2, 0, 0 );
   (void)__svg_set_fill_rgb( (struct __svg_node *)rect, 0, 0, 0 );
   (void)__svg_scene_add_node( scene, (struct __svg_node *)rect );

   {
      int x1 = x0-24.0, y1 = y0-1.0, x2 = x0-18.0, y2 = y0-7.0;

      for( i = 0; i < 8; ++i )
      {
         line = (struct __svg_line *)__svg_new_line( x1, y1, x2, y2 );
         (void)__svg_scene_add_node( scene, (struct __svg_node *)line );

         x1 += 6; x2 += 6;
      }
   }


   xM = x0 + r0 * cos( a0 );
   yM = y0 + r0 * sin( a0 );

   __svg_set_scene_stroke_width( 0.5 );

   line = (struct __svg_line *)__svg_new_line( x0, y0, xM, yM );
   (void)__svg_scene_add_node( scene, (struct __svg_node *)line );

   /* Set current values for newest objects: */
   __svg_set_scene_stroke_width( 0.5 );
   __svg_set_scene_stroke_rgb( 99, 99, 99 );
   __svg_set_scene_stroke_dasharray( "3,2" );

   line = (struct __svg_line *)__svg_new_line( x0, y0, x0, y0+r0 );
   (void)__svg_scene_add_node( scene, (struct __svg_node *)line );

   {
      double x1, y1, x2, y2, a1 = 45.0, a2 = 75.0;

      a1 = a1 * M_PI / 180.0;
      a2 = a2 * M_PI / 180.0;
      x1 = x0 + r0 * cos(a1);
      y1 = y0 + r0 * sin(a1);
      x2 = x0 - r0 * cos(a2);
      y2 = y0 + r0 * sin(a2);

      cmds = __svg_new_cmds();
      /* M x1,y1 */
      __svg_add_moveto_cmd( cmds, ABSOLUTE, x1, y1 );

      __svg_add_arc_cmd( cmds, 
                         ABSOLUTE,
                            r0, r0, /* rx, ry */
                            0.0,          /* angle  */
                            0,1,          /* lf, sf */
                            x2, y2 );

      path = (struct __svg_path *)__svg_new_path( cmds );
      (void)__svg_scene_add_node( scene, (struct __svg_node *)path );
   }

   {
      double x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6,
             x7, y7, x8, y8, x9, y9, x10, y10, x11, y11, x12, y12;

      double x3_a, y3_a, x11_a, y11_a, x12_a, y12_a;

      double  r1 = 120.0,
              r2 = 100.0,
              r3 =  90.0,
              r4 = 100.0,
              r5 =  90.0,
              r6 =  30.0,
              r7 = 100.0,
              r8 =  30.0,
              r9 =  60.0,
             r10 =  60.0,
             r11, r12;

      /* Set current values for newest objects: */
      __svg_set_scene_stroke_rgb( 99, 99, 99 );
      __svg_set_scene_stroke_width( 0.5 );
      __svg_set_scene_stroke_dasharray( "3,2" );
      __svg_set_scene_fill( "none" );

      x1 = xM - r1 * sin( a0 );
      y1 = yM + r1 * cos( a0 );

      x2 = xM - r2 * sin( a0 );
      y2 = yM + r2 * cos( a0 );

      x3 = xM;
      y3 = yM + r3;

      x3_a = xM;
      y3_a = yM + r3 + 6.0;

      x4 = xM + r4 * cos( a0 );
      y4 = yM + r4 * sin( a0 );

      x5 = xM + r5 * sin( a0 );
      y5 = yM - r5 * cos( a0 );

      x6 = xM + r6 * sin( a0 );
      y6 = yM - r6 * cos( a0 );

      x7 = xM - r7 * cos( a0 );
      y7 = yM - r7 * sin( a0 );

      x8 = xM - r8 * cos( a0 );
      y8 = yM - r8 * sin( a0 );

      x9 = x0 + r9 * cos( a0 );
      y9 = y0 + r9 * sin( a0 );

      x10 = x0;
      y10 = y0 + r10;

      r11 = r3 * cos( a0 );
      x11 = xM - r11 * sin( a0 );
      y11 = yM + r11 * cos( a0 );
      x11_a = xM - (r11+6.0) * sin( a0 );
      y11_a = yM + (r11+6.0) * cos( a0 );


      r12 = r3 * sin( a0 );
      x12 = xM + r12 * cos( a0 );
      y12 = yM + r12 * sin( a0 );
      x12_a = xM + (r12+6.0) * cos( a0 );
      y12_a = yM + (r12+6.0) * sin( a0 );

      line = (struct __svg_line *)__svg_new_line( x1, y1, xM, yM );
      (void)__svg_scene_add_node( scene, (struct __svg_node *)line );

      line = (struct __svg_line *)__svg_new_line( x4, y4, xM, yM );
      (void)__svg_scene_add_node( scene, (struct __svg_node *)line );

      line = (struct __svg_line *)__svg_new_line( x3_a, y3_a, x11_a, y11_a );
      (void)__svg_scene_add_node( scene, (struct __svg_node *)line );

      line = (struct __svg_line *)__svg_new_line( x3_a, y3_a, x12_a, y12_a );
      (void)__svg_scene_add_node( scene, (struct __svg_node *)line );

      __svg_set_scene_stroke( "black" );
      __svg_set_scene_stroke_width( 0.5 );
      __svg_set_scene_stroke_dasharray( "none" );

      cmds = __svg_new_cmds();
      /* M x1,y1 */
      __svg_add_moveto_cmd( cmds, ABSOLUTE, x9, y9 );

      __svg_add_arc_cmd( cmds, 
                         ABSOLUTE,
                            r10, r10, /* rx, ry */
                            0.0,          /* angle  */
                            0,1,          /* lf, sf */
                            x10, y10 );

      path = (struct __svg_path *)__svg_new_path( cmds );
      (void)__svg_scene_add_node( scene, (struct __svg_node *)path );

      __svg_set_scene_stroke_width( 1.0 );

      line = (struct __svg_line *)__svg_new_line( x2, y2, xM, yM );
      (void)__svg_scene_add_node( scene, (struct __svg_node *)line );

      line = (struct __svg_line *)__svg_new_line( x5, y5, xM, yM );
      (void)__svg_scene_add_node( scene, (struct __svg_node *)line );

      line = (struct __svg_line *)__svg_new_line( x7, y7, xM, yM );
      (void)__svg_scene_add_node( scene, (struct __svg_node *)line );

      __svg_set_scene_stroke_width( 2.0 );

      line = (struct __svg_line *)__svg_new_line( x3, y3, xM, yM );
      (void)__svg_scene_add_node( scene, (struct __svg_node *)line );

      line = (struct __svg_line *)__svg_new_line( x6, y6, xM, yM );
      (void)__svg_scene_add_node( scene, (struct __svg_node *)line );

      line = (struct __svg_line *)__svg_new_line( x8, y8, xM, yM );
      (void)__svg_scene_add_node( scene, (struct __svg_node *)line );

      line = (struct __svg_line *)__svg_new_line( x11, y11, xM, yM );
      (void)__svg_scene_add_node( scene, (struct __svg_node *)line );

      line = (struct __svg_line *)__svg_new_line( x12, y12, xM, yM );
      (void)__svg_scene_add_node( scene, (struct __svg_node *)line );


      /* стрелки */
      arrow( x2, y2, M_PI + a0 );
      arrow( x3, y3, M_PI );
      arrow( x11, y11, M_PI + a0 );
      arrow( x12, y12, M_PI - (M_PI / 2 - a0) );
      arrow( x5, y5, a0 );
      arrow( x6, y6, a0 );
      arrow( x7, y7, -(M_PI /2 - a0) );
      arrow( x8, y8, -(M_PI /2 - a0) );

      {
         double fx1, fy1;
         fx1 = x9 - 4.0 * sin( a0 );
         fy1 = y9 + 4.0 * cos( a0 );
         small_arrow( fx1, fy1, a0 );
      }

      __svg_set_scene_text_fill_rgb( 0, 99, 99 );
      text = (struct __svg_text *)__svg_new_text( x2, y2+15,
      "<tspan font-style=\"oblique\">B</tspan>",
                                               "Verdana", 0 );
      (void)__svg_scene_add_node( scene, (struct __svg_node *)text );

      text = (struct __svg_text *)__svg_new_text( x11+7, y11+15,
      "<tspan font-style=\"oblique\">G<tspan dy=\"4\" font-family=\"Georgia\" font-size=\"10\">&#x3c4;</tspan></tspan>",
                                               "Verdana", 0 );
      (void)__svg_scene_add_node( scene, (struct __svg_node *)text );

      text = (struct __svg_text *)__svg_new_text( x12+7, y12,
      "<tspan font-style=\"oblique\">G<tspan dy=\"4\" font-family=\"Georgia\" font-size=\"10\">n</tspan></tspan>",
                                               "Verdana", 0 );
      (void)__svg_scene_add_node( scene, (struct __svg_node *)text );

      text = (struct __svg_text *)__svg_new_text( x3, y3+20,
      "<tspan font-style=\"oblique\">G</tspan>",
                                               "Verdana", 0 );
      (void)__svg_scene_add_node( scene, (struct __svg_node *)text );

      text = (struct __svg_text *)__svg_new_text( xM-20, yM,
      "<tspan font-weight=\"bold\" font-style=\"oblique\">M</tspan>",
                                               "Verdana", 0 );
      (void)__svg_scene_add_node( scene, (struct __svg_node *)text );

      text = (struct __svg_text *)__svg_new_text( x6+5, y6+12,
      "<tspan font-family=\"Georgia\" font-style=\"oblique\">&#x3c4;</tspan>",
                                               "Verdana", 0 );
      (void)__svg_scene_add_node( scene, (struct __svg_node *)text );

      text = (struct __svg_text *)__svg_new_text( x8+5, y8-5,
      "<tspan font-family=\"Georgia\" font-style=\"oblique\">n</tspan>",
                                               "Verdana", 0 );
      (void)__svg_scene_add_node( scene, (struct __svg_node *)text );

      text = (struct __svg_text *)__svg_new_text( x5+5, y5+12,
      "<tspan font-family=\"Georgia\" font-style=\"oblique\">&#x3bd;</tspan>",
                                               "Verdana", 0 );
      (void)__svg_scene_add_node( scene, (struct __svg_node *)text );

      text = (struct __svg_text *)__svg_new_text( x7+5, y7-5,
      "<tspan font-style=\"oblique\">N</tspan>",
                                               "Verdana", 0 );
      (void)__svg_scene_add_node( scene, (struct __svg_node *)text );

      text = (struct __svg_text *)__svg_new_text( x0+10, y0+r9+10,
      "<tspan font-family=\"Georgia\" font-style=\"oblique\">&#x3c6;</tspan>",
                                               "Verdana", 0 );
      (void)__svg_scene_add_node( scene, (struct __svg_node *)text );

      text = (struct __svg_text *)__svg_new_text( x0-10, y0+r0+20,
      "<tspan font-style=\"oblique\">O<tspan dy=\"4\" font-size=\"8\">1</tspan></tspan>",
                                               "Verdana", 0 );
      (void)__svg_scene_add_node( scene, (struct __svg_node *)text );

      text = (struct __svg_text *)__svg_new_text( x0-15, y0+15,
      "<tspan font-style=\"oblique\">O</tspan>",
                                               "Verdana", 0 );
      (void)__svg_scene_add_node( scene, (struct __svg_node *)text );

      text = (struct __svg_text *)__svg_new_text( x0-15, y0+r0-5,
      "<tspan font-style=\"oblique\">-</tspan>",
                                               "Verdana", 0 );
      (void)__svg_scene_add_node( scene, (struct __svg_node *)text );

      text = (struct __svg_text *)__svg_new_text( x0+7, y0+r0-5,
      "<tspan font-style=\"oblique\">+</tspan>",
                                               "Verdana", 0 );
      (void)__svg_scene_add_node( scene, (struct __svg_node *)text );

      text = (struct __svg_text *)__svg_new_text( x0+40, y0+r0-10,
      "<tspan font-style=\"oblique\">s</tspan>",
                                               "Verdana", 0 );
      (void)__svg_scene_add_node( scene, (struct __svg_node *)text );
   }

   /* Set current values for newest objects: */
   __svg_set_scene_stroke( "black" );
   __svg_set_scene_stroke_width( 1.0 );
   __svg_set_scene_stroke_dasharray( "none" );
   __svg_set_scene_fill_rgb( 255, 255, 255 );

   circle = (struct __svg_circle *)__svg_new_circle( x0, y0, 3 );
   (void)__svg_scene_add_node( scene, (struct __svg_node *)circle );

   circle = (struct __svg_circle *)__svg_new_circle( xM, yM, 3 );
   (void)__svg_scene_add_node( scene, (struct __svg_node *)circle );

   circle = (struct __svg_circle *)__svg_new_circle( x0, y0+r0, 3 );
   (void)__svg_scene_add_node( scene, (struct __svg_node *)circle );

   __svg_set_scene_fill( "none" );

   __svg_paint_scene( scene, 0, "M.svg" );

   __svg_free_scene( scene );

   return( 0 );
}

