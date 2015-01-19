
{% block user_module_globals_data %}
	zend_bool enabled;
{% endblock %}

{# ------------- Module global data init ------------ #}

{% block user_module_globals_init %}
	SCREAM_G(enabled) = 1;
{% endblock %}

{# ------------- minit ------------ #}

{% block user_minit_end %}
	REGISTER_INI_ENTRIES();

	if((zend_set_user_opcode_handler(ZEND_BEGIN_SILENCE, 
			scream_op_ZEND_SILENCE) == FAILURE) ||
		(zend_set_user_opcode_handler(ZEND_END_SILENCE, 
			scream_op_ZEND_SILENCE) == FAILURE))
		{
			zend_error(E_NOTICE, "cannot install silence opcode override");
			SCREAM_G(enabled) = 0;
		}
{% endblock %}

{# ------------- mshutdown ------------ #}

{% block user_mshutdown_pre %}
	UNREGISTER_INI_ENTRIES();
{% endblock %}

{# ------------- minfo ------------ #}

{% block user_minfo %}
	php_info_print_table_start();
	php_info_print_table_header(2, "Scream status", (SCREAM_G(enabled) ? "On" : "Off"));
	php_info_print_table_end();
{% endblock %}







