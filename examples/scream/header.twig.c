
#include "ext/standard/php_smart_str.h"

/* ini entries */

PHP_INI_BEGIN()
STD_PHP_INI_BOOLEAN("scream.enabled", "0", PHP_INI_ALL, OnUpdateBool, enabled, zend_scream_globals, scream_globals)
PHP_INI_END()

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
