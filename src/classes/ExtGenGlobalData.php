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

private static $data_files=array(
	 'header.c' => 'header'
	,'trailer.c' => 'trailer'
	,'minit_pre.c' => 'minit_pre_code'
	,'minit_post.c' => 'minit_post_code'
	,'mshutdown_pre.c' => 'mshutdown_pre_code'
	,'mshutdown_post.c' => 'mshutdown_post_code'
	,'minfo.c' => 'minfo_code'
	,'rinit.c' => 'rinit code'
	,'rshutdown.c' => 'rshutdown_code'
	,'config.m4' => 'm4_config'
	);

//---------

public function __construct($main)
{
foreach(self::$data_files as $fname => $prop)
	$this->$prop=$main->optional_file_contents($fname);
}

//============================================================================
} // End of class
?>
