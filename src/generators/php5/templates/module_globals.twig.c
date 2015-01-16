/*--- Module global data */

ZEND_BEGIN_MODULE_GLOBALS({{ name }})
#ifdef HAVE_GETTIMEOFDAY
struct timeval _eg_base_tp;
#endif

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
/* Module global init */
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

