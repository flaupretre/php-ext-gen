/*===============================================================*/
/*               EG definitions for PHP 5                        */
/*===============================================================*/

/*---------------------------------------------------------------*/
/*--- Native types */

typedef zend_bool eg_bool;
typedef long eg_int;
typedef double eg_double;
typedef char *eg_str_val;
typedef int eg_str_len;
typedef HashTable *eg_array;

typedef zend_uchar	eg_type;

/*---------------------------------------------------------------*/
/*--- Variable types */

#define EG_IS_NULL		(eg_type)IS_NULL
#define EG_IS_BOOL		(eg_type)IS_BOOL
#define EG_IS_INT		(eg_type)IS_LONG
#define EG_IS_DOUBLE	(eg_type)IS_DOUBLE
#define EG_IS_STRING	(eg_type)IS_STRING
#define EG_IS_ARRAY		(eg_type)IS_ARRAY

#define EG_TRUE		(eg_bool)1
#define EG_FALSE	(eg_bool)0

/*---------------------------------------------------------------*/
/*--- Multi-type pseudo-structure - applies to arguments and retval */

#define _EG_FUNC_TYPE_VARS \
	eg_type type; \
	eg_bool	bval; \
	eg_int ival; \
	eg_double dval; \
	eg_str_val sval; \
	eg_str_len slen; \
	eg_array aval;

/* _EG_FUNC_TYPE_xx */
/* _tp is a pointer to the structure containing the _EG_FUNC_TYPE_VARS */

#define _EG_FUNC_TYPE_SET_TYPE(_tp,_type) { (_tp)->type=_type; }

#define _EG_FUNC_TYPE_INIT(_tp) \
	_EG_FUNC_TYPE_SET_TYPE(_tp,EG_IS_NULL); \
	(_tp)->bval=FALSE; \
	(_tp)->ival=(eg_int)0; \
	(_tp)->dval=(eg_double)0; \
	(_tp)->sval=NULL; \
	(_tp)->slen=(eg_str_len)0; \
	(_tp)->aval=NULL;

#define eg_array_efree(arr)	(void)zend_hash_destroy(arr)
#define EG_ARRAY_EFREE(arr) { eg_array_efree(arr); arr=(eg_array)0; }

#define _EG_FUNC_TYPE_RESET(_tp) \
	{ \
	switch (_tp->type) \
		{ \
		case EG_IS_STRING: \
			_tp->sval=eg_eallocate(_tp->sval,0); \
			_tp->slen=0; \
			break; \
		case EG_IS_ARRAY: \
			eg_array_efree(_tp->aval); \
			_tp->aval=NULL; \
			break; \
		} \
	_EG_FUNC_TYPE_SET_TYPE(_tp,EG_IS_NULL) \
	}

#define _EG_FUNC_TYPE_NULL(_tp)

#define   _EG_FUNC_TYPE_BOOL(_tp,val) { _EG_FUNC_TYPE_SET_TYPE(_tp,EG_IS_BOOL); _tp->bval=(eg_bool)(val); }

#define _EG_FUNC_TYPE_FALSE(_tp) _EG_FUNC_TYPE_BOOL(_tp,EG_FALSE)
#define  _EG_FUNC_TYPE_TRUE(_tp) _EG_FUNC_TYPE_BOOL(_tp,EG_TRUE)

#define    _EG_FUNC_TYPE_INT(_tp,val) { _EG_FUNC_TYPE_SET_TYPE(_tp,EG_IS_INT); _tp->ival=(eg_int)(val); }
#define _EG_FUNC_TYPE_DOUBLE(_tp,val) { _EG_FUNC_TYPE_SET_TYPE(_tp,EG_IS_DOUBLE); _tp->dval=(eg_double)(val); }

/* (len) must be referenced only once (can be strlen()) */ \
#define _EG_FUNC_TYPE_STRINGL(_tp, str, len, dup) \
	{ \
	_tp->slen = (eg_str_len)len; \
	_tp->sval = (eg_str_val)((dup) ? eg_eduplicate(str,_tp->slen) : (str)); \
	_EG_FUNC_TYPE_SET_TYPE(_tp,EG_IS_STRING); \
	}

#define _EG_FUNC_TYPE_STRING(_tp,str,dup) _EG_FUNC_TYPE_STRINGL(_tp,str,strlen(str),dup)

