/*============================================================================*/
/* These macros and declarations are independant from php-ext-gen             */
/* They are generally here to improve compatibility between minor PHP versions */

/*------------------*/

#ifndef S_ISDIR
#	define S_ISDIR(mode)	(((mode)&S_IFMT) == S_IFDIR)
#endif
#ifndef S_ISREG
#	define S_ISREG(mode)	(((mode)&S_IFMT) == S_IFREG)
#endif

#ifndef NULL
#	define NULL (char *)0
#endif

#ifndef TRUE
#	define TRUE 1
#endif

#ifndef FALSE
#	define FALSE 0
#endif

/*----------------*/
/* Compatibility macros - Allow using these macros with PHP 5.1 and more */

#ifndef Z_ADDREF
#define Z_ADDREF_P(_zp)		ZVAL_ADDREF(_zp)
#define Z_ADDREF(_z)		Z_ADDREF_P(&(_z))
#define Z_ADDREF_PP(ppz)	Z_ADDREF_P(*(ppz))
#endif

#ifndef Z_DELREF_P
#define Z_DELREF_P(_zp)		ZVAL_DELREF(_zp)
#define Z_DELREF(_z)		Z_DELREF_P(&(_z))
#define Z_DELREF_PP(ppz)	Z_DELREF_P(*(ppz))
#endif

#ifndef Z_REFCOUNT_P
#define Z_REFCOUNT_P(_zp)	ZVAL_REFCOUNT(_zp)
#define Z_REFCOUNT_PP(_zpp)	ZVAL_REFCOUNT(*(_zpp))
#endif

#ifndef Z_UNSET_ISREF_P
#define Z_UNSET_ISREF_P(_zp)	{ (_zp)->is_ref=0; }
#endif

#ifndef ZVAL_COPY_VALUE
#define ZVAL_COPY_VALUE(z, v) \
	do { \
		(z)->value = (v)->value; \
		Z_TYPE_P(z) = Z_TYPE_P(v); \
	} while (0)
#endif

#ifndef GC_REMOVE_ZVAL_FROM_BUFFER
#define GC_REMOVE_ZVAL_FROM_BUFFER(z)
#endif

#ifndef INIT_PZVAL_COPY
#define INIT_PZVAL_COPY(z, v) \
	do { \
		INIT_PZVAL(z); \
		ZVAL_COPY_VALUE(z, v); \
	} while (0)
#endif

#ifndef IS_CONSTANT_ARRAY
#define IS_CONSTANT_ARRAY IS_CONSTANT_AST
#endif

#ifndef IS_CONSTANT_TYPE_MASK
#define IS_CONSTANT_TYPE_MASK (~IS_CONSTANT_INDEX)
#endif

/*----------------*/

#ifndef MIN
#	define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
#	define MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif

/*---------------------------------------------------------------*/
/* (Taken from pcre/pcrelib/internal.h) */
/* To cope with SunOS4 and other systems that lack memmove() but have bcopy(),
define a macro for memmove() if HAVE_MEMMOVE is false, provided that HAVE_BCOPY
is set. Otherwise, include an emulating function for those systems that have
neither (there are some non-Unix environments where this is the case). This
assumes that all calls to memmove are moving strings upwards in store,
which is the case in this extension. */

#if ! HAVE_MEMMOVE
#	ifdef memmove
#		undef  memmove					/* some systems may have a macro */
#	endif
#	if HAVE_BCOPY
#		define memmove(a, b, c) bcopy(b, a, c)
#	else							/* HAVE_BCOPY */
		static void *my_memmove(unsigned char *dest, const unsigned char *src,
								size_t n)
		{
			int i;

			dest += n;
			src += n;
			for (i = 0; i < n; ++i)
				*(--dest) = *(--src);
		}
#		define memmove(a, b, c) my_memmove(a, b, c)
#	endif	/* not HAVE_BCOPY */
#endif		/* not HAVE_MEMMOVE */

/*------------*/

#ifdef _AIX
#	undef PHP_SHLIB_SUFFIX
#	define PHP_SHLIB_SUFFIX "a"
#endif

/*------------*/

#ifndef ZVAL_IS_ARRAY
#define ZVAL_IS_ARRAY(zp)	(Z_TYPE_P((zp))==IS_ARRAY)
#endif

#ifndef ZVAL_IS_STRING
#define ZVAL_IS_STRING(zp)	(Z_TYPE_P((zp))==IS_STRING)
#endif

#ifndef ZVAL_IS_LONG
#define ZVAL_IS_LONG(zp)	(Z_TYPE_P((zp))==IS_LONG)
#endif

#ifndef ZVAL_IS_BOOL
#define ZVAL_IS_BOOL(zp)	(Z_TYPE_P((zp))==IS_BOOL)
#endif

/*============================================================================*/
