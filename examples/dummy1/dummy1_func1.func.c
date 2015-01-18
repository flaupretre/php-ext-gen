# A complex test case for php-ext-gen

# string dummy1_func1(array|bool $arg1, double &$arg2, string &$arg3
#				, [string &$arg4="foo"])

arguments:
  arg1:
    type: array|bool
  arg2:
    type: double
    byref: true	
  arg3:
    type: string
    byref: true
  arg4:
    type: string
    byref: true
    default: foo

#------------------------------ Function code ------------------------------

{% block user_body %}
/* Unnecessarily complicated function body */

time_t t;

/* Print arg1 type and value */
/* arg1 can be array, bool, or null */

if(arg1->_is_null) {	/* Check if null */
	printf("arg1 is null\n");
}
else if (arg1_array) {	/* Check if array */
	printf("arg1 is an array\n");
} else {	/* Not null, not array, we know it's a bool */
	printf("arg1 is bool, value=%d\n",arg1);
}

/* Print arg2, and modify value (pass by ref) */
/* No need to check for null, as nullok is false */

printf("Received arg2 is %f\n",arg2);
arg2 *= 3;
printf("Returned arg2 is %f\n",arg2);

/* Print arg3, reallocate for a bigger string and display again */

{
char *p="I need space, more SPACE !!!";

printf("Received arg3 is <%s> (length=%d)\n",arg3,arg3_len);
EG_ARG_STRING_REPLACE(arg3, p);
strcpy(arg3,p);
printf("Returned arg3 is <%s>",arg3,arg3_len);
}

/* Print arg4 and replace it with a string */
/* Here, we use another macro to get memory for the new string */
/* Note that, even if received type is array, it is replaced by a string */
/* without leaking memory nor exposing zval API */

if(arg4_is_null) {	/* Check if null */
	printf("arg4 is null\n");
} else {	/* Not null, we know it's a string */
	printf("arg4 is a string, value=<%s>\n",arg4);
}

EG_FUNC_ARG_STRING_REALLOC(arg4, 100);
strcpy(arg4->sval,"string to return");
printf("arg4 - new length is %d\n",arg4->slen); /* Display 100 */

/* Now, return value */
/* Not thread-safe, but that's just a prototype :) */

t=time((time_t *)0);
EG_FUNC_RETURN_STRING(ctime(&t),1);

{% endblock body %}
