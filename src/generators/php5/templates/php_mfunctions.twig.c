/*---------------------------------------------------------------*/
/* phpinfo() output                                              */

static PHP_MINFO_FUNCTION({{ name }})
{
{% include 'common_minfo_code.twig.c' %}

{% block user_minfo %}{% endblock %}
}

/*---------------------------------------------------------------*/

static PHP_RINIT_FUNCTION({{ name }})
{
EG_DBG_INIT();

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

{% block user_minit_pre_init_globals %}{% endblock %}
ZEND_INIT_MODULE_GLOBALS({{ name }}, {{ name }}_globals_ctor, NULL);
{% block user_minit_post_init_globals %}{% endblock %}

/* Declare constants */

{% block user_minit_pre_constant_declare %}{% endblock %}

{% for cname,const in constants %}
	{% if (const.type=='null') %}
		_EG_REGISTER_NULL_CONSTANT("{{ cname }}");
	{% else %}
		_EG_REGISTER_{{ const.type|upper }}_CONSTANT("{{ cname }}", {{ const.value }});
	{% endif %}
{% endfor %}

{% block user_minit_post_constant_declare %}{% endblock %}

/*--------*/

ZEND_INIT_MODULE_GLOBALS({{ name }}, {{ name }}_globals_ctor, NULL);

{% block user_minit_end %}{% endblock %}

return SUCCESS;
}

/*---------------------------------------------------------------*/

static PHP_MSHUTDOWN_FUNCTION({{ name }})
{
{% block user_mshutdown_pre %}{% endblock %}

#ifndef ZTS
	{{ name }}_globals_dtor(&{{ name }}_globals TSRMLS_CC);
#endif

{% block user_mshutdown_post %}{% endblock %}

return SUCCESS;
}