#define _EG_FUNC_TYPE_ARRAY(_tp,val,dup) \
	{ \
	_tp->aval=((dup) ? _eg_zval_array_duplicate(val) : (val)); \
	_EG_FUNC_TYPE_SET_TYPE(_tp,EG_IS_ARRAY); \
	}

#define _EG_FUNC_TYPE_TO_ZVAL(zp,ip) { \
	switch((ip)->type) \
		{ \
		case EG_IS_NULL: \
			break; \
		case EG_IS_BOOL: \
			EG_ZVAL_BOOL(zp, (ip)->bval); \
			break; \
		case EG_IS_INT: \
			EG_ZVAL_INT(zp, (ip)->ival); \
			break; \
		case EG_IS_DOUBLE: \
			EG_ZVAL_DOUBLE(zp, (ip)->dval); \
			break; \
		case EG_IS_STRING: \
			EG_ZVAL_STRINGL(zp, (ip)->sval,(ip)->slen,0); \
			break; \
		case EG_IS_ARRAY: \
			EG_ZVAL_ARRAY(zp, (ip)->aval); \
			break; \
		} }

/* EG_FUNC_RETVAL_xx */

typedef struct 
	{
	int isset;
	_EG_FUNC_TYPE_VARS
	} _EG_FUNC_RETVAL;

#define _EG_FUNC_RETVAL_INIT(_rp) { (_rp)->isset=0; _EG_FUNC_TYPE_INIT(_rp); }

#define EG_FUNC_RETVAL_NULL()					{ if (!(_eg_retval->isset)) { _EG_FUNC_TYPE_NULL(_eg_retval); _eg_retval->isset=1; } }
#define EG_FUNC_RETVAL_BOOL(val)				{ if (!(_eg_retval->isset)) { _EG_FUNC_TYPE_BOOL(_eg_retval,val); _eg_retval->isset=1; } }
#define EG_FUNC_RETVAL_FALSE()  				{ if (!(_eg_retval->isset)) { _EG_FUNC_TYPE_FALSE(_eg_retval,); _eg_retval->isset=1; } }
#define EG_FUNC_RETVAL_TRUE()   				{ if (!(_eg_retval->isset)) { _EG_FUNC_TYPE_TRUE(_eg_retval,); _eg_retval->isset=1; } }
#define EG_FUNC_RETVAL_INT(val) 				{ if (!(_eg_retval->isset)) { _EG_FUNC_TYPE_INT(_eg_retval,val); _eg_retval->isset=1; } }
#define EG_FUNC_RETVAL_DOUBLE(val)				{ if (!(_eg_retval->isset)) { _EG_FUNC_TYPE_DOUBLE(_eg_retval,val); _eg_retval->isset=1; } }
#define EG_FUNC_RETVAL_STRINGL(str, len, dup)	{ if (!(_eg_retval->isset)) { _EG_FUNC_TYPE_STRINGL(_eg_retval,str, len, dup); _eg_retval->isset=1; } }
#define EG_FUNC_RETVAL_STRING(str, dup)			{ if (!(_eg_retval->isset)) { _EG_FUNC_TYPE_STRING(_eg_retval,str, dup); _eg_retval->isset=1; } }
#define EG_FUNC_RETVAL_ARRAY(val,dup)			{ if (!(_eg_retval->isset)) { _EG_FUNC_TYPE_ARRAY(_eg_retval,val,dup); _eg_retval->isset=1; } }

/* EG_FUNC_RETURN_xx */

#define EG_FUNC_RETURN_NULL()				{ EG_FUNC_RETVAL_NULL(); return; }
#define EG_FUNC_RETURN_BOOL(val)			{ EG_FUNC_RETVAL_BOOL(val); return; }
#define EG_FUNC_RETURN_FALSE()				{ EG_FUNC_RETVAL_FALSE(); return; }
#define EG_FUNC_RETURN_TRUE()				{ EG_FUNC_RETVAL_TRUE(); return; }
#define EG_FUNC_RETURN_INT(val)				{ EG_FUNC_RETVAL_INT(val); return; }
#define EG_FUNC_RETURN_DOUBLE(val)			{ EG_FUNC_RETVAL_DOUBLE(val); return; }
#define EG_FUNC_RETURN_STRINGL(str,len,dup)	{ EG_FUNC_RETVAL_STRINGL(str,len,dup); return; }
#define EG_FUNC_RETURN_STRING(str,dup)		{ EG_FUNC_RETVAL_STRING(str,dup); return; }
#define EG_FUNC_RETURN_ARRAY(val,dup)		{ EG_FUNC_RETVAL_ARRAY(val,dup); return; }

