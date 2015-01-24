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

static void _eg_convert_arg_zpp_to_scalar(eg_type target_type,zval **zpp
	,_EG_FUNC_ARGUMENT *ip TSRMLS_CC)
{
int id,type;
void *ptr;

switch(target_type)
	{
	case EG_IS_BOOL:
		EG_ZVAL_ENSURE_BOOL(zpp);
		_EG_FUNC_TYPE_BOOL(ip,EG_Z_BVAL_PP(zpp));
		break;

	case EG_IS_INT:
		EG_ZVAL_ENSURE_INT(zpp);
		_EG_FUNC_TYPE_INT(ip,EG_Z_IVAL_PP(zpp));
		break;

	case EG_IS_FLOAT:
		EG_ZVAL_ENSURE_FLOAT(zpp);
		_EG_FUNC_TYPE_FLOAT(ip,EG_Z_FVAL_PP(zpp));
		break;

	case EG_IS_STRING:
		EG_ZVAL_ENSURE_STRING(zpp);
		_EG_FUNC_TYPE_STRINGL(ip,EG_Z_STRVAL_PP(zpp),EG_Z_STRLEN_PP(zpp),0);
		break;

	case EG_IS_RESOURCE:
		EG_ZVAL_ENSURE_RESOURCE(zpp);
		ptr=EG_RESOURCE_FIND(id=EG_Z_RESVAL_PP(zpp),&type);
		_EG_FUNC_TYPE_RESOURCE(ip,id,type,ptr);
		break;
	}
}

/*---------------*/

static eg_resource _eg_res_register(void *ptr, int type, int persistent TSRMLS_DC)
{
HashTable *ht;
int index;
zend_rsrc_list_entry le;

ht=_EG_RESOURCE_HASHTABLE(persistent);

le.ptr=ptr;
le.type=type;
le.refcount=1;

index = zend_hash_next_free_element(ht);
zend_hash_index_update(ht, index, (void *) &le, sizeof(zend_rsrc_list_entry), NULL);

return (eg_resource)(persistent ? -index : index);
}

/*---------------*/

static int _eg_res_delete(eg_resource id TSRMLS_DC)
{
HashTable *ht;
zend_rsrc_list_entry *le;

ht=_EG_RESOURCE_HASHTABLE(EG_RESOURCE_IS_PERSISTENT(id));

if (zend_hash_index_find(ht, id, (void **) &le)==SUCCESS) 
	return ((--le->refcount<=0) ? zend_hash_index_del(ht, id) : SUCCESS);

return FAILURE;
}

/*---------------*/

static void *_eg_res_find(eg_resource id, int *type TSRMLS_DC)
{
HashTable *ht;
zend_rsrc_list_entry *le;

ht=_EG_RESOURCE_HASHTABLE(EG_RESOURCE_IS_PERSISTENT(id));
if (zend_hash_index_find(ht, id, (void **) &le)==SUCCESS)
	{
	*type = le->type;
	return le->ptr;
	}
else
	{
	*type = -1;
	return NULL;
	}
}

/*---------------*/

static int _eg_res_addref(eg_resource id TSRMLS_DC)
{
HashTable *ht;
zend_rsrc_list_entry *le;

ht=_EG_RESOURCE_HASHTABLE(EG_RESOURCE_IS_PERSISTENT(id));
	
if (zend_hash_index_find(ht, id, (void **) &le)==SUCCESS)
	{
	le->refcount++;
	return SUCCESS;
	}

return FAILURE;
}
