

#include <stdlib.h>
#include <stdio.h>

#include <svgc.h>

struct __svg_sheet   *sheet;
struct __svg_rect    *rect;

struct __svg_r_gradient *r_gradient;
struct __svg_stop       *stop;

struct __svg_scene *scene;

int main( void )
{

   char *url = (char *)NULL;

   sheet = (struct __svg_sheet *)__svg_new_sheet( 800, 400, 10, 10, 10, 10 );
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

   r_gradient = 
     (struct __svg_r_gradient *)__svg_new_r_gradient( "userSpaceOnUse", NULL, 400.0, 200.0, 300.0, 400.0, 200.0 );


   stop = (struct __svg_stop *)__svg_new_stop( 0.0 );
   (void)__svg_set_stop_color_rgb( (struct __svg_node *)stop, 255, 0, 0 );
   (void)__svg_set_stop_opacity( (struct __svg_node *)stop, 1.0 );

   (void)__svg_gradient_add_stop( (void *)r_gradient, stop );

   stop = (struct __svg_stop *)__svg_new_stop( 0.5 );
   (void)__svg_set_stop_color_rgb( (struct __svg_node *)stop, 0, 0, 255 );
   (void)__svg_set_stop_opacity( (struct __svg_node *)stop, 1.0 );

   (void)__svg_gradient_add_stop( (void *)r_gradient, stop );

   stop = (struct __svg_stop *)__svg_new_stop( 1.0 );
   (void)__svg_set_stop_color_rgb( (struct __svg_node *)stop, 255, 0, 0 );
   (void)__svg_set_stop_opacity( (struct __svg_node *)stop, 1.0 );

   (void)__svg_gradient_add_stop( (void *)r_gradient, stop );

   (void)__svg_sheet_add_definition( (void *)sheet,
                                     (struct __svg_node *)r_gradient );

   url = __svg_create_node_url( (struct __svg_node *)r_gradient );

   rect = (struct __svg_rect *)__svg_new_rect( 100.0, 100.0, 600.0, 200.0, 0.0, 0.0 );

   if( url )
   {
      (void)__svg_set_fill( (struct __svg_node *)rect, url );
      free( url );
   }
   else      (void)__svg_set_fill( (struct __svg_node *)rect, "rgb(255,0,0)" );

   (void)__svg_set_stroke_rgb( (struct __svg_node *)rect, 19, 44, 92 );
   (void)__svg_set_stroke_width( (struct __svg_node *)rect, 0.1 );
   (void)__svg_scene_add_node( scene, (struct __svg_node *)rect );


   __svg_paint_scene( scene, 0, "M.svg" );

   __svg_free_scene( scene );

   return( 0 );
}