/* EG_FUNC_ARG_xx */

typedef struct
	{
	eg_bool is_unset;
	eg_bool _writable;
	_EG_FUNC_TYPE_VARS
	} _EG_FUNC_ARGUMENT;

typedef struct 
	{
	zval *zp;
	_EG_FUNC_ARGUMENT i;
	} _EG_FUNC_EXTERNAL_ARGUMENT;

#define EG_FUNC_ARG_NULL(argp)				   	{ if (argp->_writable) { _EG_FUNC_TYPE_RESET(argp); } }
#define EG_FUNC_ARG_BOOL(argp,val)			   	{ if (argp->_writable) { _EG_FUNC_TYPE_RESET(argp); _EG_FUNC_TYPE_BOOL(argp,val); } }
#define EG_FUNC_ARG_FALSE(argp)  			    { if (argp->_writable) { _EG_FUNC_TYPE_RESET(argp); _EG_FUNC_TYPE_FALSE(argp); } }
#define EG_FUNC_ARG_TRUE(argp)   			   	{ if (argp->_writable) { _EG_FUNC_TYPE_RESET(argp); _EG_FUNC_TYPE_TRUE(argp); } }
#define EG_FUNC_ARG_INT(argp,val) 			    { if (argp->_writable) { _EG_FUNC_TYPE_RESET(argp); _EG_FUNC_TYPE_INT(argp,val); } }
#define EG_FUNC_ARG_DOUBLE(argp,val)			{ if (argp->_writable) { _EG_FUNC_TYPE_RESET(argp); _EG_FUNC_TYPE_DOUBLE(argp,val); } }
#define EG_FUNC_ARG_STRINGL(argp,str, len, dup) { if (argp->_writable) { _EG_FUNC_TYPE_RESET(argp); _EG_FUNC_TYPE_STRINGL(argp,str, len, dup); } }
#define EG_FUNC_ARG_STRING(argp,str, dup)	    { if (argp->_writable) { _EG_FUNC_TYPE_RESET(argp); _EG_FUNC_TYPE_STRING(argp,str, dup); } }
#define EG_FUNC_ARG_ARRAY(argp,val,dup)			{ if (argp->_writable) { _EG_FUNC_TYPE_RESET(argp); _EG_FUNC_TYPE_ARRAY(argp,val,dup); } }

/*---------------------------------------------------------------*/
/*---- Memory management */

#define eg_eallocate(ptr, size) _eg_allocate(ptr, size, 0)
#define eg_pallocate(ptr, size) _eg_allocate(ptr, size, 1)

#define EG_PEALLOCATE(ptr,size,persistent) { ptr=_eg_allocate(ptr, size, persistent); }
#define EG_EALLOCATE(ptr, size)	EG_PEALLOCATE(ptr, size, 0)
#define EG_PALLOCATE(ptr, size)	EG_PEALLOCATE(ptr, size, 1)
#define EG_EFREE(ptr)			EG_EALLOCATE(ptr, 0)
#define EG_PFREE(ptr)			EG_PALLOCATE(ptr, 0)

#define eg_eduplicate(ptr, size) _eg_duplicate(ptr, size, 0)
#define eg_pduplicate(ptr, size) _eg_duplicate(ptr, size, 1)

/*---------------------------------------------------------------*/
/*--- Fixed value zvals */

#define EG_CZVAL(name) ( eg_czval_ ## name )

#define EG_DECLARE_CZVAL(name)	zval EG_CZVAL(name)

#define EG_INIT_CZVAL(name)	EG_INIT_CZVAL_VALUE(name, #name)

#define EG_INIT_CZVAL_VALUE(name,value)	\
	{ \
	INIT_ZVAL(EG_CZVAL(name)); \
	ZVAL_STRING(&(EG_CZVAL(name)), value,0); \
	}

