/*============================================================================*/
/* All these functions are exposed through macros */

/*---------------------------------------------------------------*/
/* memory alloc/realloc/free/duplicate                           */

static void *_eg_allocate(void *ptr, size_t size, int persistent)
{
if (ptr)
	{
	if (size) ptr=perealloc(ptr, size, persistent);
	else
		{
		pefree(ptr, persistent);
		ptr=NULL;
		}
	}
else
	{
	if (size) ptr=pemalloc(size, persistent);
	/* _eg_allocate(NULL,0) does nothing */
	}
return ptr;
}
	
/*----------*/

static void *_eg_duplicate(void *ptr, size_t size, int persistent)
{
char *p;

if (!ptr) return NULL;

/* Don't transmit 0 size to _eg_allocate, as it would free mem */
if (!size) return _eg_allocate(NULL,1,persistent);

p=_eg_allocate(NULL,size,persistent);
memmove(p,ptr,size);
return p;
}

/*---------------------------------------------------------------*/
/* Adapted from zval_copy_ctor() */
/* If no destructor provided, defaults to ZVAL_PTR_DTOR */

static HashTable *_eg_zval_array_duplicate(HashTable *source_ht)
{
HashTable *dest_ht;
zval *tmp;

dest_ht=(HashTable *)eg_eallocate(NULL,sizeof(HashTable));
zend_hash_init(dest_ht, zend_hash_num_elements(source_ht), NULL, ZVAL_PTR_DTOR, 0);
zend_hash_copy(dest_ht, source_ht, (copy_ctor_func_t) zval_add_ref, (void *) &tmp, sizeof(zval *));
dest_ht->nNextFreeElement = source_ht->nNextFreeElement;
return dest_ht;
}

/*---------------------------------------------------------------*/

static int _eg_extension_is_loaded(char *name TSRMLS_DC)
{
int status;

status=zend_hash_exists(&module_registry, name, strlen(name)+1);
EG_DBG_MSG2("Checking if extension %s is loaded: %s",name,(status ? "yes" : "no"));
return status;
}

/*---------------------------------------------------------------*/

static void _eg_convert_arg_zp_to_scalar(eg_type target_type,zval *zp
	,_EG_FUNC_ARGUMENT *ip)
{
switch(target_type)
	{
	case EG_IS_BOOL:
		EG_ZVAL_ENSURE_BOOL(zp);
		_EG_FUNC_TYPE_BOOL(ip,EG_Z_BVAL_P(zp));
		break;

	case EG_IS_INT:
		EG_ZVAL_ENSURE_INT(zp);
		_EG_FUNC_TYPE_INT(ip,EG_Z_IVAL_P(zp));
		break;

	case EG_IS_FLOAT:
		EG_ZVAL_ENSURE_FLOAT(zp);
		_EG_FUNC_TYPE_FLOAT(ip,EG_Z_FVAL_P(zp));
		break;

	case EG_IS_STRING:
		EG_ZVAL_ENSURE_STRING(zp);
		_EG_FUNC_TYPE_STRINGL(ip,EG_Z_STRVAL_P(zp),EG_Z_STRLEN_P(zp),1);
		break;
	}
}
