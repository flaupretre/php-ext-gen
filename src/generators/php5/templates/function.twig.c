{% block pre %}
{% endblock pre %}

/* Defines function {{ func.name }} */

PHP_FUNCTION({{ func.name }})
{
/*---- Variable declaration -------------*/

{% block declarations %}

{% for argname,arg in func.arguments %}
{% for var in arg.vars %}
{{ var.type}} {{argname}}{{ var.ext }};
{% endfor %}

{% endfor %}

char *retval;/*TODO*/
long retval_len;

{% endblock declarations %}

/*---- Parse arguments ------------------*/

{% block argument_parsing %}

if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "{{ func.parse_format }}"
	{{ func.parse_arguments }}) == FAILURE) return;
/*TODO*/
{% endblock argument_parsing %}

/*---- Function body */
{ /* Enclose in braces because of argument declarations */

{% block body %}
{% endblock body %}

}

/*---- Compute return values */

{% block return_values %}
/* TODO */
{% endblock return_values %}
}
