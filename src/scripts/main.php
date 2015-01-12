<?php

try
{
ini_set('display_errors',true);

$args=$_SERVER['argv'];
array_shift($args);
ExtGenCmd::run($args);
}
catch(Exception $e)
	{
	if (getenv('DEBUG')!==false) throw $e;
	else echo "*** ERROR: ".$e->getMessage()."\n\n";
	exit(1);
	}
