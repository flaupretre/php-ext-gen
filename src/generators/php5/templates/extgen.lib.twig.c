/*============================================================================*/
/* The functions starting with '_' are not exposed directly to user */

/*---------------------------------------------------------------*/
/* memory alloc/realloc/free/duplicate                           */

static void *eg_allocate(void *ptr, size_t size, int persistent)
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

static void *eg_duplicate(void *ptr, size_t size, int persistent)
{
void *p;

if (!ptr) return NULL;

/* Don't transmit 0 size to _eg_allocate, as it would free mem */
if (!size) return eg_allocate(NULL,1,persistent);

p=eg_allocate(NULL,size,persistent);
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
	,EG_ARGUMENT *ip TSRMLS_CC)
{
eg_resource res;
eg_restype rtype;
void *ptr;

_EG_VARS_SET_TYPE(ip,target_type);
switch(target_type)
	{
	case EG_IS_BOOL:
		EG_ZVAL_ENSURE_BOOL(zpp);
		_EG_VARS_BOOL(ip,EG_Z_BVAL_PP(zpp));
		break;

	case EG_IS_INT:
		EG_ZVAL_ENSURE_INT(zpp);
		_EG_VARS_INT(ip,EG_Z_IVAL_PP(zpp));
		break;

	case EG_IS_FLOAT:
		EG_ZVAL_ENSURE_FLOAT(zpp);
		_EG_VARS_FLOAT(ip,EG_Z_FVAL_PP(zpp));
		break;

	case EG_IS_STRING:
		EG_ZVAL_ENSURE_STRING(zpp);
		_EG_VARS_STRINGL(ip,EG_Z_STRVAL_PP(zpp),EG_Z_STRLEN_PP(zpp),0);
		break;

	case EG_IS_RESOURCE:
		res=0;
		rtype=0;
		ptr=NULL;
		if (Z_TYPE_PP(zpp)==IS_RESOURCE) {
			res=EG_Z_RESVAL_PP(zpp);
			ptr=zend_list_find(res,&rtype);
			if (!ptr) res=0; /* Invalid resource */
			}
		_EG_VARS_RESOURCE(ip,res);
		EG_RES_TYPE(ip)=rtype;
		EG_RES_PTR(ip)=ptr;
		break;
	}
}

/* Resources ============================================================*/

/*---------------*/

static void *_eg_resource_alloc(size_t size, eg_restype type, int persistent)
{
void *ptr;

ptr=eg_allocate(NULL,size,persistent);
memset(ptr,'\0',size);
((_eg_resource_common_data *)ptr)->_eg_persistent=persistent;
((_eg_resource_common_data *)ptr)->_eg_type=type;

return ptr;
}

/*---------------*/
/* Private - Build a composite key from the original key and resource type */

static char *_eg_resource_persistent_key(eg_restype type, char *key, size_t keylen)
{
char *tkey;

tkey=eg_eallocate(NULL,keylen+3);
tkey[0]=(type & 0xff);
tkey[1]=((type >> 8) & 0xff);
memmove(tkey+2,key,keylen+1);

return tkey;
}

/*---------------*/

static void _eg_resource_persistent_register(void *ptr, char *key, eg_size keylen TSRMLS_DC)
{
zend_rsrc_list_entry le;
char *tkey;

le.ptr=ptr;
le.type=((_eg_resource_common_data *)ptr)->_eg_type;
le.refcount=1;

tkey=_eg_resource_persistent_key(le.type,key,keylen);

zend_hash_add(&EG(persistent_list),tkey,keylen+3,&le, sizeof(le), NULL);

EG_EALLOCATE(tkey,0);
}

/*---------------*/

static void *_eg_resource_persistent_find(eg_restype type, char *key, eg_size keylen TSRMLS_DC)
{
zend_rsrc_list_entry *le;
char *tkey;


tkey=_eg_resource_persistent_key(type,key,keylen);

if (zend_hash_find(&EG(persistent_list),tkey,keylen+3, (void **)(&le)) == SUCCESS)
	return le->ptr;

return NULL;
}
