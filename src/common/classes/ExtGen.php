<?php
//=============================================================================
/**
* This is static-only class with methods used from other classes.
*
* @copyright Francois Laupretre <TODO>
* @license TODO
* @category php-ext-gen
* @package php-ext-gen
*/
//============================================================================

class ExtGen
{
//----- Properties

//-----
// Return software version from package metadata

public static function version()
{
$mnt=\PHK\Mgr::uriToMnt(__FILE__);
$pkg=\PHK\Mgr::instance($mnt);
return $pkg->option('version');
}

//-----
// Return null if element does not exist

public static function optional_element($arr,$key)
{
return self::element($arr,$key,true);
}

//-----
// Element must exist

public static function element($arr,$key,$optional=false)
{
if (!is_array($arr))
	{
	var_dump($arr);
	throw new Exception('Input arg is not an array');
	}

if (!array_key_exists($key,$arr))
	{
	if (!$optional)	throw new Exception("$key: Key not found");
	return null;
	}

return $arr[$key];
}

//============================================================================
} // End of class
?>
