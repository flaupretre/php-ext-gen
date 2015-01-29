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

eg_string p;
eg_float f;

/* Print arg1 type and value */
/* arg1 can be array, bool, or null */

if (EG_TYPE(arg1)==EG_IS_ARRAY) {	/* Check if array */
	php_printf("arg1 is an array\n");
} else {	/* Not array, we know it's a bool */
	php_printf("arg1 is bool, value=%d\n",EG_BVAL(arg1));
}

/* Print arg2, and modify value (pass by ref) */

php_printf("Received arg2 is %f\n",EG_FVAL(arg2));
f=3.14 * EG_FVAL(arg2);
p="replaced arg2";
EG_ARG_SET_STRING(arg2,p,1);

/* Print arg3, reallocate and replace with a larger string */

p="I need space, more SPACE !!!";

php_printf("Received arg3 is <%s> (length=%d)\n",EG_STRVAL(arg3),EG_STRLEN(arg3));
EG_ARG_SET_STRING(arg3, p, 1);

/* Arg 4 */

if (EG_ARG_IS_UNSET(arg4))
	php_printf("arg4 is not set\n");
else
	{
	php_printf("Received arg4 is <%s> (length=%d)\n",EG_STRVAL(arg4),EG_STRLEN(arg4));
	EG_ARG_SET_FLOAT(arg4, f);
	}

/* Now, return value */

EG_RETURN_STRING("Return",1);

{% endblock user_body %}
