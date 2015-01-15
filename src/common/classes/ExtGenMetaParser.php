<?php
//=============================================================================
/**
* This class is the metadata parser base class.
*
* It supports multiple syntaxes
*
* @copyright Francois Laupretre <TODO>
* @license TODO
* @category php-ext-gen
* @package php-ext-gen
*/
//============================================================================

abstract class ExtGenMetaParser
{

//----- Properties

public $gen;

//---------

abstract public function decode($buf);

//---------

public function __construct($gen)
{
$this->gen=$gen;
}

//---------
// Determines metadata syntax from the files present in the source dir

public static function create($gen)
{
if (file_exists($gen->source_dir.'/global.yml'))
	$syntax='Yaml';
elseif (file_exists($gen->source_dir.'/global.json'))
	$syntax='Json';
else
	throw new Exception("Cannot determine metadata syntax");
	
$class='ExtGenMetaParser'.$syntax;
return new $class($gen);
}

//-----
// Parse metadata
// Ensure we return an array

protected function check_decode_result($buf,$ret)
{
if (!is_array($ret)) $ret=array();
return $ret;
}

//-----

public function decode_file($basename)
{
$file=$basename.'.'.$this->file_extension;
$buf=$this->gen->file_contents($file);
return $this->decode($buf);
}

//============================================================================
} // End of class
?>
