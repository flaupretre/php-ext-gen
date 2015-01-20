
/* Compute return value */

_EG_FUNC_TYPE_TO_ZVAL(&return_value,&_eg_retval_s);

/* Compute passed-by-ref returned values */

{% if func.arguments|length != 0 %}

{% for argname,arg in func.arguments if (arg.type!="zval") %}
	{% if (arg.byref) %}
	/* If arg was not written back, do nothing */
	if ({{ argname }}_es.i._written)
		{
		zpp=&({{ argname }}_es.zp);
		ip=&({{ argname }}_es.i);
		_EG_FUNC_TYPE_TO_ZVAL(zpp,ip); /* this macro frees previous data if any */
		}
	{% endif %}
{% endfor %} {# foreach arg if arg.type != zval #}
{% endif %} {# if count(args) != 0 #}
