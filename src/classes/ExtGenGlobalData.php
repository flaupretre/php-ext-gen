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
// Properties need to be declared to be accessible bt twig using '.'

public $header;
public $trailer;
public $minit_pre_code;
public $minit_post_code;
public $mshutdown_pre_code;
public $mshutdown_post_code;
public $minfo_code;
public $rinit_code;
public $rshutdown_code;
public $config_m4;
public $module_globals_code;
public $module_globals_init_code;
public $module_globals_dtor_code;

private static $data_files=array(
	 'header.twig.c' => 'header'
	,'trailer.twig.c' => 'trailer'
	,'minit_pre.twig.c' => 'minit_pre_code'
	,'minit_post.twig.c' => 'minit_post_code'
	,'mshutdown_pre.twig.c' => 'mshutdown_pre_code'
	,'mshutdown_post.twig.c' => 'mshutdown_post_code'
	,'minfo.twig.c' => 'minfo_code'
	,'rinit.twig.c' => 'rinit_code'
	,'rshutdown.twig.c' => 'rshutdown_code'
	,'config.twig.m4' => 'config_m4'
	,'module_globals.twig.h' => 'module_globals_code'
	,'module_globals_init.twig.c' => 'module_globals_init_code'
	,'module_globals_dtor.twig.c' => 'module_globals_dtor_code'
	);

//---------

public function __construct($main)
{
foreach(self::$data_files as $fname => $prop)
	{
	$buf=$main->optional_file_contents($fname);
	if (is_null($buf)) $buf='';
	$this->$prop=$buf;
	}
}

//------------
// Expand every property through renderer

public function expand($renderer)
{
foreach(self::$data_files as $fname => $prop)
	{
	$this->$prop=$renderer->render_string($fname,$this->$prop);
	}
}

//============================================================================
} // End of class
?>
