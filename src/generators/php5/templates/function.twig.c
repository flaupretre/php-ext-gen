/* Defines function {{ func.name }} */

{% block user_header %}
{% endblock user_header %}

/*---------------------------------------------------------------------*/
/* Internal function */

static void extgen_func_{{ func.name }}(
struct _eg_func_retval_struct *eg_ret
, int _eg_num_set_args
{% for argname in func.arguments|keys %}
, _eg_func_argument_struct *{{ argname }}
{% endfor %} )
{	/*---- Function body */

{ /* Enclose in braces because of argument declarations */
{% block user_body %}
{% endblock user_body %}
} /* Here, we must return immediately as return can be done from inside body %}
}

/*---------------------------------------------------------------------*/
/* PHP_FUNCTION, used as a wrapper around internal function */

PHP_FUNCTION({{ func.name }})
{
struct eg_retval_struct eg_ret_s;
int _eg_num_set_args;
{% for argname in func.arguments|keys %}
_eg_func_argument_struct {{ argname }}_s;
{% endfor %} )
{% block user_external_func_declarations %}
{% endblock user_external_func_declarations %}

/* Init extgen local variables */

_eg_num_set_args=ZEND_NUM_ARGS();
_EG_FUNC_TYPE_INIT(&eg_ret_s);
{% for argname in func.arguments|keys %}
{{ argname }}_s.is_unset=EG_FALSE;
{{ argname }}_s.is_null=EG_FALSE;
{{ argname }}_s.outzp=(zval *)0;
_EG_FUNC_TYPE_INIT(&{{ argname }}_s);
{% endfor %} )
{% block user_external_post_init %}
{% endblock user_external_post_init %}

/* Parse arguments */

{% include 'argument_parsing.twig.c' %}

	{ /* Enclose in braces because of possible argument declarations */

	/*---- Call internal function */

	{% block user_external_pre_call %}
	{% endblock user_external_pre_call %}

		{ /* Enclose in braces because of possible argument declarations */
		extgen_func_{{ func.name }}(
			  &eg_ret_s
			, _eg_num_set_args
			{% for argname in func.arguments|keys %}, &{{ argname }}_s{% endfor %} )
		}

	{% block user_external_post_call %}
	{% endblock user_external_post_call %}
	}

{% block compute_return_values %}
{% include 'compute_return_values.twig.c' %}
{% endblock compute_return_values %}

{% block user_external_end %}
{% endblock user_external_end %}
}