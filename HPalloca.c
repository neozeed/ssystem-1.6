#ifdef emacs
#include "config.h"
#ifdef static
/* actually, only want this if static is defined as ""
   -- this is for usg, in which emacs must undefine static
   in order to make unexec workable
   */
#ifndef STACK_DIRECTION
you
lose
-- must know STACK_DIRECTION at compile-time
#endif /* STACK_DIRECTION undefined */
#endif 
#else
#define xmalloc malloc
#endif 

#if defined(X3J11) || defined(__STDC__)
typedef void    *pointer;               /* generic pointer type */
#else
typedef char    *pointer;               /* generic pointer type */
#endif

#define NULL    0                       /* null pointer constant */

extern void     free();
extern pointer  xmalloc();

/*
        Define STACK_DIRECTION if you know the direction of stack
        growth for your system; otherwise it will be automatically
        deduced at run-time.

        STACK_DIRECTION > 0 => grows toward higher addresses
        STACK_DIRECTION < 0 => grows toward lower addresses
        STACK_DIRECTION = 0 => direction of growth unknown
*/

#ifndef STACK_DIRECTION
#define STACK_DIRECTION 0               /* direction unknown */
#endif
#if STACK_DIRECTION != 0

#define STACK_DIR       STACK_DIRECTION /* known at compile-time */

#else   /* STACK_DIRECTION == 0; need run-time code */

static int      stack_dir;              /* 1 or -1 once known */
#define STACK_DIR       stack_dir

static void
find_stack_direction (/* void */)
{
  static char   *addr = NULL;   /* address of first
                                   `dummy', once known */
  auto char     dummy;          /* to get stack address */

  if (addr == NULL)
    {                           /* initial entry */
      addr = &dummy;

      find_stack_direction ();  /* recurse once */
    }
  else                          /* second entry */
    if (&dummy > addr)
      stack_dir = 1;            /* stack grew upward */
    else
      stack_dir = -1;           /* stack grew downward */
}

#endif  /* STACK_DIRECTION == 0 */



/*
        An "alloca header" is used to:
        (a) chain together all alloca()ed blocks;
        (b) keep track of stack depth.

        It is very important that sizeof(header) agree with malloc()
        alignment chunk size.  The following default should work okay.
*/

#ifndef ALIGN_SIZE
#define ALIGN_SIZE      sizeof(double)
#endif

typedef union hdr
{
  char  align[ALIGN_SIZE];      /* to force sizeof(header) */
  struct
    {
      union hdr *next;          /* for chaining headers */
      char *deep;               /* for stack depth measure */
    } h;
} header;

/*
        alloca( size ) returns a pointer to at least `size' bytes of
        storage which will be automatically reclaimed upon exit from
        the procedure that called alloca().  Originally, this space
        was supposed to be taken from the current stack frame of the
        caller, but that method cannot be made to work for some
        implementations of C, for example under Gould's UTX/32.
*/

static header *last_alloca_header = NULL; /* -> last alloca header */

pointer
alloca (size)                   /* returns pointer to storage */
     unsigned   size;           /* # bytes to allocate */
{
  auto char     probe;          /* probes stack depth: */
  register char *depth = &probe;

#if STACK_DIRECTION == 0
  if (STACK_DIR == 0)           /* unknown growth direction */
    find_stack_direction ();
#endif

                                /* Reclaim garbage, defined as all alloca()ed st
orage that
                                   was allocated from deeper in the stack than c
urrently. */

  {
    register header     *hp;    /* traverses linked list */

    for (hp = last_alloca_header; hp != NULL;)
      if (STACK_DIR > 0 && hp->h.deep > depth
          || STACK_DIR < 0 && hp->h.deep < depth)
        {
          register header       *np = hp->h.next;

          free ((pointer) hp);  /* collect garbage */

          hp = np;              /* -> next header */
        }
      else
        break;                  /* rest are not deeper */

    last_alloca_header = hp;    /* -> last valid storage */
  }

  if (size == 0)
    return NULL;                /* no allocation required */

  /* Allocate combined header + user data storage. */
  {
    register pointer    new = xmalloc (sizeof (header) + size);
    /* address of header */

    ((header *)new)->h.next = last_alloca_header;
    ((header *)new)->h.deep = depth;

    last_alloca_header = (header *)new;

    /* User storage begins just after header. */

    return (pointer)((char *)new + sizeof(header));
  }
}
