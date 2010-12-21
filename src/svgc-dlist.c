
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

#include <svgc-dlist.h>

struct svgc_dlist *create_svgc_dlist()
{
   struct svgc_dlist  *ret = (struct svgc_dlist *)0;

   ret = (struct svgc_dlist *)malloc( sizeof( struct svgc_dlist ) );
   if( !ret )
   {
      return( (struct svgc_dlist *)0 );
   }

   ret->root_up   = (struct svgc_dlist_node *)0;
   ret->root_down = (struct svgc_dlist_node *)0;
   ret->ptr       = (struct svgc_dlist_node *)0;

   return( ret );

}


void free_svgc_dlist( plist, free_data )
   struct svgc_dlist          *plist;
   FREE_SVGC_DLIST_NODE_DATA   free_data;
{
   struct svgc_dlist_node  *p;

   if( plist != (struct svgc_dlist *)0 )
   {
      if( plist->root_up   == (struct svgc_dlist_node *)0 &&
          plist->root_down == (struct svgc_dlist_node *)0    )
      {
         plist->ptr = (struct svgc_dlist_node *)0;
         return;
      }

      for( p = plist->root_up; p != (struct svgc_dlist_node *)0; p = p->down )
      {
         if( p->up )
         {
            if( p->up->id ) free( p->up->id );
            if( free_data ) free_data( p->up->data );
            free( p->up );
         }
      }
      if( plist->root_down->id ) free( plist->root_down->id );
      if( free_data )            free_data( plist->root_down->data );
      free( plist->root_down );

      plist->root_up   = (struct svgc_dlist_node *)0;
      plist->root_down = (struct svgc_dlist_node *)0;
      plist->ptr       = (struct svgc_dlist_node *)0;

      free( plist );
   }
}

/* add svgc_dlist_node up */
struct svgc_dlist_node *add_to_svgc_dlist_up( plist, id, data )
   struct svgc_dlist  *plist;
   char               *id;
   void               *data;
{
   struct svgc_dlist_node  *ret = (struct svgc_dlist_node *)0;
   char                    *s   = (char *)0;

   if( !plist ) return( (struct svgc_dlist_node *)0 );

   ret = (struct svgc_dlist_node *)malloc( sizeof( struct svgc_dlist_node ) );
   if( !ret )
   {
      return( (struct svgc_dlist_node *)0 );
   }

   ret->id   = (char *)0;
   ret->data = (void *)0;

   if( id && id[0] )
   {
      s = (char *)malloc( strlen( id ) + 1 );
      if( !s )
      {
         free( ret );
         return( (struct svgc_dlist_node *)0 );
      }
      strcpy( s, id );
      ret->id = s;
   }

   if( data ) ret->data = data;

   /* add to list */
   if( plist->root_up   == (struct svgc_dlist_node *)0 &&
       plist->root_down == (struct svgc_dlist_node *)0   )
   {
      plist->root_up = plist->root_down = plist->ptr = ret;
      ret->up        = (struct svgc_dlist_node *)0;
      ret->down      = (struct svgc_dlist_node *)0;
   }
   else
   {
      plist->root_up->up = ret;
      ret->up            = (struct svgc_dlist_node *)0;
      ret->down          = plist->root_up;
      plist->root_up     = ret;
   }

   return( ret );
}

