{% block argument_parsing %}

{# Allow user code to compute context-dependant default values #}
{% block user_pre_argument_parsing %}{% endblock %}

{% if func.arguments|length != 0 %}
{
zval *zp;
_EG_FUNC_ARGUMENT *ip;

/*---- Zend-parse arguments ------------------*/

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "{{ func.parse_format }}"
	{% for argname in func.arguments|keys %}, &({{ argname }}_es.zp){% endfor %}
	) == FAILURE) return;

/*-- Process arguments */
/* Do nothing on bare zvals */

{% for argname,arg in func.arguments if (arg.type!="zval") %}
zp={{ argname }}_es.zp;
ip=&({{ argname }}_es.i);

_EG_FUNC_TYPE_INIT(ip);
if ({{ loop.index }} > ZEND_NUM_ARGS())
	{
	/*--- Arg was not set */
	ip->is_unset=EG_TRUE;
	ip->_writable=EG_FALSE;

	/* Set default value if one was given */

	{% if arg.default is not null %}
		{% if (arg.scalar_type=='string') %}
			_EG_FUNC_TYPE_{{ arg.default|upper }}(ip,({{ arg.default }}));
		{% else %}
			{ /* Use tmp string because formula must run once only */
			eg_string tmp_string={{ arg.default }}
			_EG_FUNC_TYPE_STRING(ip,tmp_string,1);
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
				_EG_FUNC_TYPE_ARRAY(ip,EG_Z_ARRVAL_P(zp),0 /* ? dup=0/1 a voir ? */)
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
		_EG_FUNC_TYPE_SET_TYPE(ip,EG_{{ arg.nulltype|upper }});
		}
{% endif %}
	}
{% endfor %} {# foreach arg if arg.type != zval #}
}
{% endif %} {# if count(args) != 0 #}

/*----------------------------------------------------------------------*/

{% block user_post_argument_parsing %}{% endblock %}

{% endblock argument_parsing %}
