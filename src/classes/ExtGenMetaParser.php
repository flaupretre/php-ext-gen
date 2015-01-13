<?php
//=============================================================================
/**
* This class is a metadata parser.
*
* It supports multiple syntaxes
*
* @copyright Francois Laupretre <TODO>
* @license TODO
* @category php-ext-gen
* @package php-ext-gen
*/
//============================================================================

class ExtGenMetaParser
{

//----- Properties

public $syntax;
private $json;

//---------
// Determines metadata syntax from the files present in the source dir

public function __construct($dir)
{
if (file_exists($dir.'/global.yml'))
	{
	$this->syntax='yaml';
	}
elseif (file_exists($dir.'/global.json'))
	{
	$this->syntax='json';
	$this->json=new Services_JSON();
	}
else
	{
	throw new Exception("Cannot determine metadata syntax");
	}
}

//-----
// Parse metadata

public function decode($buf)
{
switch($this->syntax)
	{
	case 'yaml':
		$ret=\Symfony\Component\Yaml\Yaml::parse($buf);
		break;

	case 'json':
		$ret=self::$json->decode($buf);
		break;
	}
if ($ret===null) $ret=array();

if (!is_array($ret))
	{
	var_dump($buf);
	var_dump($ret);
	throw new Exception('Parser returned non-array');
	}

return $ret;
}

//============================================================================
} // End of class
?>