/* add svgc_dlist_node down */
struct svgc_dlist_node *add_to_svgc_dlist_down( plist, id, data )
   struct svgc_dlist  *plist;
   char               *id;
   void               *data;
{
   struct svgc_dlist_node  *ret = (struct svgc_dlist_node *)0;
   char                    *s   = (char *)0;

   if( !plist ) return( (struct svgc_dlist_node *)0 );

   ret = (struct svgc_dlist_node *)malloc( sizeof( struct svgc_dlist_node ) );
   if( !ret )
   {
      return( (struct svgc_dlist_node *)0 );
   }

   ret->id   = (char *)0;
   ret->data = (void *)0;

   if( id && id[0] )
   {
      s = (char *)malloc( strlen( id ) + 1 );
      if( !s )
      {
         free( ret );
         return( (struct svgc_dlist_node *)0 );
      }
      strcpy( s, id );
      ret->id = s;
   }

   if( data ) ret->data = data;

   /* add to list */
   if( plist->root_up   == (struct svgc_dlist_node *)0 &&
       plist->root_down == (struct svgc_dlist_node *)0   )
   {
      plist->root_up = plist->root_down = plist->ptr = ret;
      ret->up        = (struct svgc_dlist_node *)0;
      ret->down      = (struct svgc_dlist_node *)0;
   }
   else
   {
      plist->root_down->down = ret;
      ret->up                = plist->root_down;
      ret->down              = (struct svgc_dlist_node *)0;
      plist->root_down       = ret;
      plist->ptr             = ret;
   }

   return( ret );
}


struct svgc_dlist_node *insert_into_svgc_dlist_up( plist, p )
   struct svgc_dlist       *plist;
   struct svgc_dlist_node  *p;
{
   if( plist && p )
   {
      if( plist->root_up   == (struct svgc_dlist_node *)0 &&
          plist->root_down == (struct svgc_dlist_node *)0   )
      {
         plist->root_up = plist->root_down = plist->ptr = p;
         p->up          = (struct svgc_dlist_node *)0;
         p->down        = (struct svgc_dlist_node *)0;
      }
      else
      {
         plist->root_up->up = p;
         p->up              = (struct svgc_dlist_node *)0;
         p->down            = plist->root_up;
         plist->root_up     = p;
      }
      return( p );
   }
   else
   {
      return( (struct svgc_dlist_node *)0 );
   }
}

struct svgc_dlist_node *insert_into_svgc_dlist_down( plist, p )
   struct svgc_dlist       *plist;
   struct svgc_dlist_node  *p;
{
   if( plist && p )
   {
      if( plist->root_up   == (struct svgc_dlist_node *)0 &&
          plist->root_down == (struct svgc_dlist_node *)0   )
      {
         plist->root_up = plist->root_down = plist->ptr = p;
         p->up          = (struct svgc_dlist_node *)0;
         p->down        = (struct svgc_dlist_node *)0;
      }
      else
      {
         plist->root_down->down = p;
         p->up                  = plist->root_down;
         p->down                = (struct svgc_dlist_node *)0;
         plist->root_down       = p;
         plist->ptr             = p;
      }
      return( p );
   }
   else
   {
      return( (struct svgc_dlist_node *)0 );
   }
}

struct svgc_dlist_node *insert_into_svgc_dlist_after( plist, point, p )
   struct svgc_dlist       *plist;
   struct svgc_dlist_node  *point;
   struct svgc_dlist_node  *p;
{
   if( plist && point && p )
   {
      if( plist->root_up   == (struct svgc_dlist_node *)0 &&
          plist->root_down == (struct svgc_dlist_node *)0   )
      {
         plist->root_up = plist->root_down = plist->ptr = p;
         p->up          = (struct svgc_dlist_node *)0;
         p->down        = (struct svgc_dlist_node *)0;
      }
      else
      {
         point->down->up = p;
         p->down         = point->down;
         point->down     = p;
         p->up           = point;
      }
      return( p );
   }
   else
   {
      return( (struct svgc_dlist_node *)0 );
   }
}


/*
  lookup svgc_dlist_node, since up for down ( down ).
 */
struct svgc_dlist_node *lookup_svgc_dlist_node_down( plist, id )
   struct svgc_dlist  *plist;
   char               *id;
{
   struct svgc_dlist_node *p;

   if( !plist && !id && !id[0] ) return( (struct svgc_dlist_node *)0 );

   if( plist != (struct svgc_dlist *)0 )
   {
      for( p = plist->root_up; p != (struct svgc_dlist_node *)0; p = p->down )
      {
         if( strcmp( p->id, id ) == 0 ) return( p );
      }
   }
   return( (struct svgc_dlist_node *)0 );
}

/*
  lookup svgc_dlist_node, since down for up ( down ).
 */
