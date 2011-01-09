

#include <stdlib.h>
#include <stdio.h>

#include <svgc.h>

struct __svg_sheet   *sheet;
struct __svg_line    *line;
struct __svg_rect    *rect;
struct __svg_circle  *circle;

struct __svg_l_gradient *l_gradient;
struct __svg_stop       *stop;

struct __svg_scene *scene;

int main( void )
{

   sheet = (struct __svg_sheet *)__svg_new_sheet( 200, 200, 10, 10, 10, 10 );
   if( !sheet )
   {
      printf( "Unable to create sheet\n" );
   }
   (void)__svg_set_stroke_width( (struct __svg_node *)sheet, 1.0 );

   scene = __svg_new_scene( sheet );
   if( !scene )
   {
      printf( "Unable to create scene\n" );
   }

   l_gradient = 
     (struct __svg_l_gradient *)__svg_new_l_gradient( NULL, 0.0, 0.0, 0.0, 1.0 );

   stop = (struct __svg_stop *)__svg_new_stop( 0.0 );
   (void)__svg_set_stop_color_rgb( (struct __svg_node *)stop, 0, 247, 248 );
   (void)__svg_set_stop_opacity( (struct __svg_node *)stop, 1.0 );

   (void)__svg_gradient_add_stop( (void *)l_gradient, stop );

   stop = (struct __svg_stop *)__svg_new_stop( 0.5 );
   (void)__svg_set_stop_color_rgb( (struct __svg_node *)stop, 19, 44, 92 );
   (void)__svg_set_stop_opacity( (struct __svg_node *)stop, 1.0 );

   (void)__svg_gradient_add_stop( (void *)l_gradient, stop );

   stop = (struct __svg_stop *)__svg_new_stop( 1.0 );
   (void)__svg_set_stop_color_rgb( (struct __svg_node *)stop, 0, 3, 42 );
   (void)__svg_set_stop_opacity( (struct __svg_node *)stop, 1.0 );

   (void)__svg_gradient_add_stop( (void *)l_gradient, stop );

   (void)__svg_sheet_add_definition( (void *)sheet,
                                     (struct __svg_node *)l_gradient );

   {
      char *url = __svg_create_node_url( (struct __svg_node *)l_gradient );

      rect = (struct __svg_rect *)__svg_new_rect( 50.0, 50.0, 50.0, 50.0, 5.0, 5.0 );

      if( url )
      {
         (void)__svg_set_fill( (struct __svg_node *)rect, url );
         free( url );
      }
      else      (void)__svg_set_fill( (struct __svg_node *)rect, "rgb(255,0,0)" );

      (void)__svg_set_stroke_rgb( (struct __svg_node *)rect, 19, 44, 92 );
      (void)__svg_set_stroke_width( (struct __svg_node *)rect, 0.1 );
      (void)__svg_scene_add_node( scene, (struct __svg_node *)rect );
   }

   /* Set current values for newest objects: */
   __svg_set_scene_stroke_linecap( LINECAP_SQUARE );
   __svg_set_scene_fill( "none" );
   __svg_set_scene_stroke_width( 2.0 );
   __svg_set_scene_stroke_rgb( 0, 138, 147 );
   __svg_set_scene_stroke_dasharray( "15,7,3,7" );

   line = (struct __svg_line *)__svg_new_line( 50.0, 150.0, 150.0, 50.0 );
   (void)__svg_scene_add_node( scene, (struct __svg_node *)line );


   /* Set current values for newest objects: */
   __svg_set_scene_stroke( "black" );
   __svg_set_scene_stroke_width( 1.0 );
   __svg_set_scene_stroke_dasharray( "none" );
   __svg_set_scene_fill_rgb( 255, 0, 0 );

   circle = (struct __svg_circle *)__svg_new_circle( 125.0, 125.0, 35.0 );
   (void)__svg_scene_add_node( scene, (struct __svg_node *)circle );


   __svg_paint_scene( scene, 0, "M.svg" );

   __svg_free_scene( scene );

   return( 0 );
}
