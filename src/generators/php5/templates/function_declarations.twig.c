/* {{ name }} exposed functions
 *
 * every user-visible function must have an entry here
 */

/*----- Arginfo ---------*/

{%  for func in functions %}
ZEND_BEGIN_ARG_INFO_EX(arginfo_func_{{ func.name }}, 0, 0, {{ func.required_args_count }})
{% for argname,arg in func.arguments %}
	{% set byref=(arg.ref ? '1' : '0') %}
	{% if arg.mixed %}
		ZEND_ARG_INFO({{ byref }}, {{ argname }})
	{% else %}
		{% if (arg.type=='array') %}
			ZEND_ARG_ARRAY_INFO({{ byref }}, {{ argname }}, {{ nullok }})
		{% else %}
			ZEND_ARG_TYPE_INFO({{ byref }}, {{ argname }}, {{ arg.zval_type }}, {{ byref }})
		{% endif %}
	{% endif %}
{% endfor %}
ZEND_END_ARG_INFO()
{% endfor %}

/*------ Functions --------*/

static zend_function_entry extgen_functions[] = {
{%  for func in functions %}
    PHP_FE({{ func.name }}, arginfo_func_{{ func.name }})


{% endfor %}
    {NULL, NULL, NULL}  /* must be the last line */
};
