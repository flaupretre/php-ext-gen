{% include 'common_header.twig.c' %}

/*=======================================================================*/
/* The main C source file for extension {{ name }} */
/*=======================================================================*/

/*------------------------*/

{% if options.debug %}
#define EXTGEN_DEBUG
{% endif %}

/*------------------------*/

{% include 'includes.twig.c' %}

{% include 'utils.twig.h' %}

#define PHP_{{ uname }}_VERSION "{{ version }}"

#define PHP_{{ uname }}_EXTNAME "{{ name }}"

zend_module_entry {{ name }}_module_entry;

#define phpext_{{ name }}_ptr &{{ name }}_module_entry

{# TODO Declare CZVALs #}
{# TODO Declare HKEYs #}

/*--- Module global data */

ZEND_BEGIN_MODULE_GLOBALS({{ name }})

{{ global_data.module_globals_code }}

ZEND_END_MODULE_GLOBALS({{ name }})

/*---*/

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
/* Module global init
/* Initialize a new zend_{{ name }}_globals struct during thread spin-up */
{# Default: clear memory block #}

static void {{ name }}_globals_ctor(zend_{{ name }}_globals * globals TSRMLS_DC)
{
{% if global_data.module_globals_init_code != "" %}
{{ global_data.module_globals_init_code }}
{% else %}
CLEAR_DATA(*globals); /* Init everything to 0/NULL */
{% endif %}
}

/*------------------------*/
/* Module global destructor */
/* Any resources allocated during initialization may be freed here */

#ifndef ZTS
static void {{ name }}_globals_dtor(zend_{{ name }}_globals * globals TSRMLS_DC)
{
{{ global_data.module_globals_dtor_code }}
}
#endif

/*---------------------------------------------------------------*/
/* PHP_Mxxx functions */

{% include 'php_mfunctions.twig.c' %}

/*---------------------------------------------------------------*/

static void build_constant_values()
{
{# TODO #}
}

/*---------------------------------------------------------------*/
/*-- Module definition --*/

zend_module_entry {{ name }}_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	PHP_{{ uname }}_EXTNAME,
	NULL,
	PHP_MINIT({{ name }}),
	PHP_MSHUTDOWN({{ name }}),
	PHP_RINIT({{ name }}),
	PHP_RSHUTDOWN({{ name }}),
	PHP_MINFO({{ name }}),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_{{ uname }}_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};

/*---------------------------------------------------------------*/
/*--- Header code */

{{ global_data.header }}

/*-- Functions -------*/

{% for fname,function in functions %}
#include "{{ function.dest_filename }}"
{% endfor %}

/* Trailer code */

{{ global_data.trailer }}

/*====================================================================*/