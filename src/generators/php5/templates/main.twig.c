{% include 'common.header.twig.c' %}

/*=======================================================================*/
/* The main C source file for extension {{ name }} */
/*=======================================================================*/

#include "extgen.includes.h"
#include "extgen.compatibility.h"
#include "extgen.definitions.h"

#define PHP_{{ uname }}_VERSION "{{ version }}"
#define PHP_{{ uname }}_EXTNAME "{{ name }}"

extern zend_module_entry {{ name }}_module_entry;
#define phpext_{{ name }}_ptr &{{ name }}_module_entry

/*---------------------------------------------------------------*/
/*--- Header code */

{{ global_data.user_header }}

/*---------------------------------------------------------------*/
/*--- Module global data */

ZEND_BEGIN_MODULE_GLOBALS({{ name }})
#ifdef HAVE_GETTIMEOFDAY
struct timeval _eg_base_tp;
#endif

/* Ini variables */

{% for ini in ini_settings %}
	{{ ini.vartype }} {{ ini.name }};
{% endfor %}

/* Module variables */

{% block user_module_globals_data %}{% endblock %}

ZEND_END_MODULE_GLOBALS({{ name }})

/*---------------------------------------------------------------*/

#ifdef ZTS
#	define {{ uname }}_G(v) TSRMG({{ name }}_globals_id, zend_{{ name }}_globals *, v)
#else
#	define {{ uname }}_G(v) ({{ name }}_globals.v)
#endif

ZEND_DECLARE_MODULE_GLOBALS({{ name }})

#ifdef COMPILE_DL_{{ uname }}
	ZEND_GET_MODULE({{ name }})
#endif

/*---------------------------------------------------------------*/
/* Module global init */
/* Initialize a new zend_{{ name }}_globals struct during thread spin-up */
{# Default: clear memory block #}

static void {{ name }}_globals_ctor(zend_{{ name }}_globals * globals TSRMLS_DC)
{
EG_CLEAR_DATA(*globals); /* Init everything to 0/NULL */

{% block user_module_globals_init %}{% endblock %}
}

/*------------------------*/
/* Module global destructor */
/* Any resources allocated during initialization may be freed here */

#ifndef ZTS
static void {{ name }}_globals_dtor(zend_{{ name }}_globals * globals TSRMLS_DC)
{
{% block user_module_globals_dtor %}{% endblock %}
}
#endif

/*---------------------------------------------------------------*/
/* Fixed values */

{% include 'main.fixed_values.twig.c' %}

/*---------------------------------------------------------------*/
/* extgen functions */

#include "extgen.lib.c"

/*---------------------------------------------------------------*/
/* Ini settings */

PHP_INI_BEGIN()

{% for ini in ini_settings %}
	{{ ini.std_macro }}("{{ name }}.{{ ini.name }}", "{{ ini.default }}",
		{{ ini.access }}, {{ ini.modify }}, {{ ini.name }}, zend_{{ name }}_globals,
		{{ name }}_globals)
{% endfor %}

PHP_INI_END()

/*---------------------------------------------------------------*/
/* Resource code (struct + destructor) */

{% for resource in resources %}
#include "{{ resource.dest_filename }}"
{% endfor %}

/*---------------------------------------------------------------*/
/*--- Header code */

{{ global_data.user_code }}

/*---------------------------------------------------------------*/
/* Extension functions */

{% for fname,function in functions %}
#include "{{ function.dest_filename }}"
{% endfor %}

/*---------------------------------------------------------------*/
/* PHP_Mxxx functions */

/*-------------------*/
/* phpinfo() output  */

static PHP_MINFO_FUNCTION({{ name }})
{
{% include 'common.minfo.twig.c' %}

{% if flags.minfo_displays_ini %}
	DISPLAY_INI_ENTRIES();
{% endif %}

{% block user_minfo %}{% endblock %}
}

/*---------------------------------------------------------------*/

static PHP_RINIT_FUNCTION({{ name }})
{
_EG_DBG_INIT();

{% block user_rinit %}{% endblock %}

return SUCCESS;
}

/*---------------------------------------------------------------*/

static PHP_RSHUTDOWN_FUNCTION({{ name }})
{
{% block user_rshutdown %}{% endblock %}

return SUCCESS;
}

/*---------------------------------------------------------------*/

static PHP_MINIT_FUNCTION({{ name }})
{
{% block user_minit_start %}{% endblock %}

_eg_build_{{ name }}_constant_values();

/* Init module globals */

ZEND_INIT_MODULE_GLOBALS({{ name }}, {{ name }}_globals_ctor, NULL);

/* Register ini entries */

REGISTER_INI_ENTRIES();

/* Declare constants */

{% for cname,const in constants %}
	{% if (const.type=='null') %}
		_EG_REGISTER_NULL_CONSTANT("{{ cname }}");
	{% else %}
		_EG_REGISTER_{{ const.type|upper }}_CONSTANT("{{ cname }}", {{ const.value }});
	{% endif %}
{% endfor %}

/*--------*/
/* Declare resource types */

{% for resource in resources %}
	EG_RESOURCE_TYPE({{ resource.name }})=(eg_restype)zend_register_list_destructors_ex(
		_eg_resource_dtor_{{ resource.name }}_ext_np,
		_eg_resource_dtor_{{ resource.name }}_ext_p,
		"{{ resource.display_string }}",
		module_number);
{% endfor %}

/*--------*/

{% block user_minit%}{% endblock %}
{% block user_minit_end %}{% endblock %}

return SUCCESS;
}

/*---------------------------------------------------------------*/

static PHP_MSHUTDOWN_FUNCTION({{ name }})
{
{% block user_mshutdown_pre %}{% endblock %}
{% block user_mshutdown %}{% endblock %}

/* Unregister ini entries */

UNREGISTER_INI_ENTRIES();

/* Free global data */

#ifndef ZTS
{{ name }}_globals_dtor(&{{ name }}_globals TSRMLS_CC);
#endif

{% block user_mshutdown_post %}{% endblock %}

return SUCCESS;
}

/*---------------------------------------------------------------*/
/*-- Module definition --*/

{% include 'main.function_declarations.twig.c' %}

{% include 'main.module_definition.twig.c' %}

/*====================================================================*/