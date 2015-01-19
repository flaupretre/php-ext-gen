
/* Compute return value */

_EG_FUNC_TYPE_TO_ZVAL(return_value,&_eg_retval_s);

/* Compute passed-by-ref returned values */

{% if func.arguments|length != 0 %}
{
zval *zp;
_EG_FUNC_ARGUMENT *ip;

{% for argname,arg in func.arguments if (arg.type!="zval") %}
	{% if (arg.byref) %}
		zp={{ argname }}_es.zp;
		ip=&({{ argname }}_es.i);
		EG_ZVAL_NULL(zp); /* Free previous data */
		_EG_FUNC_TYPE_TO_ZVAL(zp,ip);
	{% endif %}
{% endfor %} {# foreach arg if arg.type != zval #}
}
{% endif %} {# if count(args) != 0 #}
