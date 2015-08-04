#ifndef _H_TYPE_H_
#define	_H_TYPE_H_

#ifndef STATUS
#define STATUS int
#endif

#ifndef BOOL
typedef int     BOOL;
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

typedef int   ssize;

#ifndef OK
#define OK 0
#endif

#ifndef ERROR_E
#define ERROR_E (-1)
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


#define  usr_log(fmt) 

#define	LOCAL	static
#define IMPORT  extern



/* common error number definition */
enum
{
	E_INDEX	= 1,		/* invlid index */
	E_NAME,			/* invalid name */
	E_PARAM,			/* invalid parameter(s) */
	E_NORSRC,			/* no resource */
	E_REG,				/* register failed */
	E_AGAIN,			/* do more than once */
	E_NOTREADY,		/* not ready */
	E_EXIST,			/* already exist */
	E_NOSUCH,			/* no such instance */
	E_CORRUPT,			/* corrupted */
	E_TOOMUCH,			/* too much instances */
	E_NOTSUPPORT,		/* feature not support */
	E_FAIL,				/* general failure */
	E_NODATA,			/* no get data */			
	E_UNKOWN,			/* unkown failure */
	
	/* E_COMMON is not assignable number, it is the start
	 * of module specific error number.
	 */
	E_COMMON,
};


#define ALIGN(size, align)           (((size) + (align) - 1) & ~((align) - 1))
#define ALIGN_DOWN(size, align)      ((size) & ~((align) - 1))

#define TO_TYPE(point,type1)   ((type1 *)(point))
#define  TYPE_TO_VALUE(point,type1) (*((type1 *)(point)))


#endif
