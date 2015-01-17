{% block argument_parsing %}

{% block user_pre_argument_parsing %}
{# Allows user code to compute context-dependant default values #}
{% endblock user_pre_argument_parsing %}

/*---- Zend-parse arguments ------------------*/

if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "{{ func.parse_format }}"
	{{ func.parse_arguments }}) == FAILURE) return;

{% for argname,arg in func.arguments %}


{{ argname }}_s.type=EG_Z_TYPE(



{% set arg_zval = argname ~ '_zval' }}

{% if (arg.nullok) %}
	if (EG_ZVAL_IS_NULL({{ arg_zval }}) {{ argname }}_is_null=1;
	else {
{% else %}
{% endif %}
/* Here, we have eliminated the case (nullok and zval is null) */
/* Convert */

{% if arg.accept_array





{% if (arg.nullok) %}
	}
{% endif %}



{% endfor %}

/*----------------------------------------------------------------------*/

{% block user_post_argument_parsing %}
{% endblock user_post_argument_parsing %}

{% endblock argument_parsing %}
