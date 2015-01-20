{# ------------- minit ------------ #}

{% block user_minit %}

	if((zend_set_user_opcode_handler(ZEND_BEGIN_SILENCE, 
			scream_op_ZEND_SILENCE) == FAILURE) ||
		(zend_set_user_opcode_handler(ZEND_END_SILENCE, 
			scream_op_ZEND_SILENCE) == FAILURE))
		{
			zend_error(E_NOTICE, "cannot install silence opcode override");
			SCREAM_G(enabled) = 0;
		}

{% endblock %}