/*---------------------------------------------------------------*/
/*--- Pre-computed string hashes */

typedef struct {
	char *string;
	unsigned int len;
	ulong hash;
} _EG_HKEY_STRUCT;

#define EG_HKEY(name) ( eg_hkey_ ## name )

#define EG_HKEY_STRING(name)	EG_HKEY(name).string
#define EG_HKEY_LEN(name)		EG_HKEY(name).len
#define EG_HKEY_HASH(name)		EG_HKEY(name).hash

#define EG_DECLARE_HKEY(name)	_EG_HKEY_STRUCT HKEY(name)

#define EG_INIT_HKEY(name) EG_INIT_HKEY_VALUE(name, #name)

#define EG_INIT_HKEY_LITERAL(name,value) EG_INIT_HKEY_STRING(name,value,sizeof( value ))

#define EG_INIT_HKEY_STRING(name,value,len) \
	{ \
	EG_HKEY_STRING(name)= value ; \
	EG_HKEY_LEN(name)=len; \
	EG_HKEY_HASH(name)=zend_get_hash_value(value,len); \
	}

#define EG_FIND_HKEY(ht,name,respp) \
	zend_hash_quick_find(ht,EG_HKEY_STRING(name) \
		,EG_HKEY_LEN(name),EG_HKEY_HASH(name),(void **)(respp))

#define EG_HKEY_EXISTS(ht,name) \
	zend_hash_quick_exists(ht,EG_HKEY_STRING(name), EG_HKEY_LEN(name), EG_HKEY_HASH(name))

/*--- Debug messages */

/*---------------------------------------------------------------*/
/*--- Debug messages */

#ifdef EXTGEN_DEBUG

/* If debug mode is on */

#ifdef HAVE_GETTIMEOFDAY
#define _EG_DBG_INIT() \
	{ \
	struct timezone _eg_dummy_tz; \
	(void)gettimeofday(&{{ uname }}_G(_eg_base_tp),&_eg_dummy_tz); \
	}

#define _EG_DBG_PRINT_TIME() \
	{ \
	struct timeval _eg_tp; \
	struct timezone _eg_tz; \
	time_t _eg_sec; \
	\
	(void)gettimeofday(&_eg_tp,&_eg_tz); \
	_eg_sec=_eg_tp.tv_sec-{{ uname }}_G(_eg_base_tp).tv_sec; \
	if (_eg_is_web()) php_printf("<br>"); \
	php_printf("<"); \
	if (_eg_sec) php_printf("%ld/",_eg_sec); \
	php_printf("%ld> : ",_eg_tp.tv_usec-{{ uname }}_G(_eg_base_tp).tv_usec); \
	(void)gettimeofday(&{{ uname }}_G(_eg_base_tp),&tz); \
	}

#else
#	define _EG_DBG_INIT()
#	define _EG_DBG_PRINT_TIME()
#endif

#	define EG_DBG_MSG(_format) { _EG_DBG_PRINT_TIME(); php_printf(_format "\n"); }
#	define EG_DBG_MSG1(_format,_var1) { _EG_DBG_PRINT_TIME(); php_printf(_format "\n",_var1); }
#	define EG_DBG_MSG2(_format,_var1,_var2) { _EG_DBG_PRINT_TIME(); php_printf(_format "\n",_var1,_var2); }
#	define EG_DBG_MSG3(_format,_var1,_var2,_var3) { _EG_DBG_PRINT_TIME(); php_printf(_format "\n",_var1,_var2,_var3); }
#	define EG_DBG_CHECK_MEM()	full_mem_check(1)

#else

/* If debug mode is off */

#	define EG_DBG_INIT()
#	define EG_DBG_MSG(_format)
#	define EG_DBG_MSG1(_format,_var1)
#	define EG_DBG_MSG2(_format,_var1,_var2)
#	define EG_DBG_MSG3(_format,_var1,_var2,_var3)
#	define EG_DBG_CHECK_MEM()

#endif

/*---------------------------------------------------------------*/
/*--- Exceptions */

#define EG_THROW_EXCEPTION(_format)	\
	{ \
	EG_DBG_MSG("Throwing exception: " _format); \
	(void)zend_throw_exception_ex(NULL,0 TSRMLS_CC ,_format); \
	}

