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
<td>int <i>var</i></td>
<td>int <i>var</i></td>
<td>false if <i>var</i> == 0, true otherwise
</tr>

<tr>
<td>int</td>
<td>?</i></td>
<td>zend_long <i>var</i></td>
<td>&nbsp;</td>
</tr>

<tr>
<td>double</i></td>
<td>?</i></td>
<td>double <i>var</i></td>
<td>&nbsp;</td>
</tr>

<tr>
<td>string</td>
<td>?</i></td>
<td>char *<i>var</i><br>size_t <i>var</i>_len</td>
<td>extend using :<br>EXTGEN_STR_REALLOC(<i>var</i>,[new_len])</td>
</tr>

<tr>
<td>array</td>
<td>?</i></td>
<td>Hastable *<i>var</i>_array</td>
<td></td>
</tr>

<tr>
<td>mixed (arg only)</td>
<td>?</i></td>
<td>Hastable *<i>var</i>_array<br>+ scalar vars</td>
<td>If arg is array, <i>var</i>_array is non-NULL</td>
</tr>

</table>

When an argument sets 'nullok', a variable named <i>var</i>\_is\_null is created. It contains a value != 0 only if null was set for this arg.

# Return value

The 'retval' prefix is reserved for return value.

Two other int variables, 'retval_false' and 'retval_null', allow to take precedence on return value. If one of these two vars is set to a non-0 value, the 'normal' return value is ignored. If both are non-0 (shouldn't happen), false is returned.