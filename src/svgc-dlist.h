
/***************************************************************
  SVGC-DLIST.H

    This file containt declarations of SVG double linked
    list functions.
 ***************************************************************/
/*
  This file contant RUSSIAN letters( code-page: iso-8859-5 )
 ***************************************************************/

#ifndef __SVGC_DLIST_H__
#define __SVGC_DLIST_H__

#if defined( __cplusplus )
extern "C"
{
#endif

/* the structure of a double linked list entry */
typedef struct svgc_dlist_node   svgc_dlist_node;
struct svgc_dlist_node
{
   struct svgc_dlist_node  *up;
   struct svgc_dlist_node  *down;

   char     *id;
   void     *data;
};

/* the structure of a double linked list instance */
typedef struct svgc_dlist   svgc_dlist;
struct svgc_dlist
{
   struct svgc_dlist_node *root_up;
   struct svgc_dlist_node *root_down;
   struct svgc_dlist_node *ptr;
};

typedef void  ( *FREE_SVGC_DLIST_NODE_DATA     )( void * );
typedef void  ( *FOR_EACH_SVGC_DLIST_NODE_DATA )( void * );
typedef void  ( *PRINT_SVGC_DLIST_NODE_DATA    )( void * );


extern struct svgc_dlist *create_svgc_dlist( void );
extern void free_svgc_dlist( struct svgc_dlist *plist, FREE_SVGC_DLIST_NODE_DATA free_data );

/* add svgc_dlist_node up */
extern struct svgc_dlist_node *add_to_svgc_dlist_up( struct svgc_dlist *plist, char *id, void *data );
/* add svgc_dlist_node down */
extern struct svgc_dlist_node *add_to_svgc_dlist_down( struct svgc_dlist *plist, char *id, void *data );

extern struct svgc_dlist_node *insert_into_svgc_dlist_up( struct svgc_dlist *plist, struct svgc_dlist_node *p );

extern struct svgc_dlist_node *insert_into_svgc_dlist_down( struct svgc_dlist *plist, struct svgc_dlist_node *p );

extern struct svgc_dlist_node *insert_into_svgc_dlist_after( struct svgc_dlist *plist, struct svgc_dlist_node *point, struct svgc_dlist_node *p );

/*
  lookup svgc_dlist_node, since up for down ( down ).
 */
extern struct svgc_dlist_node *lookup_svgc_dlist_node_down( struct svgc_dlist *plist, char *id );

/*
  lookup svgc_dlist_node, since down for up ( down ).
 */
extern struct svgc_dlist_node *lookup_svgc_dlist_node_up( struct svgc_dlist *plist, char *id );

/* detach svgc_dlist_node */
extern struct svgc_dlist_node *detach_svgc_dlist_node( struct svgc_dlist *plist, struct svgc_dlist_node *p );
/* detach svgc_dlist_node by id */
extern struct svgc_dlist_node *detach_svgc_dlist_node_by_id( struct svgc_dlist *plist, char *id );

/* remove svgc_dlist_node */
extern int remove_svgc_dlist_node( struct svgc_dlist *plist, struct svgc_dlist_node *p, FREE_SVGC_DLIST_NODE_DATA free_data );
/* remove svgc_dlist_node by id */
int remove_svgc_dlist_node_by_id( struct svgc_dlist *plist, char *id, FREE_SVGC_DLIST_NODE_DATA free_data );

/* move svgc_dlist_node: detach from SLIST, and insert_into_svgc_dlist_down() to TLIST */
extern struct svgc_dlist_node *move_svgc_dlist_node( struct svgc_dlist *tlist, struct svgc_dlist *slist, struct svgc_dlist_node *p );


extern void set_svgc_dlist_ptr_up( struct svgc_dlist *plist );
extern void set_svgc_dlist_ptr_down( struct svgc_dlist *plist );

extern void for_each_svgc_dlist_node_data( struct svgc_dlist *plist, FOR_EACH_SVGC_DLIST_NODE_DATA func );
extern void printf_svgc_dlist( struct svgc_dlist *plist, PRINT_SVGC_DLIST_NODE_DATA func );

#if defined( __cplusplus )
} /* extern "C" */
#endif

#endif /* __SVGC_DLIST_H__ */
