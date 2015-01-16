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

static int _eg_extension_is_loaded(char *name TSRMLS_DC)
{
int status;

status=zend_hash_exists(&module_registry, name, strlen(name)+1);
DBG_MSG2("Checking if extension %s is loaded: %s",name,(status ? "yes" : "no"));
return status;
}
