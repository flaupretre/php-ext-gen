
/* Compute return value */

_EG_VARS_TO_ZVAL(return_value_ptr,&_eg_retval_s);

/* Compute passed-by-ref returned values */

{% if func.arguments|length != 0 %}

{% for argname,arg in func.arguments if (arg.type!="zval") %}
	{% if (arg.byref) %}
		ip=&({{ argname }}_es.i);
		/* If arg not set or not written back, don't write to zval */
		if ((!(ip->is_unset)) && (ip->_written))
			{
			zpp=&({{ argname }}_es.zp);
			_EG_VARS_TO_ZVAL(zpp,ip); /* this macro frees previous zval data if any */
			}
		else
			{
			_EG_VARS_RESET(ip); /* Free argument data */
			}
	{% else %}
		_EG_VARS_RESET(&({{ argname }}_es.i)); /* Free argument data */
	{% endif %}
{% endfor %} {# foreach arg if arg.type != zval #}
{% endif %} {# if count(args) != 0 #}
