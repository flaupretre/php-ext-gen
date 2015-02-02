
/*--- Defining resource {{ resource.name }} */

static eg_restype EG_RESOURCE_TYPE({{ resource.name }})=0;

/* Data structure */

typedef struct {
int _eg_persistent;
eg_restype _eg_type;
{% block user_resource_struct %}{% endblock %}
} EG_RESOURCE_STRUCT({{ resource.name }});

/* Internal destructor */

static void _eg_resource_dtor_{{ resource.name }}_int(
	EG_RESOURCE_STRUCT({{ resource.name }})* ptr, int persistent)
{
{% block user_resource_dtor %}{% endblock %}
{# Dont free struct here as user code could contain a 'return' #}
}

/* External destructor - persistent */

static void _eg_resource_dtor_{{ resource.name }}_ext_p(zend_rsrc_list_entry *resource)
{
EG_RESOURCE_STRUCT({{ resource.name }})* ptr;

if (resource && resource->ptr)
	{
	ptr=(EG_RESOURCE_STRUCT({{ resource.name }})*)(resource->ptr);
	if (ptr->_eg_persistent) {
		_eg_resource_dtor_{{ resource.name }}_int(ptr, 1);
		EG_PALLOCATE(resource->ptr,0);
		}
	}
}

/* External destructor - non-persistent */

static void _eg_resource_dtor_{{ resource.name }}_ext_np(zend_rsrc_list_entry *resource)
{
EG_RESOURCE_STRUCT({{ resource.name }}) *ptr;

if (resource && resource->ptr)
	{
	ptr=(EG_RESOURCE_STRUCT({{ resource.name }})*)(resource->ptr);
	if (!(ptr->_eg_persistent)) {
		_eg_resource_dtor_{{ resource.name }}_int(ptr, 0);
		EG_EALLOCATE(resource->ptr,0);
		}
	}
}
