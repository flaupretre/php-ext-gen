--TEST--
Scream: Simple test
--SKIP--
<?php
if(!extension_loaded("scream")) {
    echo "skip";
}
?>
--INI--
scream.enabled=0
error_reporting=E_ALL|E_STRICT
--FILE--
<?php
$foo = @$bar1;
ini_set('scream.enabled', 1);
$foo = @$bar2;
ini_set('scream.enabled', 0);
$foo = @$bar3;

echo "--DONE--";
?>
--EXPECTF--
Notice: Undefined variable: bar2 in %s on line %d
--DONE--
