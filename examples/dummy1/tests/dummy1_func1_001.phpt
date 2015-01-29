--TEST--
Dummy func1 (variation 1)
--FILE--
<?php
var_dump(function_exists('dummy1_func1'));

$arg1=array('a' => 1, 'b' => 2.5, 'c' => 'foo');
$arg2=2;
$arg3='this is arg3';
$arg4='dummy';

echo "\n==== Run 1:\n";

$result=dummy1_func1($arg1,$arg2,$arg3);

echo "arg2 :\n";
var_dump($arg2);

echo "arg3 :\n";
var_dump($arg3);

echo "Result:";
var_dump($result);

echo "\n==== Run 2:\n";

$arg2=2;

dummy1_func1($arg1,$arg2,$arg3,$arg4);

echo "arg2\n";
var_dump($arg2);

echo "arg3\n";
var_dump($arg3);

echo "arg4\n";
var_dump($arg4);

echo "Constants :";

var_dump(DUMMY1_CONST1);
var_dump(DUMMY1_COLOR);

?>
===DONE===
--EXPECT--
bool(true)

==== Run 1:
arg1 is an array
Received arg2 is 2.000000
Received arg3 is <this is arg3> (length=12)
arg4 is not set
arg2 :
string(13) "replaced arg2"
arg3 :
string(28) "I need space, more SPACE !!!"
Result:string(6) "Return"

==== Run 2:
arg1 is an array
Received arg2 is 2.000000
Received arg3 is <I need space, more SPACE !!!> (length=28)
Received arg4 is <dummy> (length=5)
arg2
string(13) "replaced arg2"
arg3
string(28) "I need space, more SPACE !!!"
arg4
float(6.28)
Constants :int(10)
string(3) "red"
===DONE===
