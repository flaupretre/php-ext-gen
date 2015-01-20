# A complex test case for php-ext-gen

# string dummy1_func1(array|bool $arg1, double &$arg2, string &$arg3
#				, [string &$arg4="foo"])

arguments:
  arg1:
    type: array|bool
  arg2:
    type: float
    byref: true	
  arg3:
    type: string
    byref: true
  arg4:
    type: string
    optional: true
    byref: true
    default: foo

#------------------------------ Function code ------------------------------

{% block user_body %}
/* Unnecessarily complicated function body */

eg_str_val p;
eg_float f;

/* Print arg1 type and value */
/* arg1 can be array, bool, or null */

if (arg1->type==EG_IS_ARRAY) {	/* Check if array */
	php_printf("arg1 is an array\n");
} else {	/* Not array, we know it's a bool */
	php_printf("arg1 is bool, value=%d\n",arg1->bval);
}

/* Print arg2, and modify value (pass by ref) */

php_printf("Received arg2 is %f\n",arg2->fval);
f=3.14 * arg2->fval;
p="replaced arg2";
EG_FUNC_ARG_STRING(arg2,p,1);

/* Print arg3, reallocate for a bigger string and display again */

p="I need space, more SPACE !!!";

php_printf("Received arg3 is <%s> (length=%d)\n",arg3->sval,arg3->slen);
EG_FUNC_ARG_STRING(arg3, p, 1);

/* Arg 4 */

if (arg4->is_unset)
	php_printf("arg4 is not set\n");
else
	{
	php_printf("Received arg4 is <%s> (length=%d)\n",arg4->sval,arg4->slen);
	EG_FUNC_ARG_FLOAT(arg4, f);
	}

/* Now, return value */

EG_FUNC_RETURN_STRING("Return",1);

{% endblock user_body %}