#define EG_THROW_EXCEPTION_1(_format,_arg1)	\
	{ \
	EG_DBG_MSG1("Throwing exception: " _format , _arg1); \
	(void)zend_throw_exception_ex(NULL,0 TSRMLS_CC ,_format,_arg1); \
	}

#define EG_THROW_EXCEPTION_2(_format,_arg1,_arg2)	\
	{ \
	EG_DBG_MSG2("Throwing exception: " _format , _arg1, _arg2); \
	(void)zend_throw_exception_ex(NULL,0 TSRMLS_CC ,_format,_arg1,_arg2); \
	}

#define EG_THROW_EXCEPTION_3(_format,_arg1,_arg2,_arg3)	\
	{ \
	EG_DBG_MSG3("Throwing exception: " _format , _arg1, _arg2, _arg3); \
	(void)zend_throw_exception_ex(NULL,0 TSRMLS_CC ,_format,_arg1,_arg2,_arg3); \
	}

#define EG_EXCEPTION_ABORT(_format)	\
	{ \
	EG_THROW_EXCEPTION(_format); \
	return; \
	}

#define EG_EXCEPTION_ABORT_1(_format,_arg1)	\
	{ \
	EG_THROW_EXCEPTION_1(_format,_arg1); \
	return; \
	}

#define EG_EXCEPTION_ABORT_2(_format,_arg1,_arg2)	\
	{ \
	EG_THROW_EXCEPTION_2(_format,_arg1,_arg2); \
	return; \
	}

#define EG_EXCEPTION_ABORT_3(_format,_arg1,_arg2,_arg3)	\
	{ \
	EG_THROW_EXCEPTION_3(_format,_arg1,_arg2,_arg3); \
	return; \
	}

#define EG_EXCEPTION_ABORT_RET(_ret,_format)	\
	{ \
	EG_THROW_EXCEPTION(_format); \
	return _ret; \
	}

#define EG_EXCEPTION_ABORT_RET_1(_ret,_format,_arg1)	\
	{ \
	EG_THROW_EXCEPTION_1(_format,_arg1); \
	return _ret; \
	}

#define EG_EXCEPTION_ABORT_RET_2(_ret,_format,_arg1,_arg2)	\
	{ \
	EG_THROW_EXCEPTION_1(_format,_arg1,_arg2); \
	return _ret; \
	}

#define EG_EXCEPTION_ABORT_RET_3(_ret,_format,_arg1,_arg2,_arg3)	\
	{ \
	EG_THROW_EXCEPTION_1(_format,_arg1,_arg2,_arg3); \
	return _ret; \
	}

#define EXCEPTION_IS_PENDING	(EG(exception))

/*---------------------------------------------------------------*/
/*--- Path-related macros */

#ifdef PATH_MAX
#	define EG_PATH_MAX PATH_MAX
#else
#	define EG_PATH_MAX 1023
#endif

#define EG_CHECK_PATH_LEN(_zp,_delta) \
	{ \
	if (Z_STRLEN_P((_zp)) > (EG_PATH_MAX-_delta-1)) \
		{ \
		EG_EXCEPTION_ABORT_1("Path too long",NULL); \
		} \
	}

#define EG_CHECK_PATH_LEN_RET(_zp,_delta,_ret) \
	{ \
	if (Z_STRLEN_P((_zp)) > (PATH_MAX-_delta-1)) \
		{ \
		EG_EXCEPTION_ABORT_RET_1(_ret,"Path too long",NULL); \
		} \
	}


#define EG_ALLOC_PERMANENT_ZVAL(z)		{ z=eg_pallocate(NULL, sizeof(zval)); }

#define EG_ALLOC_INIT_PERMANENT_ZVAL(zp) { \
	EG_ALLOC_PERMANENT_ZVAL(zp); \
	INIT_ZVAL(*zp); \
	}

/*---------------------------------------------------------------*/
/*--- zval abstraction */

#define EG_Z_TYPE(z)			Z_TYPE(z)
#define EG_Z_TYPE_P(z)			Z_TYPE_P(z)

