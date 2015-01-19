--TEST--
Dummy func1 (variation 1)
--FILE--
<?php
var_dump(function_exists('dummy1_func1'));

var_dump(DUMMY1_CONST1);
var_dump(DUMMY1_COLOR);

?>
===DONE===
--EXPECT--
bool(true)
int(10)
string(3) "red"
===DONE===
