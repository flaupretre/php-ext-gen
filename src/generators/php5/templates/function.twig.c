/* Defines function {{ func.name }} */

{% block user_header %}{% endblock %}

/*---------------------------------------------------------------------*/
/* Internal function */

static void extgen_func_{{ func.name }}(
_EG_FUNC_RETVAL *_eg_retval
, int return_value_used
, int _eg_num_set_args
{% for arg in func.arguments %}
	, {{ (arg.type=="zval") ? "zval" : "_EG_FUNC_ARGUMENT" }} *{{ arg.name }}
{% endfor %} TSRMLS_DC)
{	/*---- Function body */

{ /* Enclose in braces because of argument declarations */
{% block user_body %}{% endblock %}
} {# Here, we must return immediately as return can be done from inside body #}
}

/*---------------------------------------------------------------------*/
/* PHP_FUNCTION, used as a wrapper around internal function */

PHP_FUNCTION({{ func.name }})
{
_EG_FUNC_RETVAL _eg_retval_s; /* Return value */
{% for arg in func.arguments %}
_EG_FUNC_EXTERNAL_ARGUMENT {{ arg.name }}_es;
{% endfor %}
zval **zpp;
_EG_FUNC_ARGUMENT *ip;
{% block user_external_func_declarations %}{% endblock %}

/* Init extgen local variables */

_EG_FUNC_RETVAL_INIT(&_eg_retval_s);

/* Parse arguments */

{# This block allows user code to compute context-dependant default values #}
{% block user_pre_argument_parsing %}{% endblock %}
{% include 'function.argument_parsing.twig.c' %}
{% block user_post_argument_parsing %}{% endblock %}

	/*---- Call internal function */
	{ /* Enclose in braces because of possible argument declarations */

	{% block user_external_pre_call %}{% endblock %}

		{ /* Enclose in braces because of possible argument declarations */
		extgen_func_{{ func.name }}(
			  &_eg_retval_s
			, return_value_used
			, (int)ZEND_NUM_ARGS()
			{% for arg in func.arguments %}
				{% if (arg.type=="zval") %}
					, {{ arg.name }}_es.zp
				{% else %}
					, &({{ arg.name }}_es.i)
				{% endif %}
			{% endfor %} TSRMLS_CC);
		}

	{% block user_external_post_call %}{% endblock %}
	}

{% block compute_return_values %}
{% include 'function.compute_return_values.twig.c' %}
{% endblock compute_return_values %}

{% block user_external_end %}{% endblock %}
}