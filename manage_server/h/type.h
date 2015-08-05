#ifndef _H_TYPE_H_
#define	_H_TYPE_H_

#ifndef STATUS
#define STATUS int
#endif

#ifndef BOOL
typedef int     BOOL;
#endif



/*
 * Parameters Marking
 */
#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef INOUT
#define INOUT
#endif


#ifndef UINT8
#define UINT8 unsigned char
#endif

#ifndef UINT16
#define UINT16 unsigned short int
#endif

#ifndef UINT32
#define UINT32 unsigned  int
#endif

#ifndef ULONG
#define ULONG unsigned  long int
#endif



#ifndef INT32
#define INT32   int
#endif


typedef int                SSIZE;


typedef int   ssize;

#ifndef OK
#define OK 0
#endif

#ifndef ERROR
#define ERROR (-1)
#endif


#ifndef	 FALSE
#define  	 FALSE     (0)
#endif

#ifndef 	TRUE
#define  	TRUE     (!FALSE)
#endif


#define	LOCAL	static
#define IMPORT  extern



#define ALIGN(size, align)           (((size) + (align) - 1) & ~((align) - 1))
#define ALIGN_DOWN(size, align)      ((size) & ~((align) - 1))

#define SWAP32(x) htonl(x)
#define SWAP16(x) htons(x)

#define swap32(x)	\
	((((x) & 0xff000000U) >>24) | \
	  (((x) & 0x00ff0000U) >> 8) | \
	  (((x) & 0x0000ff00U) << 8) | \
	  (((x) & 0x000000ffU) <<24))

#define swap16(x)	\
	((((x) & 0xff00) >>8) | \
	  (((x) & 0x00ff) << 8) )

#endif
