<?php
//=============================================================================
/**
* This class stores global data taken from various files
*
* @copyright Francois Laupretre <TODO>
* @license TODO
* @category php-ext-gen
* @package php-ext-gen
*/
//============================================================================

class ExtGenGlobalData
{

//----- Properties

public $gen;

// Properties need to be declared to be accessible from twig using '.'

public $header;
public $trailer;
public $config_m4;

private static $data_files=array(
	 'header.twig.c' => 'header'
	,'trailer.twig.c' => 'trailer'
	,'config.twig.m4' => 'config_m4'
	);

//---------

public function __construct($gen)
{
$this->gen=$gen;

foreach(self::$data_files as $fname => $prop)
	{
	$buf=$gen->optional_file_contents($fname);
	if (is_null($buf)) $buf='';
	$this->$prop=$buf;
	}
}

//------------
// Expand every property through renderer

public function prepare()
{
foreach(self::$data_files as $fname => $prop)
	{
	$this->$prop=$this->gen->renderer->render_string($fname,$this->$prop);
	}
}

//============================================================================
} // End of class
?>