#define EG_Z_BVAL(z)			Z_BVAL(z)
#define EG_Z_BVAL_P(zp)	        Z_BVAL_P(zp)
#define EG_Z_IVAL(z)            Z_LVAL(z)
#define EG_Z_IVAL_P(zp)         Z_LVAL_P(zp)
#define EG_Z_DVAL(z)            Z_DVAL(z)
#define EG_Z_DVAL_P(zp)         Z_DVAL_P(zp)
#define EG_Z_STRVAL(z)          Z_STRVAL(z)
#define EG_Z_STRVAL_P(zp)       Z_STRVAL_P(zp)
#define EG_Z_STRLEN(z)          Z_STRLEN(z)
#define EG_Z_STRLEN_P(zp)       Z_STRLEN_P(zp)
#define EG_Z_ARRVAL(z)          Z_ARRVAL(z)
#define EG_Z_ARRVAL_P(zp)       Z_ARRVAL_P(zp)

#ifdef ZVAL_ARRAY
#	define EG_ZVAL_ARRAY(zp,ht)	ZVAL_ARRAY(zp,ht)
#else
#	define EG_ZVAL_ARRAY(zp,ht) { Z_TYPE_P(zp)=EG_IS_ARRAY; Z_ARRVAL_P(zp)=ht; }
#endif

#define EG_ZVAL_NULL(zp)		ZVAL_NULL(zp)

#define EG_ZVAL_BOOL(zp,val)	ZVAL_BOOL(zp,val)
#define EG_ZVAL_INT(zp,val)		ZVAL_LONG(zp,val)
#define EG_ZVAL_DOUBLE(zp,val)	ZVAL_DOUBLE(zp,val)

#define EG_ZVAL_STRING(zp,val,duplicate)		ZVAL_STRING(zp,val,duplicate)
#define EG_ZVAL_STRINGL(zp,val,len,duplicate)	ZVAL_STRINGL(zp,val,len,duplicate)
#define EG_ZVAL_EMPTY_STRING(zp)				ZVAL_EMPTY(zp)

#define EG_ZVAL_FALSE(zp)	ZVAL_FALSE(zp)
#define EG_ZVAL_TRUE(zp)	ZVAL_TRUE(zp)

#define _EG_ZVAL_IS_TYPE(zp,type) 	(EG_Z_TYPE_P((zp))==type)

#define EG_ZVAL_IS_NULL(zp)		_EG_ZVAL_IS_TYPE(zp,EG_IS_NULL)
#define EG_ZVAL_IS_BOOL(zp)		_EG_ZVAL_IS_TYPE(zp,EG_IS_BOOL)
#define EG_ZVAL_IS_INT(zp)		_EG_ZVAL_IS_TYPE(zp,EG_IS_INT)
#define EG_ZVAL_IS_DOUBLE(zp)	_EG_ZVAL_IS_TYPE(zp,EG_IS_DOUBLE)
#define EG_ZVAL_IS_STRING(zp)	_EG_ZVAL_IS_TYPE(zp,EG_IS_STRING)
#define EG_ZVAL_IS_ARRAY(zp)	_EG_ZVAL_IS_TYPE(zp,EG_IS_ARRAY)

#define EG_ZVAL_ENSURE_BOOL(zp) { if (EG_Z_TYPE_P((zp))!=EG_IS_BOOL) convert_to_boolean((zp)); }
#define EG_ZVAL_ENSURE_INT(zp) { if (EG_Z_TYPE_P((zp))!=EG_IS_INT) convert_to_long((zp)); }
#define EG_ZVAL_ENSURE_DOUBLE(zp) { if (EG_Z_TYPE_P((zp))!=EG_IS_DOUBLE) convert_to_double((zp)); }
#define EG_ZVAL_ENSURE_STRING(zp) { if (EG_Z_TYPE_P((zp))!=EG_IS_STRING) convert_to_string((zp)); }
#define EG_ZVAL_ENSURE_ARRAY(zp) { if (EG_Z_TYPE_P((zp))!=EG_IS_ARRAY) convert_to_array((zp)); }

/*---------------------------------------------------------------*/
/*--- API numbers for each minor version */

#define EG_PHP_5_0_X_API_NO                220040412
#define EG_PHP_5_1_X_API_NO                220051025
#define EG_PHP_5_2_X_API_NO                220060519
#define EG_PHP_5_3_X_API_NO                220090626
#define EG_PHP_5_4_X_API_NO                220100525
#define EG_PHP_5_5_X_API_NO                220121212
#define EG_PHP_5_6_X_API_NO                220131226