struct svgc_dlist_node *lookup_svgc_dlist_node_up( plist, id )
   struct svgc_dlist  *plist;
   char               *id;
{
   struct svgc_dlist_node *p;

   if( !plist && !id && !id[0] ) return( (struct svgc_dlist_node *)0 );

   if( plist != (struct svgc_dlist *)0 )
   {
      for( p = plist->root_down; p != (struct svgc_dlist_node *)0; p = p->up )
      {
         if( strcmp( p->id, id ) == 0 ) return( p );
      }
   }
   return( (struct svgc_dlist_node *)0 );
}



/* detach svgc_dlist_node */
struct svgc_dlist_node *detach_svgc_dlist_node( plist, p )
   struct svgc_dlist       *plist;
   struct svgc_dlist_node  *p;
{
   if( plist && p )
   {
      if( plist->root_up   == (struct svgc_dlist_node *)0 &&
          plist->root_down == (struct svgc_dlist_node *)0   )
      {
         /* EMPTY svgc_dlist */
         return( (struct svgc_dlist_node *)0 );
      }
      else
      {
         if( p->up )   p->up->down = p->down;
         if( p->down ) p->down->up = p->up;


         if( p == plist->root_up )   plist->root_up   = p->down;
         if( p == plist->root_down ) plist->root_down = p->up;
         if( p == plist->ptr )       plist->ptr       = p->down;

         return( p );
      }
   }
   else
   {
      return( (struct svgc_dlist_node *)0 );
   }
}

/* detach svgc_dlist_node by id */
struct svgc_dlist_node *detach_svgc_dlist_node_by_id( plist, id )
   struct svgc_dlist  *plist;
   char               *id;
{
   struct svgc_dlist_node  *p = (struct svgc_dlist_node *)0;

   p = lookup_svgc_dlist_node_down( plist, id );

   if( plist && p )
   {
      if( plist->root_up   == (struct svgc_dlist_node *)0 &&
          plist->root_down == (struct svgc_dlist_node *)0   )
      {
         /* EMPTY svgc_dlist */
         return( (struct svgc_dlist_node *)0 );
      }
      else
      {
         if( p->up )   p->up->down = p->down;
         if( p->down ) p->down->up = p->up;


         if( p == plist->root_up )   plist->root_up   = p->down;
         if( p == plist->root_down ) plist->root_down = p->up;
         if( p == plist->ptr )       plist->ptr       = p->down;

         return( p );
      }
   }
   else
   {
      return( (struct svgc_dlist_node *)0 );
   }
}

/* remove svgc_dlist_node */
int remove_svgc_dlist_node( plist, p, free_data )
   struct svgc_dlist          *plist;
   struct svgc_dlist_node     *p;
   FREE_SVGC_DLIST_NODE_DATA   free_data;
{
   if( plist && p )
   {
      if( plist->root_up   == (struct svgc_dlist_node *)0 &&
          plist->root_down == (struct svgc_dlist_node *)0   )
      {
         /* EMPTY svgc_dlist */
         return( 0 ); /* FALSE */
      }
      else
      {
         if( p->up )   p->up->down = p->down;
         if( p->down ) p->down->up = p->up;


         if( p == plist->root_up )   plist->root_up   = p->down;
         if( p == plist->root_down ) plist->root_down = p->up;
         if( p == plist->ptr )       plist->ptr       = p->down;

         if( p->id ) free( p->id );
         if( free_data ) free_data( p->data );
         free( p );

         return( 1 ); /* TRUE */
      }
   }
   else
   {
      return( 0 ); /* FALSE */
   }
}

