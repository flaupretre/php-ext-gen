
/*--- Defining resource {{ resource.name }} */

static eg_restype EG_RESOURCE_TYPE({{ resource.name }})=0;

/* Data structure */

typedef struct {
{% block user_resource_struct %}
} EG_RESOURCE_STRUCT({{ resource.name }});

/* Internal destructor */

static void _EG_RESOURCE_DTOR_BASE({{ resource.name }})_int(
	EG_RESOURCE_STRUCT({{ resource.name }})* ptr, int persistent)
{
{% block user_resource_dtor %}
{# Dont free struct here as user code could contain a 'return' #}
}

/* External destructor - persistent */

static void _EG_RESOURCE_DTOR({{ resource.name }})_ext_p(zend_rsrc_list_entry *rsrc)
{
if (rsrc && rsrc->ptr)
	{
	_EG_RESOURCE_DTOR_BASE({{ resource.name }})_int(
		(EG_RESOURCE_STRUCT({{ resource.name }})*)(rsrc->ptr), 1);
	EG_PALLOCATE(rsrc->ptr,0);
	}
}

/* External destructor - non-persistent */

static void _EG_RESOURCE_DTOR({{ resource.name }})_ext_d(zend_rsrc_list_entry *rsrc)
{
if (rsrc && rsrc->ptr)
	{
	_EG_RESOURCE_DTOR_BASE({{ resource.name }})_int(
		(EG_RESOURCE_STRUCT({{ resource.name }})*)(rsrc->ptr), 0);
	EG_EALLOCATE(rsrc->ptr,0);
	}
}
