# PHP to C mapping

<table>

<tr>
<th>PHP</th>
<th>EG type</th>
<th>Var names</th>
<th>PHP 5</th>
<th>PHP 7</th>
<th>HHVM</th>
</tr>

<tr>
<td>bool</td>
<td>eg_bool</td>
<td><i>var</i></td>
<td>zend_bool</td>
<td>int</td>
<td>?</td>
</tr>

<tr>
<td>int</td>
<td>eg_int</td>
<td><i>var</i></td>
<td>long</td>
<td>zend_long</td>
<td>?</td>
</tr>

<tr>
<td>double</i></td>
<td>eg_double</i></td>
<td><i>var</i></td>
<td>double</td>
<td>double</td>
<td>?</td>
</tr>

<tr>
<td>string</td>
<td>eg_string<br>eg_str_size</td>
<td><i>var</i><br><i>var</i>_len</td>
<td>char *<br>int</td>
<td>char *<br>size_t</td>
<td>?</td>
</tr>

<tr>
<td>array</td>
<td>eg_array</td>
<td><i>var</i>_array</td>
<td>Hastable *</td>
<td>Hastable *</td>
<td>?</td>
</tr>

</table>

When an argument sets 'nullok', a variable named <i>var</i>\_is\_null is created. It contains a value != 0 if null was set for this arg. In this case, the 'normal' variables are null or 0.

# Return value

The 'retval' prefix is reserved for return value.

Two other int variables, 'retval\_false' and 'retval\_null', allow to take precedence on return value. If one of these two vars is set to a non-0 value, the 'normal' return value is ignored. If both are non-0 (shouldn't happen), false is returned.