
/* zend hooks */

static int scream_op_ZEND_SILENCE(ZEND_OPCODE_HANDLER_ARGS)
{
	if(SCREAM_G(enabled))
	{
		execute_data->opline++;
		return ZEND_USER_OPCODE_CONTINUE;
	}
	return ZEND_USER_OPCODE_DISPATCH;
}
