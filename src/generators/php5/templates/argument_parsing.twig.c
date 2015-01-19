
{% if func.arguments|length != 0 %}
{
zval *zp;
_EG_FUNC_ARGUMENT *ip;

/*---- Zend-parse arguments ------------------*/

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "{{ func.parse_format }}"
	{% for arg in func.arguments %}, &({{ arg.name }}_es.zp){% endfor %}
	) == FAILURE) return;

/*-- Process arguments */
/* Do nothing on bare zvals */

{% for arg in func.arguments if (arg.type!="zval") %}
zp={{ arg.name }}_es.zp;
ip=&({{ arg.name }}_es.i);

_EG_FUNC_TYPE_INIT(ip);
if ({{ loop.index }} > ZEND_NUM_ARGS())
	{
	/*--- Arg was not set */
	ip->is_unset=EG_TRUE;
	ip->_writable=EG_FALSE;

	/* Set default value if one was given */

	{% if arg.default is not null %}
		{% if (arg.scalar_type=='string') %}
			{ /* Use tmp string because formula must run once only */
			eg_str_val tmp_string={{ arg.default }};
			_EG_FUNC_TYPE_STRING(ip,tmp_string,1);
			}
		{% else %}
			{
			_EG_FUNC_TYPE_{{ arg.scalar_type|upper }}(ip,({{ arg.default }}));
			}
		{% endif %}
	{% endif %} {# Arg defines default #}
	}
else
	{
	 /*--- Arg was set */
	ip->is_unset=EG_FALSE;
	ip->_writable={{ (arg.byref ? "EG_TRUE" : "EG_FALSE") }};
{% if arg.nullok %}
	if (zp)
		{
{% endif %}
		{% if (arg.accept_array) %}
			if ((EG_Z_TYPE_P(zp)==EG_IS_ARRAY)||{{ (arg.type=='array' ? 1 : 0) }})
				{ /* Write array */
				EG_ZVAL_ENSURE_ARRAY(zp);
				_EG_FUNC_TYPE_ARRAY(ip,EG_Z_ARRVAL_P(zp),0); /* ? dup=0/1 a voir ? */
				}
			else
				{ /* mixed receiving scalar */
				_eg_convert_arg_zp_to_scalar(EG_IS_{{ arg.scalar_type|upper }},zp,ip);
				}
		{% else %} {# Array not accepted -> scalar only #}
			_eg_convert_arg_zp_to_scalar(EG_IS_{{ arg.scalar_type|upper }},zp,ip);
		{% endif %} {# accept_array #}
{% if arg.nullok %}
		}
	else
		{
		_EG_FUNC_TYPE_SET_TYPE(ip,EG_IS_{{ arg.nulltype|upper }});
		}
{% endif %}
	}
{% endfor %} {# foreach arg if arg.type != zval #}
}
{% endif %} {# if count(args) != 0 #}

/*----------------------------------------------------------------------*/
