
/*============================================================================*/
/* Generic arginfo structures */

ZEND_BEGIN_ARG_INFO_EX(EG_UT_noarg_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(EG_UT_1arg_arginfo, 0, 0, 1)
ZEND_ARG_INFO(0, arg1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(EG_UT_2args_arginfo, 0, 0, 2)
ZEND_ARG_INFO(0, arg1)
ZEND_ARG_INFO(0, arg2)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(EG_UT_3args_arginfo, 0, 0, 3)
ZEND_ARG_INFO(0, arg1)
ZEND_ARG_INFO(0, arg2)
ZEND_ARG_INFO(0, arg3)
ZEND_END_ARG_INFO()

#ifdef RETURN_BY_REF_IS_BROKEN

ZEND_BEGIN_ARG_INFO_EX(EG_UT_noarg_ref_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(EG_UT_1arg_ref_arginfo, 0, 0, 1)
ZEND_ARG_INFO(0, arg1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(EG_UT_2args_ref_arginfo, 0, 0, 2)
ZEND_ARG_INFO(0, arg1)
ZEND_ARG_INFO(0, arg2)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(EG_UT_3args_ref_arginfo, 0, 0, 3)
ZEND_ARG_INFO(0, arg1)
ZEND_ARG_INFO(0, arg2)
ZEND_ARG_INFO(0, arg3)
ZEND_END_ARG_INFO()

#else

ZEND_BEGIN_ARG_INFO_EX(EG_UT_noarg_ref_arginfo, 0, 1, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(EG_UT_1arg_ref_arginfo, 0, 1, 1)
ZEND_ARG_INFO(0, arg1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(EG_UT_2args_ref_arginfo, 0, 1, 2)
ZEND_ARG_INFO(0, arg1)
ZEND_ARG_INFO(0, arg2)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(EG_UT_3args_ref_arginfo, 0, 1, 3)
ZEND_ARG_INFO(0, arg1)
ZEND_ARG_INFO(0, arg2)
ZEND_ARG_INFO(0, arg3)
ZEND_END_ARG_INFO()

#endif

/*============================================================================*/
/*---------------------------------------------------------------*/
/* memory alloc/realloc/free/duplicate                           */

static void *eg_ut_allocate(void *ptr, size_t size, int persistent)
{
	if (ptr) {
		if (size) ptr=perealloc(ptr, size, persistent);
		else {
			pefree(ptr, persistent);
			ptr=NULL;
		}
	} else {
		if (size) ptr=pemalloc(size, persistent);
	}
return ptr;
}
	
#define eg_ut_eallocate(ptr, size) eg_ut_allocate(ptr, size, 0)
#define eg_ut_pallocate(ptr, size) eg_ut_allocate(ptr, size, 1)

#define PEALLOCATE(ptr, size, persistent)	ptr=eg_ut_allocate(ptr, size, persistent)
#define EALLOCATE(ptr, size)	PEALLOCATE(ptr, size, 0)
#define PALLOCATE(ptr, size)	PEALLOCATE(ptr, size, 1)
#define EFREE(ptr)				EALLOCATE(ptr, 0)
#define PFREE(ptr)				PALLOCATE(ptr, 0)

/*---------------------------------------------------------------*/

static void *eg_ut_duplicate(void *ptr, size_t size, int persistent)
{
	char *p;

	if (!ptr) return NULL;
	if (size==0) return eg_ut_allocate(NULL,1,persistent);

	p=eg_ut_allocate(NULL,size,persistent);
	memmove(p,ptr,size);
	return p;
}

#define eg_ut_eduplicate(ptr, size) eg_ut_duplicate(ptr, size, 0)
#define eg_ut_pduplicate(ptr, size) eg_ut_duplicate(ptr, size, 1)

/*---------------------------------------------------------------*/
/* Debug messages */

#ifdef EXTGEN_DEBUG

#ifdef HAVE_GETTIMEOFDAY
static struct timeval _eg_ut_base_tp;
#endif

/*------------*/

EG_UT_SYMBOL void eg_ut_dbg_init_time()
{
#ifdef HAVE_GETTIMEOFDAY
	struct timezone tz;

	(void)gettimeofday(&_eg_ut_base_tp,&tz);
#endif
}

/*------------*/

EG_UT_SYMBOL void eg_ut_dbg_print_time()
{
#ifdef HAVE_GETTIMEOFDAY
	struct timeval tp;
	struct timezone tz;
	time_t sec;

	(void)gettimeofday(&tp,&tz);
	sec=tp.tv_sec-_eg_ut_base_tp.tv_sec;
	if (eg_ut_is_web()) php_printf("<br>");
	php_printf("<");
	if (sec) php_printf("%ld/",sec);
	php_printf("%ld> : ",tp.tv_usec-_eg_ut_base_tp.tv_usec);
	(void)gettimeofday(&_eg_ut_base_tp,&tz);
#endif
}

#endif	/* EXTGEN_DEBUG */

/*---------------------------------------------------------------*/

EG_UT_SYMBOL void eg_ut_decref(zval *zp)
{
	Z_DELREF_P(zp);
	if (Z_REFCOUNT_P(zp)<=1) Z_UNSET_ISREF_P(zp);
}

/*---------*/
/* Free zval content and reset it */

EG_UT_SYMBOL void eg_ut_pezval_dtor(zval *zp, int persistent)
{
	if (persistent) {
		switch (Z_TYPE_P(zp) & IS_CONSTANT_TYPE_MASK) {
		  case IS_STRING:
		  case IS_CONSTANT:
			  pefree(Z_STRVAL_P(zp), persistent);
			  break;

		  case IS_ARRAY:
		  case IS_CONSTANT_ARRAY:
			  zend_hash_destroy(Z_ARRVAL_P(zp));
			  pefree(Z_ARRVAL_P(zp), persistent);
			  break;
		}
	} else {
		zval_dtor(zp);
	}
	INIT_ZVAL(*zp);
}

/*---------*/

EG_UT_SYMBOL void eg_ut_ezval_dtor(zval *zp) { eg_ut_pezval_dtor(zp,0); }
EG_UT_SYMBOL void eg_ut_pzval_dtor(zval *zp) { eg_ut_pezval_dtor(zp,1); }

/*---------*/
/* clear the zval pointer */

EG_UT_SYMBOL void eg_ut_pezval_ptr_dtor(zval ** zpp, int persistent)
{
	if (*zpp) {
		if (persistent) {
			eg_ut_decref(*zpp);
			/* php_printf("Reference count = %d\n",Z_REFCOUNT_PP(zpp)); */
			if (Z_REFCOUNT_PP(zpp) == 0) {
				eg_ut_pzval_dtor(*zpp);
				GC_REMOVE_ZVAL_FROM_BUFFER(*zpp);
				eg_ut_pallocate(*zpp, 0);
			} 
		} else {
			zval_ptr_dtor(zpp);
		}
		(*zpp)=NULL;
	}
}

/*---------*/

EG_UT_SYMBOL void eg_ut_ezval_ptr_dtor(zval **zpp) { eg_ut_pezval_ptr_dtor(zpp,0); }
EG_UT_SYMBOL void eg_ut_pzval_ptr_dtor(zval **zpp) { eg_ut_pezval_ptr_dtor(zpp,1); }

/*---------*/

EG_UT_SYMBOL void eg_ut_persistent_array_init(zval * zp)
{
	HashTable *htp;

	htp=eg_ut_pallocate(NULL,sizeof(HashTable));
	(void)zend_hash_init(htp,0, NULL,(dtor_func_t)eg_ut_pzval_ptr_dtor,1);
	INIT_PZVAL(zp);
	ZVAL_ARRAY(zp, htp);
}

/*---------*/

EG_UT_SYMBOL void eg_ut_persistent_copy_ctor(zval ** ztpp)
{
	*ztpp=eg_ut_persist_zval(*ztpp);
}

/*---------*/
/* Duplicates a zval and all its descendants to persistent storage */
/* Does not support objects and resources */

EG_UT_SYMBOL zval *eg_ut_persist_zval(zval * zsp)
{
	int type, len;
	char *p;
	zval *ztp;

	ALLOC_PERMANENT_ZVAL(ztp);
	INIT_PZVAL_COPY(ztp,zsp);

	switch (type = Z_TYPE_P(ztp)) {	/* Default: do nothing (when no additional data) */
	  case IS_STRING:
	  case IS_CONSTANT:
		  len = Z_STRLEN_P(zsp);
		  p=eg_ut_pduplicate(Z_STRVAL_P(zsp), len + 1);
		  ZVAL_STRINGL(ztp, p, len, 0);
		  break;

	  case IS_ARRAY:
	  case IS_CONSTANT_ARRAY:
		  eg_ut_persistent_array_init(ztp);
		  zend_hash_copy(Z_ARRVAL_P(ztp), Z_ARRVAL_P(zsp)
						 , (copy_ctor_func_t) eg_ut_persistent_copy_ctor,
						 NULL, sizeof(zval *));
		  Z_TYPE_P(ztp) = type;
		  break;

	  case IS_OBJECT:
	  case IS_RESOURCE:
			{
			TSRMLS_FETCH();
			EXCEPTION_ABORT_RET(NULL,"Cannot make resources/objects persistent");
			}
	}
	return ztp;
}

/*---------------------------------------------------------------*/

EG_UT_SYMBOL zval *eg_ut_new_instance(char *class_name, int class_name_len,
	int construct, int nb_args,	zval ** args TSRMLS_DC)
{
	zend_class_entry **ce;
	zval *instance;

	if (zend_lookup_class_ex(class_name, class_name_len
#if PHP_API_VERSION >= 20100412
/* PHP 5.4+: additional argument to zend_lookup_class_ex */
							, NULL
#endif
							, 1, &ce TSRMLS_CC) == FAILURE) {
		EXCEPTION_ABORT_RET_1(NULL,"%s: class does not exist",class_name);
	}

	ALLOC_INIT_ZVAL(instance);
	object_init_ex(instance, *ce);

	if (construct) {
		eg_ut_call_user_function_void(instance, ZEND_STRL("__construct"), nb_args,
			args TSRMLS_CC);
	}

	return instance;
}

/*---------------------------------------------------------------*/

EG_UT_SYMBOL void eg_ut_call_user_function_void(zval *obj_zp, char *func,
	int func_len, int nb_args, zval ** args TSRMLS_DC)
{
	zval *ret;

	ALLOC_INIT_ZVAL(ret);
	eg_ut_call_user_function(obj_zp, func, func_len, ret, nb_args, args TSRMLS_CC);
	eg_ut_ezval_ptr_dtor(&ret);		/* Discard return value */
}

/*---------------------------------------------------------------*/

EG_UT_SYMBOL int eg_ut_call_user_function_bool(zval * obj_zp, char *func,
	int func_len, int nb_args, zval ** args TSRMLS_DC)
{
	zval *ret;
	int result;

	ALLOC_INIT_ZVAL(ret);
	eg_ut_call_user_function(obj_zp, func, func_len, ret, nb_args, args TSRMLS_CC);
	result = zend_is_true(ret);
	eg_ut_ezval_ptr_dtor(&ret);

	return result;
}

/*---------------------------------------------------------------*/

EG_UT_SYMBOL long eg_ut_call_user_function_long(zval *obj_zp, char *func,
	int func_len, int nb_args, zval ** args TSRMLS_DC)
{
	zval *ret;
	long result;

	ALLOC_INIT_ZVAL(ret);
	eg_ut_call_user_function(obj_zp, func, func_len, ret, nb_args, args TSRMLS_CC);

	ENSURE_LONG(ret);
	result=Z_LVAL_P(ret);
	eg_ut_ezval_ptr_dtor(&ret);

	return result;
}

/*---------------------------------------------------------------*/

EG_UT_SYMBOL void eg_ut_call_user_function_string(zval *obj_zp, char *func,
	int func_len, zval * ret, int nb_args, zval ** args TSRMLS_DC)
{
	eg_ut_call_user_function(obj_zp, func, func_len, ret, nb_args, args TSRMLS_CC);
	if (EG(exception)) return;

	ENSURE_STRING(ret);
}

/*---------------------------------------------------------------*/

EG_UT_SYMBOL void eg_ut_call_user_function_array(zval * obj_zp, char *func,
	int func_len, zval * ret, int nb_args, zval ** args TSRMLS_DC)
{
	eg_ut_call_user_function(obj_zp, func, func_len, ret, nb_args, args TSRMLS_CC);
	if (EG(exception)) return;

	if (!ZVAL_IS_ARRAY(ret)) {
		THROW_EXCEPTION_2("%s method should return an array (type=%d)",
			func, Z_TYPE_P(ret));
	}
}

/*---------------------------------------------------------------*/

EG_UT_SYMBOL void eg_ut_call_user_function(zval *obj_zp, char *func,
	int func_len, zval *ret, int nb_args, zval ** args TSRMLS_DC)
{
	int status;
	zval *func_zp;

#if ZEND_MODULE_API_NO <= 20050922
/* PHP version 5.1 doesn't accept 'class::function' as func_zp (static method).
It requires passing the class name as a string zval in obj_zp, and the method
name in func_zp. Unfortunately, this mechanism is rejected by PHP 5.3, which
requires a null obj_zp and a string 'class::function' in func_zp. This function
always receives 'class::function' and makes it compatible with old versions.
Note: I am not sure of the ZEND_MODULE_API value where this behavior changed. If
it is wrong, let me know. The only thing I know is that it changed between 5.1.6
and 5.3.9 */

	char *p,*op,*p2;
	int clen;

	MAKE_STD_ZVAL(func_zp);
	clen=0;
	if (!obj_zp) { /* Only on static calls */
		for (op=p=func;;p++) {
			if (!(*p)) break;
			if (((*p)==':')&&((*(p+1))==':')) {
				clen=p-op;
				break;
			}
		}
	}	
	if (clen) {
		p2=eg_ut_eduplicate(op,clen+1);
		p2[clen]='\0';
		MAKE_STD_ZVAL(obj_zp);
		ZVAL_STRINGL(obj_zp,p2,clen,0);
		ZVAL_STRINGL(func_zp,op+clen+2,func_len-clen-2,1);
	} else {
		ZVAL_STRINGL(func_zp,func,func_len,1);	/* Default */
	}
#else
	MAKE_STD_ZVAL(func_zp);
	ZVAL_STRINGL(func_zp,func,func_len,1);
#endif

	status=call_user_function(EG(function_table), &obj_zp, func_zp, ret, nb_args,
		args TSRMLS_CC);
	eg_ut_ezval_ptr_dtor(&func_zp);

#if ZEND_MODULE_API_NO <= 20050922
	if (clen) {
		eg_ut_ezval_ptr_dtor(&obj_zp);
	}
#endif

	if (status!=SUCCESS) {
		THROW_EXCEPTION_1("call_user_function(func=%s) failed",func);
	}
}

/*---------------------------------------------------------------*/

EG_UT_SYMBOL int eg_ut_extension_loaded(char *name, int len TSRMLS_DC)
{
	int status;

	status=zend_hash_exists(&module_registry, name, len + 1);
	DBG_MSG2("Checking if extension %s is loaded: %s",name,(status ? "yes" : "no"));
	return status;
}

/*---------------------------------------------------------------*/

EG_UT_SYMBOL void eg_ut_load_extension_file(zval *file TSRMLS_DC)
{
	if (!eg_ut_call_user_function_bool(NULL,ZEND_STRL("dl"),1,&file TSRMLS_CC)) {
		THROW_EXCEPTION_1("%s: Cannot load extension",Z_STRVAL_P(file));
	}
}

/*---------------------------------------------------------------*/

#ifdef PHP_WIN32
#define _EG_UT_LE_PREFIX "php_"
#else
#define _EG_UT_LE_PREFIX
#endif

EG_UT_SYMBOL void eg_ut_load_extension(char *name, int len TSRMLS_DC)
{
	zval *zp;
	char *p;

	if (eg_ut_extension_loaded(name, len TSRMLS_CC)) return;

	spprintf(&p,MAXPATHLEN,_EG_UT_LE_PREFIX "%s." PHP_SHLIB_SUFFIX,name);
	MAKE_STD_ZVAL(zp);
	ZVAL_STRING(zp,p,0);

	eg_ut_load_extension_file(zp TSRMLS_CC);

	eg_ut_ezval_ptr_dtor(&zp);
}

/*---------------------------------------------------------------*/

EG_UT_SYMBOL void eg_ut_load_extensions(zval * extensions TSRMLS_DC)
{
	HashTable *ht;
	HashPosition pos;
	zval **zpp;

	if (!ZVAL_IS_ARRAY(extensions)) {
		THROW_EXCEPTION("eg_ut_load_extensions: argument should be an array");
		return;
	}

	ht = Z_ARRVAL_P(extensions);

	zend_hash_internal_pointer_reset_ex(ht, &pos);
	while (zend_hash_get_current_data_ex(ht, (void **) (&zpp), &pos) ==
		   SUCCESS) {
		if (ZVAL_IS_STRING(*zpp)) {
			eg_ut_load_extension(Z_STRVAL_PP(zpp),Z_STRLEN_PP(zpp) TSRMLS_CC);
			if (EG(exception)) return;
		}
		zend_hash_move_forward_ex(ht, &pos);
	}
}

/*---------------------------------------------------------------*/

EG_UT_SYMBOL void eg_ut_require(char *string, zval * ret TSRMLS_DC)
{
	char *p;

	spprintf(&p, MAXPATHLEN+12, "require '%s';", string);

	zend_eval_string(p, ret, "eval" TSRMLS_CC);

	EFREE(p);
}

/*---------------------------------------------------------------*/

EG_UT_SYMBOL int eg_ut_strings_are_equal(zval * zp1, zval * zp2 TSRMLS_DC)
{
	if ((!zp1) || (!zp2))
		return 0;

	ENSURE_STRING(zp1);
	ENSURE_STRING(zp2);

	if (Z_STRLEN_P(zp1) != Z_STRLEN_P(zp2)) return 0;

	return (!memcmp(Z_STRVAL_P(zp1), Z_STRVAL_P(zp1), Z_STRLEN_P(zp1)));
}

/*---------------------------------------------------------------*/

EG_UT_SYMBOL void eg_ut_exit(int status TSRMLS_DC)
{
	EG(exit_status) = status;
	zend_bailout();
}

/*---------------------------------------------------------------*/
/* Basic (and fast) file_get_contents(). Ignores safe mode and magic quotes */

EG_UT_SYMBOL void eg_ut_file_get_contents(char *path, zval *ret TSRMLS_DC)
{
	php_stream *stream;
	char *contents;
	int len;

	stream=php_stream_open_wrapper(path,"rb",REPORT_ERRORS,NULL);
	if (!stream) EXCEPTION_ABORT_1("%s : Cannot open file",path);

	len=php_stream_copy_to_mem(stream,&contents,PHP_STREAM_COPY_ALL,0);
	php_stream_close(stream);

	if (len < 0) EXCEPTION_ABORT_1("%s : Cannot read file",path);

	eg_ut_ezval_dtor(ret);
	ZVAL_STRINGL(ret,contents,len,0);
}

/*========================================================================*/