/*---------------------------------------------------------------*/
/*--- Numbers */

#define EG_CHAR_IS_DIGIT(c)		ZEND_IS_DIGIT(c)
#define EG_CHAR_IS_XDIGIT(c)	ZEND_IS_XDIGIT(c)

#define EG_CONVERT_DOUBLE_TO_INT(n)	zend_dval_to_lval(n)

#define EG_IS_NUMERIC_STRING(string,str_len,intp,doublep,allow_errors,overflowp) \
	is_numeric_string_ex(string,str_len,intp,doublep,allow_errors,overflowp)

/*---------------------------------------------------------------*/
/*--- Thread-safe stuff */

#ifdef ZTS
#define EG_MUTEX_DECLARE(x)			MUTEX_T x ## _mutex
#define EG_STATIC_MUTEX_DECLARE(x)	static MUTEX_T x ## _mutex
#define EG_MUTEX_SETUP(x)			x ## _mutex = tsrm_mutex_alloc()
#define EG_MUTEX_SHUTDOWN(x)		tsrm_mutex_free(x ## _mutex)
#define EG_MUTEX_LOCK(x)			tsrm_mutex_lock(x ## _mutex)
#define EG_MUTEX_UNLOCK(x)			tsrm_mutex_unlock(x ## _mutex)
#else
#define EG_MUTEX_DECLARE(x)
#define EG_STATIC_MUTEX_DECLARE(x)
#define EG_MUTEX_SETUP(x)
#define EG_MUTEX_SHUTDOWN(x)
#define EG_MUTEX_LOCK(x)
#define EG_MUTEX_UNLOCK(x)
#endif

/*---------------------------------------------------------------*/
/*--- Constants */

#define _EG_REGISTER_NULL_CONSTANT(name)         REGISTER_NULL_CONSTANT(name,CONST_CS|CONST_PERSISTENT)
#define _EG_REGISTER_BOOL_CONSTANT(name,val)     REGISTER_BOOL_CONSTANT(name,val ,CONST_CS|CONST_PERSISTENT)
#define _EG_REGISTER_INT_CONSTANT(name,val)     REGISTER_LONG_CONSTANT(name,val ,CONST_CS|CONST_PERSISTENT)
#define _EG_REGISTER_DOUBLE_CONSTANT(name,val)     REGISTER_DOUBLE_CONSTANT(name,val ,CONST_CS|CONST_PERSISTENT)
#define _EG_REGISTER_STRING_CONSTANT(name,val)     REGISTER_STRING_CONSTANT(name,val,CONST_CS|CONST_PERSISTENT)
#define _EG_REGISTER_STRINGL_CONSTANT(name,val,len) REGISTER_STRINGL_CONSTANT(name,val,len,CONST_CS|CONST_PERSISTENT)

/*---------------------------------------------------------------*/
/*--- extgen-specific */


/*---------------------------------------------------------------*/
/*--- Miscellaneoous */

#define EG_PHP_FUNCTION_NAME(name)	ZEND_FN(name)
#define EG_PHP_METHOD_NAME(name)	ZEND_MN(name)

#define EG_EXTENSION_IS_LOADED(name)	_eg_extension_is_loaded(name)

#define EG_EXIT(status) { EG(exit_status) = status; zend_bailout(); }

#define EG_CLEAR_DATA(_v)	memset(&(_v),'\0',sizeof(_v)); \

#if PHP_API_VERSION >= 20100412
	typedef size_t EG_PHP_ESCAPE_HTML_ENTITIES_SIZE;
#else
	typedef int EG_PHP_ESCAPE_HTML_ENTITIES_SIZE;
#endif

/*============================================================================*/
/*--- Function declarations */

static void *_eg_allocate(void *ptr, size_t size, int persistent);
static void *_eg_duplicate(void *ptr, size_t size, int persistent);
static HashTable *_eg_zval_array_duplicate(HashTable *source_ht);
static int _eg_extension_is_loaded(char *name TSRMLS_DC);
static void _eg_convert_arg_zp_to_scalar(eg_type target_type,zval *zp,_EG_FUNC_ARGUMENT *ip);

/*============================================================================*/
