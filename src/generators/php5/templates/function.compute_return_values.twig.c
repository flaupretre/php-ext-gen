
/* Compute return value */

_EG_FUNC_TYPE_TO_ZVAL(return_value_ptr,&_eg_retval_s);

/* Compute passed-by-ref returned values */

{% if func.arguments|length != 0 %}

{% for argname,arg in func.arguments if (arg.type!="zval") %}
	ip=&({{ argname }}_es.i);
	/* If arg not set or not written back, don't write to zval */
	if ({{ (arg.byref ? 1 : 0) }} && (!{{ argname }}_es.i.is_unset)
		&& ({{ argname }}_es.i._written))
		{
		zpp=&({{ argname }}_es.zp);
		_EG_FUNC_TYPE_TO_ZVAL(zpp,ip); /* this macro frees previous zval data if any */
		}
	else
		{
		_EG_FUNC_TYPE_RESET(ip); /* Free argument data */
		}
{% endfor %} {# foreach arg if arg.type != zval #}
{% endif %} {# if count(args) != 0 #}
