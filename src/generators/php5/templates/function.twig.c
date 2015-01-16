/* Defines function {{ func.name }} */

{% block pre %}
{% endblock pre %}

/*---------------------------------------------------------------------*/
/* Internal function */

static struct eg_retval_struct *extgen_func_{{ func.name }}
	({{ func.call_arg_list }})
{% for argname,arg in func.arguments %}
{% for var in arg.vars %}
{{ var.type}} {{argname}}{{ var.ext }};
{% endfor %}
{% endfor %}
{	/*---- Function body */
struct eg_retval_struct eg_ret;
_EG_INIT_RETVAL(eg_ret);

{ /* Enclose in braces because of argument declarations */
{% block body %}
{% endblock body %}
}

EG_RETURN();
}

/*---------------------------------------------------------------------*/
/* PHP_FUNCTION, used as a wrapper around internal function */

PHP_FUNCTION({{ func.name }})
{
{% for argname,arg in func.arguments %}
{% for var in arg.vars %}
{{ var.type}} {{argname}}{{ var.ext }}=({{ var.type}})0;
{% endfor %}
{% endfor %}
struct eg_retval_struct *retp;

/*---- Parse arguments ------------------*/

{% block argument_parsing %}

if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "{{ func.parse_format }}"
	{{ func.parse_arguments }}) == FAILURE) return;
/*TODO*/
{% endblock argument_parsing %}

{% block pre_call_function %}
{% endblock pre_call_function %}

/*---- Call internal function */
{ /* Enclose in braces because of argument declarations */
retp=extgen_func_{{ func.name }}({{ func.call_arg_list }});
}

{% block post_call_function %}
{% endblock post_call_function %}

/*---- Compute return values */

{% block compute_return_values %}
/* TODO */
{% endblock compute_return_values %}
}
