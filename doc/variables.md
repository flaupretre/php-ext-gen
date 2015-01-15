# PHP to C mapping

<table>

<tr>
<th>PHP</td>
<th>C (PHP 5)</td>
<th>C (PHP 7)</td>
<th>Comments
</tr>

<tr>
<td>bool</td>
<td>zend_bool <i>var</i></td>
<td>int <i>var</i></td>
<td>false if <i>var</i> == 0, true otherwise
</tr>

<tr>
<td>int</td>
<td>long <i>var</i></i></td>
<td>zend_long <i>var</i></td>
<td>&nbsp;</td>
</tr>

<tr>
<td>double</i></td>
<td>double <i>var</i></td>
<td>double <i>var</i></td>
<td>&nbsp;</td>
</tr>

<tr>
<td>string</td>
<td>char *<i>var</i><br>int <i>var</i>_len</td>
<td>char *<i>var</i><br>size_t <i>var</i>_len</td>
<td>extend using :<br>EXTGEN_STR_REALLOC(<i>var</i>,[new_len])</td>
</tr>

<tr>
<td>array</td>
<td>Hastable *<i>var</i>_array</td>
<td>Hastable *<i>var</i>_array</td>
<td></td>
</tr>

<tr>
<td>mixed (arg only)</td>
<td>Hastable *<i>var</i>_array<br>+ scalar vars</td>
<td>Hastable *<i>var</i>_array<br>+ scalar vars</td>
<td>If arg is array, <i>var</i>_array is non-NULL</td>
</tr>

</table>

When an argument sets 'nullok', a variable named <i>var</i>\_is\_null is created. It contains a value != 0 if null was set for this arg. In this case, the 'normal' variables are null or 0.

# Return value

The 'retval' prefix is reserved for return value.

Two other int variables, 'retval\_false' and 'retval\_null', allow to take precedence on return value. If one of these two vars is set to a non-0 value, the 'normal' return value is ignored. If both are non-0 (shouldn't happen), false is returned.