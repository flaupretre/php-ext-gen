{% include 'common/header.twig.c' %}

/*=======================================================================*/
/* The main C source file for extension {{ name }} */
/*=======================================================================*/

/*------------------------*/

{% if flags.debug %}
#define EXTGEN_DEBUG
{% endif %}

/*------------------------*/

{% include 'includes.twig.c' %}

{% include 'utils.twig.h' %}

#define PHP_{{ upper_name }}_VERSION "{{ version }}"

#define EXTGEN_EXTNAME "{{ name }}"

zend_module_entry {{ name }}_module_entry;

#define phpext_{{ name }}_ptr &{{ name }}_module_entry

{# TODO Declare CZVALs #}
{# TODO Declare HKEYs #}

ZEND_BEGIN_MODULE_GLOBALS({{ name }})

{{ global_data.module_globals_code }}

ZEND_END_MODULE_GLOBALS({{ name }})

#ifdef ZTS
#	define {{ upper_name }}_G(v) TSRMG({{ name }}_globals_id, zend_{{ name }}_globals *, v)
#else
#	define {{ upper_name }}_G(v) ({{ name }}_globals.v)
#endif

ZEND_DECLARE_MODULE_GLOBALS({{ name }})

#ifdef COMPILE_DL_{{ upper_name }}
	ZEND_GET_MODULE({{ name }})
#endif

/*---------------------------------------------------------------*/
/* phpinfo() output                                              */

static PHP_MINFO_FUNCTION({{ name }})
{
php_info_print_table_start();

php_info_print_table_row(2, "This extension was generated by","php-ext-gen");
php_info_print_table_row(2, "Generator version","{{ software.version }}");
php_info_print_table_row(2, "Generation Date","{{ date()|date("Y/m/d") }}");
php_info_print_table_row(2, "Target PHP engine","{{ engine.name }}");
php_info_print_table_row(2, "Target version","{{ engine.version }}");

php_info_print_table_end();

{{ global_data.minfo_code }}
}

/*---------------------------------------------------------------*/
/* Initialize a new zend_{{ name }}_globals struct during thread spin-up */

static void {{ name }}_globals_ctor(zend_{{ name }}_globals * globals TSRMLS_DC)
{
{% if global_data.module_globals_init_code != "" %}
{{ global_data.module_globals_init_code }}
{% else %}
CLEAR_DATA(*globals); /* Init everything to 0/NULL */
{% endif %}
}

/*------------------------*/
/* Any resources allocated during initialization may be freed here */

#ifndef ZTS
static void {{ name }}_globals_dtor(zend_{{ name }}_globals * globals TSRMLS_DC)
{
{{ global_data.module_globals_dtor_code }}
}
#endif

/*---------------------------------------------------------------*/

static void build_constant_values()
{
{# TODO #}
}

/*---------------------------------------------------------------*/

static PHP_RINIT_FUNCTION({{ name }})
{
DBG_INIT();

{{ global_data.rinit_code }}

return SUCCESS;
}

/*---------------------------------------------------------------*/

static PHP_RSHUTDOWN_FUNCTION({{ name }})
{
{{ global_data.rshutdown_code }}

return SUCCESS;
}

/*---------------------------------------------------------------*/

static PHP_MINIT_FUNCTION({{ name }})
{
/* build_constant_values(); */

{{ global_data.minit_pre_code }}

ZEND_INIT_MODULE_GLOBALS({{ name }}, {{ name }}_globals_ctor, NULL);

{{ global_data.minit_post_code }}

return SUCCESS;
}

/*---------------------------------------------------------------*/

static PHP_MSHUTDOWN_FUNCTION({{ name }})
{
{{ global_data.mshutdown_pre_code }}

#ifndef ZTS
	{{ name }}_globals_dtor(&{{ name }}_globals TSRMLS_CC);
#endif

{{ global_data.mshutdown_post_code }}

return SUCCESS;
}

/*---------------------------------------------------------------*/
/*-- Module definition --*/

zend_module_entry {{ name }}_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	PHP_{{ upper_name }}_EXTNAME,
	NULL,
	PHP_MINIT({{ name }}),
	PHP_MSHUTDOWN({{ name }}),
	PHP_RINIT({{ name }}),
	PHP_RSHUTDOWN({{ name }}),
	PHP_MINFO({{ name }}),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_{{ upper_name }}_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};

/*---------------------------------------------------------------*/
/*--- Header code */

{{ global_data.header }}

/*-- Functions -------*/

{% for fname,function in functions %}
	{% include 'function.twig.c' %}
{% endfor %}

/* Trailer code */

{{ global_data.trailer }}

/*====================================================================*/