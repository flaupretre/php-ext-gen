/*---------------------------------------------------------------*/
/* phpinfo() output                                              */

static PHP_MINFO_FUNCTION({{ name }})
{
{% include 'common_minfo_code.twig.c' %}

{{ global_data.minfo_code }}
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

