
/* Compute return value */

switch(eg_ret_s.type)
	{
	case EG_IS_NULL:
		break;

	case EG_IS_BOOL:
		EG_ZVAL_BOOL(return_value, eg_ret_s.bool_val);
		break;

	case EG_IS_INT:
		EG_ZVAL_INT(return_value, eg_ret_s.int_val);
		break;

	case EG_IS_DOUBLE:
		EG_ZVAL_DOUBLE(return_value, eg_ret_s.double_val);
		break;

	case EG_IS_STRING:
		EG_ZVAL_STRINGL(return_value, eg_ret_s.string_val,eg_ret_s.string_len,0);
		break;

	case EG_IS_ARRAY:
		EG_ZVAL_ARRAY(return_value, eg_ret_s.array_val);
		break;

/* Compute pass-by-ref returned values */