/* remove svgc_dlist_node by id */
int remove_svgc_dlist_node_by_id( plist, id, free_data )
   struct svgc_dlist          *plist;
   char                       *id;
   FREE_SVGC_DLIST_NODE_DATA   free_data;
{
   struct svgc_dlist_node  *p = (struct svgc_dlist_node *)0;

   p = lookup_svgc_dlist_node_down( plist, id );

   if( plist && p )
   {
      if( plist->root_up   == (struct svgc_dlist_node *)0 &&
          plist->root_down == (struct svgc_dlist_node *)0   )
      {
         /* EMPTY svgc_dlist */
         return( 0 ); /* FALSE */
      }
      else
      {
         if( p->up )   p->up->down = p->down;
         if( p->down ) p->down->up = p->up;


         if( p == plist->root_up )   plist->root_up   = p->down;
         if( p == plist->root_down ) plist->root_down = p->up;
         if( p == plist->ptr )       plist->ptr       = p->down;

         if( p->id ) free( p->id );
         if( free_data ) free_data( p->data );
         free( p );

         return( 1 ); /* TRUE */
      }
   }
   else
   {
      return( 0 ); /* FALSE */
   }
}


/* move svgc_dlist_node: detach from SLIST, and insert_into_svgc_dlist_down() to TLIST */
struct svgc_dlist_node *move_svgc_dlist_node( tlist, slist, p )
   struct svgc_dlist      *tlist;
   struct svgc_dlist      *slist;
   struct svgc_dlist_node *p;
{
   if( !slist || !p ) return( (struct svgc_dlist_node *)0 );

   if( slist->root_up   == (struct svgc_dlist_node *)0 &&
       slist->root_down == (struct svgc_dlist_node *)0   )
   {
      /* EMPTY partition_list */
      return( (struct svgc_dlist_node *)0 );
   }
   else
   {
      if( p->up )   p->up->down = p->down;
      if( p->down ) p->down->up = p->up;


      if( p == slist->root_up )   slist->root_up     = p->down;
      if( p == slist->root_down ) slist->root_down   = p->up;
      if( p == slist->ptr )       slist->ptr = p->down;

      insert_into_svgc_dlist_down( tlist, p );

      return( p );
   }
}



void set_svgc_dlist_ptr_up( plist )
   struct svgc_dlist  *plist;
{
   if( !plist ) return;
   plist->ptr = plist->root_up;
}
void set_svgc_dlist_ptr_down( plist )
   struct svgc_dlist  *plist;
{
   if( !plist ) return;
   plist->ptr = plist->root_down;
}

void for_each_svgc_dlist_node_data( plist, func )
   struct svgc_dlist              *plist;
   FOR_EACH_SVGC_DLIST_NODE_DATA   func;
{
   struct svgc_dlist_node  *p;

   if( plist != (struct svgc_dlist *)0 )
   {
      if( plist->root_up   == (struct svgc_dlist_node *)0 &&
          plist->root_down == (struct svgc_dlist_node *)0    )
      {
         plist->ptr = (struct svgc_dlist_node *)0;
         return;
      }

      for( p = plist->root_up; p != (struct svgc_dlist_node *)0; p = p->down )
      {
         if( p->up )
         {
            if( func ) func( p->up->data );
         }
      }
      if( func ) func( plist->root_down->data );
   }
}

void printf_svgc_dlist( plist, func )
   struct svgc_dlist           *plist;
   PRINT_SVGC_DLIST_NODE_DATA   func;
{
   struct svgc_dlist_node  *p;

   fprintf( stderr, "\nD-LIST:\n"
                      "======\n\n" );

   if( plist != (struct svgc_dlist *)0 )
   {
      if( plist->root_up   == (struct svgc_dlist_node *)0 &&
          plist->root_down == (struct svgc_dlist_node *)0    )
      {
         plist->ptr = (struct svgc_dlist_node *)0;
         return;
      }

      for( p = plist->root_up; p != (struct svgc_dlist_node *)0; p = p->down )
      {
         if( p->up )
         {
            if( p->up->id )
               fprintf( stderr, " id = '%s':\n", p->up->id );
            else
               fprintf( stderr, " id = '':\n" );
            if( func ) func( p->up->data );
         }
      }
      if( plist->root_down->id )
         fprintf( stderr, " id = '%s':\n", plist->root_down->id );
      else
         fprintf( stderr, " id = '':\n" );
      if( func ) func( plist->root_down->data );
   }
}
