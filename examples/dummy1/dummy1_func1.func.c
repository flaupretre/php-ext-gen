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

eg_str_val p;
time_t t;

/* Print arg1 type and value */
/* arg1 can be array, bool, or null */

if (arg1->type==EG_IS_ARRAY) {	/* Check if array */
	printf("arg1 is an array\n");
} else {	/* Not array, we know it's a bool */
	printf("arg1 is bool, value=%d\n",arg1->bval);
}

/* Print arg2, and modify value (pass by ref) */

printf("Received arg2 is %f\n",arg2->dval);
EG_FUNC_ARG_DOUBLE(arg2,arg2->dval * 3);
printf("Returned arg2 is %f\n",arg2->dval);

/* Print arg3, reallocate for a bigger string and display again */

{
p="I need space, more SPACE !!!";

printf("Received arg3 is <%s> (length=%d)\n",arg3->sval,arg3->slen);
EG_FUNC_ARG_STRING(arg3, p, 1);
printf("Returned arg3 is <%s> (length=%d)\n",arg3->sval,arg3->slen);
}

/* Arg 4 */

printf("Received arg4 is <%s> (length=%d)\n",arg4->sval,arg4->slen);

p=eg_eallocate(NULL,101);
strcpy(p,"string to return");
EG_FUNC_ARG_STRINGL(arg4, p, 100, 0);

printf("arg4 - new length is %d\n",arg4->slen); /* Display 100 */

/* Now, return value */
/* Not thread-safe, but that's just a prototype :) */

t=time((time_t *)0);
EG_FUNC_RETURN_STRING(ctime(&t),1);

{% endblock user_body %}
