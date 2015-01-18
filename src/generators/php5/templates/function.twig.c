/* Defines function {{ func.name }} */

{% block user_header %}{% endblock %}

/*---------------------------------------------------------------------*/
/* Internal function */

static void extgen_func_{{ func.name }}(
_EG_FUNC_RETVAL *eg_ret
, int _eg_num_set_args
{% for argname,arg in func.arguments %}
	, {{ (arg.type=="zval") ? "zval" : "_EG_FUNC_ARGUMENT" }} *{{ argname }}
{% endfor %} )
{	/*---- Function body */

{ /* Enclose in braces because of argument declarations */
{% block user_body %}{% endblock %}
} {# Here, we must return immediately as return can be done from inside body #}
}

/*---------------------------------------------------------------------*/
/* PHP_FUNCTION, used as a wrapper around internal function */

PHP_FUNCTION({{ func.name }})
{
_EG_FUNC_RETVAL eg_ret_s; /* Return value */
{% for argname in func.arguments|keys %}
_EG_FUNC_EXTERNAL_ARGUMENT {{ argname }}_es;
{% endfor %}
{% block user_external_func_declarations %}{% endblock %}

/* Init extgen local variables */

_EG_FUNC_TYPE_INIT(&eg_ret_s);

{% block user_external_post_init %}{% endblock %}

/* Parse arguments */

{% include 'argument_parsing.twig.c' %}

	/*---- Call internal function */
	{ /* Enclose in braces because of possible argument declarations */

	{% block user_external_pre_call %}{% endblock %}

		{ /* Enclose in braces because of possible argument declarations */
		extgen_func_{{ func.name }}(
			  &eg_ret_s
			, (int)ZEND_NUM_ARGS()
			{% for argname,arg in func.arguments %}
				{% if (arg.type=="zval") %}
					, {{ argname }}_es.zp
				{% else %}
					, &({{ argname }}_es.i)
				{% endif %}
			{% endfor %} );
		}

	{% block user_external_post_call %}{% endblock %}
	}

{% block compute_return_values %}
{% include 'compute_return_values.twig.c' %}
{% endblock compute_return_values %}

{% block user_external_end %}{% endblock %}
}