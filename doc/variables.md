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

When an argument sets 'nullok', a variable named <i>var</i>\_is\_null is created. It contains a value != 0 if null was set for this arg. In this case, the 'normal' variables are null or 0.

# Function return value

There is no restriction on function return value type. The function code can implement its own logic to return any supported value type.

Pre-defined macros are used from user body code the return value. These macros can be used several times, only the last call will set the return value. They also take care of releasing the memory theu could have allocated in a previous execution (in the same function execution).

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
As with strings, the array wan be duplicated using a non-null dup parameter.</td>
</tr>
</table>

Another set of macros exists. Their names are derived from the macros listed above by replacing 'RETVAL' with 'RETURN'. As their name implies, they set the return value, and, then exit from the function.


# Function arguments

Supported types, with possible options :

<table>
<tr>
<th>Type</th>
<th>Supports<br>nullok</td>
<th>Supports<br>default<br>value</td>
<th>Possible<br>default<br>values</td>
<th>Implicit<br>default<br>value</td>
<th>Comments</td>
</tr>

<tr>
<td>zval</td>
<td>No</td>
<td>No</td>
<td>&nbsp;</td>
<td>&nbsp</td>
<td>Used to delegate (yet) unsupported type management to user code</td>
</tr>

<tr>
<td>bool</td>
<td>No</td>
<td>Yes</td>
<td>EG_TRUE/<br>EG_FALSE</td>
<td>False</td>
<td>Null is received as 0</td>
</tr>

<tr>
<td>int</td>
<td>No</td>
<td>Yes</td>
<td>Integer</td>
<td>0</td>
<td>Null is received as 0</td>
</tr>

<tr>
<td>double</td>
<td>No</td>
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
<td>array|{bool/int/double/string}</td>
<td>Yes</td>
<td>Yes</td>
<td>Same as 2nd member</td>
<td>Same as 2nd member</td>
<td>Null is received as 0</td>
</tr>
</table>

Every argument types can be passed by reference or by value.

A set of macros, similar to the macros used to set the return value, are used to set the
returned value of arguments passed by ref:




























