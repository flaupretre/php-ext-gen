
{% if func.arguments|length != 0 %}

/*---- Zend-parse arguments ------------------*/

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "{{ func.parse_format }}"
	{% for arg in func.arguments %}, &({{ arg.name }}_es.zp){% endfor %}
	) == FAILURE) return;

/*-- Process arguments */
/* Do nothing on bare zvals */

{% for arg in func.arguments if (arg.type!="zval") %}
zpp=&({{ arg.name }}_es.zp);
ip=&({{ arg.name }}_es.i);

_EG_VARS_INIT(ip);
ip->_written=0;
if ({{ loop.index }} > ZEND_NUM_ARGS())
	{
	/*--- Arg was not set */
	ip->is_unset=EG_TRUE;

	/* Set default value if one was given */

	{% if arg.default is not null %}
		ip->_writable=EG_FALSE;
		{% if (arg.scalar_type=='string') %}
			{ /* Use tmp string because formula must run once only */
			eg_string tmp_string={{ arg.default }};
			_EG_VARS_STRING(ip,tmp_string,1);
			}
		{% else %}
			{
			_EG_VARS_{{ arg.scalar_type|upper }}(ip,({{ arg.default }}));
			}
		{% endif %}
	{% else %} /* As we have no default, arg can be written by user code */
		ip->_writable=EG_TRUE;
	{% endif %} {# Arg defines default #}
	}
else /*--- Arg was set */
	{
	ip->is_unset=EG_FALSE;
	ip->_writable={{ (arg.byref ? "EG_TRUE" : "EG_FALSE") }};
{% if arg.nullok %}
	if (*zpp)
		{
{% endif %}
		{% if (arg.accept_array) %}
			if ((EG_Z_TYPE_PP(zpp)==EG_IS_ARRAY)||{{ (arg.type=='array' ? 1 : 0) }})
				{ /* Write array */
				EG_ZVAL_ENSURE_ARRAY(zpp);
				_EG_VARS_ARRAY(ip,EG_Z_ARRVAL_PP(zpp),0);
				}
			else
				{ /* mixed receiving scalar */
				_eg_convert_arg_zpp_to_scalar(EG_IS_{{ arg.scalar_type|upper }},zpp,ip);
				}
		{% else %} {# Array not accepted -> scalar only #}
			_eg_convert_arg_zpp_to_scalar(EG_IS_{{ arg.scalar_type|upper }},zpp,ip);
		{% endif %} {# accept_array #}
		{% if (arg.accept_resource and (arg.resource_type is not null)) %}
			/* Check resource type against defined resource_type */
			if (EG_TYPE(ip)==EG_IS_RESOURCE) {
				if (EG_RES_TYPE(ip) != EG_RESOURCE_TYPE({{ arg.resource_type }})) {
					/* Invalidate resource */
					EG_RESOURCE(ip)=(eg_resource)0;
					EG_RES_TYPE(ip)=-1;
					EG_RES_PTR(ip)=NULL;
					}
				}
		{% endif %} {# Resource type check #}
{% if arg.nullok %}
		}
	else
		{
		_EG_VARS_SET_TYPE(ip,EG_IS_{{ arg.nulltype|upper }});
		}
{% endif %}
	}
{% endfor %} {# foreach arg if arg.type != zval #}
{% endif %} {# if count(args) != 0 #}

/*----------------------------------------------------------------------*/
