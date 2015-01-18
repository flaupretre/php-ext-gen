# PHP to C type mapping

<table>

<tr>
<th>PHP</th>
<th>EG</th>
<th>EG arg<br> field</th>
<th>PHP 5</th>
<th>PHP 7</th>
<th>HHVM</th>
</tr>

<tr>
<td>bool</td>
<td>eg_bool</td>
<td>bval</td>
<td>zend_bool</td>
<td>int</td>
<td>?</td>
</tr>

<tr>
<td>int</td>
<td>eg_int</td>
<td>ival</td>
<td>long</td>
<td>zend_long</td>
<td>?</td>
</tr>

<tr>
<td>double</i></td>
<td>eg_double</i></td>
<td>dval</td>
<td>double</td>
<td>double</td>
<td>?</td>
</tr>

<tr>
<td>string pointer</td>
<td>eg_string</td>
<td>sval</td>
<td>char *</td>
<td>char *</td>
<td>?</td>
</tr>

<tr>
<td>string size</td>
<td>eg_str_size</td>
<td>slen</td>
<td>int</td>
<td>size_t</td>
<td>?</td>
</tr>

<tr>
<td>array</td>
<td>eg_array</td>
<td>aval</td>
<td>Hastable *</td>
<td>Hashtable *</td>
<td>?</td>
</tr>

</table>

# Function return value

There is no restriction on function return value type. The function code can implement its own logic to return any supported value type. You should execute one of this macro only once per function execution. When called several times, only the first call sets the value. Subsequent calls are ignored (TODO implement this protection).

Pre-defined macros are called from user code to set the return value.

<table>
<tr>
<th>Macro</th>
<th>Comments</td>
</tr>

<tr>
<td>EG_FUNC_RETVAL_NULL()</td>
<td>Sets null return value</td>
</tr>

<tr>
<td>EG_FUNC_RETVAL_BOOL(bool)</td>
<td>Sets boolean return value</td>
</tr>


<tr>
<td>EG_FUNC_RETVAL_FALSE()<br>EG_FUNC_RETVAL_TRUE()</td>
<td>Sets true/false as return value</td>
</tr>
<tr>
<td>EG_FUNC_RETVAL_INT(int)</td>
<td>Sets an integer return value</td>
</tr>

<tr>
<td>EG_FUNC_RETVAL_DOUBLE(double)</td>
<td>Sets a double return value</td>
</tr>

<tr>
<td>EG_FUNC_RETVAL_STRINGL(str, len, dup)</td>
<td>Sets a string return value, providing explicit size. Use this macro if you know the size of your string. <p>
For binary strings (which may contain null chars), this macro is the one to use.<p>
The resulting string must be stored in dynamically allocated memory. In most cases, setting 'dup' to 1 allocates and copies the string content for you. If string memory is already dynamically allocated and under your control, you can transfer the string with dup=0. In this case, you must not modify anything relative to this pointer later nor free it.<p>
Never set a null dup value for string literals !
</td>
</tr>

<tr>
<td>EG_FUNC_RETVAL_STRING(str, dup)</td>
<td>The same as EF_FUNC_RETVAL_STRINGL but string size is computed using strlen(). Not binary-safe.</td>
</tr>

<tr>
<td>EG_FUNC_RETVAL_ARRAY(val,dup)</td>
<td>Sets an array return value. This array must contain zval elements only<p>
As with strings, the array can be duplicated by setting a non-null dup parameter.</td>
</tr>
</table>

Another set of macros exists. Their names are derived from the macros listed above by replacing 'RETVAL' with 'RETURN'. As their name implies, they set the return value, and, then exit from the function.


# Function arguments

## Types ##

<table>
<tr>
<th>Type</th>
<th>Supports<br>pass<br>by ref</td>
<th>Supports<br>default<br>value</td>
<th>Possible<br>default<br>values</td>
<th>Built-in<br>default<br>value</td>
<th>Comments</td>
</tr>

<tr>
<td>zval</td>
<td>Yes</td>
<td>No</td>
<td></td>
<td></td>
<td>Used to delegate (yet) unsupported type management to user code</td>
</tr>

<tr>
<td>bool</td>
<td>Yes</td>
<td>Yes</td>
<td>EG_TRUE/<br>EG_FALSE</td>
<td>False</td>
<td>Null is received as 0</td>
</tr>

<tr>
<td>int</td>
<td>Yes</td>
<td>Yes</td>
<td>Integer</td>
<td>0</td>
<td>Null is received as 0</td>
</tr>

<tr>
<td>double</td>
<td>Yes</td>
<td>Yes</td>
<td>Numeric</td>
<td>0</td>
<td>Null is received as 0</td>
</tr>

<tr>
<td>string</td>
<td>Yes</td>
<td>Yes</td>
<td>String</td>
<td>NULL</td>
<td></td>
</tr>

<tr>
<td>array</td>
<td>Yes</td>
<td>No</td>
<td></td>
<td></td>
<td></td>
</tr>

<tr>
<td>array|null</td>
<td>No</td>
<td>No</td>
<td></td>
<td></td>
<td></td>
</tr>

<tr>
<td>array|{bool/int/double/string}</td>
<td>No <sup>(1)</sup></</td>
<td>Yes</td>
<td>Same as 2nd member</td>
<td>Same as 2nd member</td>
<td>Null is received as 0</td>
</tr>
</table>

## Macros

A set of macros, similar to the macros used to set the return value, are used to set the
returned value of arguments passed by ref.

<b>Notes:</b>

* When an argument is passed by ref, the returned value can have any type.

* When an argument is passed by value, it is considered read-only. Using one of the macros below with a passed-by-val argument will do nothing.

<table>
<tr>
<th>Macro</th>
</tr>

<tr>
<td>EG_FUNC_ARG_NULL(arg)</td>
</tr>

<tr>
<td>EG_FUNC_ARG_BOOL(arg,bool)</td>
</tr>

<tr>
<td>EG_FUNC_ARG_FALSE(arg)<br>EG_FUNC_ARG_TRUE(arg)</td>
</tr>
<tr>
<td>EG_FUNC_ARG_INT(arg,int)</td>
</tr>

<tr>
<td>EG_FUNC_ARG_DOUBLE(arg,double)</td>
</tr>

<tr>
<td>EG_FUNC_ARG_STRINGL(arg,str, len, dup)</td>
</tr>

<tr>
<td>EG_FUNC_ARG_STRING(arg,str, dup)</td>
</tr>

<tr>
<td>EG_FUNC_ARG_ARRAY(arg,val,dup)</td>
</tr>
</table>

## Accessing argument values

The function body receives a pointer for each function argument. This pointer has the same name as the name the argument was declared with.

* For arguments declared with type 'zval', the argument is a 'zval *'. The user code is totally responsible of the way it uses this zval. If the argument is passed by ref, the user code can change the value of this zval.

* For other declared types, the transmitted argument is a pointer to a structure containing 'decoded fields'. Depending on the argument type, the user code can use the following fields :

<table>
<tr>
<th>field</th>
<th>Type</th>
<th>Comments</th>
</tr>

<tr>
<td>type</td>
<td>eg_type<br>(numeric)</td>
<td>One of EG_IS_NULL, EG_IS_BOOL, EG_IS_INT, EG_IS_DOUBLE, EG_IS_STRING, EG_IS_ARRAY<p>Used in case of mixed types. Using this field, you can determine which argument type was passed to the function.<p>
Example: If argument type is 'array|string', this field can be equal to EG_IS_ARRAY or EG_IS_STRING. Depending on this value, you will access the corresponding fields below.<p>
When argument type is not mixed, this field is provided but useless as you already know the type of your argument, so you which field to access.</td>
</tr>

<tr>
<td>bval</td>
<td>eg_bool</td>
<td>
The value of a boolean argument</td>
</tr>

<tr>
<td>ival</td>
<td>eg_int</td>
<td>
The value of an integer argument</td>
</tr>

<tr>
<td>dval</td>
<td>eg_double</td>
<td>
The value of a double argument</td>
</tr>

<tr>
<td>sval</td>
<td>eg_string</td>
<td>
The address of a dynamically-allocated memory buffer containing a string argument (always null-terminated)</td>
</tr>

<tr>
<td>slen</td>
<td>eg_str_size</td>
<td>
String size</td>
</tr>

<tr>
<td>aval</td>
<td>eg_array</td>
<td>
A pointer to an array</td>
</tr>

<tr>
<td>is_unset</td>
<td>eg_bool</td>
<td>
This field is true only if the argument is optional AND was not set when the function was called. In this case, the structure already contains a default value (defined by the user or built-in).<p>
This field can be used, for instance, to decide if we must compute a complex default value (one to complex to set in the function definition).</td>
</tr>

</table>

# Examples

The examples below show sample function definition file. A function definition file, as its name implies, defines a function. Its name is '<function-name>.func.c'. It contains everything related to the function.

A simple 'bridge' function taken from the newt extension :

	#PHP prototype:
	#    int newt_centered_window(int $width, int $height [, string $title=null ])
	#
	# No need for an explicit default value for 'title', as our default for string
	# is a NULL pointer and this is the same in newt.
	#---------------------------------------------------------------------------- 

	arguments:
	  width:
	    type: int
	  height:
	    type: int
	  title:
	    type: string
		optional: true 

	{% block body %}
	int ret;

	ret=newtCenteredWindow((long)(width->ival),(long)(height->ival),title->sval);
	EG_FUNC_RETURN_INT(ret);
	{% endblock %}


A minimal function taking no argument and always returning null:

	# void newt_bell()
	# This function has no metadata to define, as it has no argument
	
	{% block body %}
	newt_Bell();
	{% endblock %}

A dummy function taking 2 arguments of type 'double', one by ref, and the second by value. It replaces the first argument with the product of both args. Useless but illustrates pass-by-ref.

	#PHP: void dummy_mult(&$var1, $var2)
	
	arguments:
	  var1:
	    type: double
	    byref: true
	  var2:
	    type: double

	{% block body %}
	/* Could be written in one line with no intermediate var... */
	double ret;

	ret = var1->dval * var2->dval;
	EG_FUNC_ARG_DOUBLE(var1, ret); // set var1 returned value
	{% endblock %}
