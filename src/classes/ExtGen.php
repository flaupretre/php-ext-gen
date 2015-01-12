<?php
//=============================================================================
/**
* This the main php-ext-gen class.
*
* @copyright Francois Laupretre <TODO>
* @license TODO
* @category php-ext-gen
* @package php-ext-gen
*/
//============================================================================

class ExtGen
{

private static $generators=array(
	'php5'	=> 'ExtGenGeneratorPHP5'
	,'php7'	=> 'ExtGenGeneratorPHP7'
	,'hhvm'	=> 'ExtGenGeneratorHHVM'
	);

private static $supported_syntaxes=array('yaml','json');

//----- Properties

private $source_dir;

private $syntax;	// Metadata syntax
private $json;		// json decoder (only if syntax==json)

private $name;		// Ext name

private $autoconf	// Autoconf-related info

private $header;	// C code to place at the beginning
private $trailer;	// C code to place at the end

private $functions	// Function table. Array of ExtGenFunctions instances
private $constants	// Constant table. Array of ExtGenConstant instances

//---------

public static function build($source_dir,$dest_dir, $format, $syntax)
{

/* Checks */

if (!is_dir($source_dir))
	throw new Exception("$source_dir: Source dir is not an existing directory");

if ((@lstat($dest_dir))!==false)
	throw new Exception("$dest_dir: Destination directory must not exist");

// Get input files

$ext=new self($source_dir);
$ext->read_input();

// Build generator

if (!array_key_exists($format,$generators))
	throw new Exception("$format: Invalid generator");

$gen_class=$generators[$format];
$worker=new $gen_class($source_dir);
$worker->read_source_data($source_dir);
$worker->output($dest_dir);
}

=============== Dynamic methods ====================================
//---- All generator classes extend ExtGen

protected function __construct($source_dir,$syntax)
{
$this->source_dir=$source_dir;

$syntax=strtolower($syntax);
if (array_search($syntax, self::$supported_syntaxes)===false
	throw new Exception("$syntax: Unsupported metadata syntax");
$this->syntax=$syntax;
}

//-----
// Init metadata parser

protected function syntax_init($buf)
{
switch($this->syntax)
	{
	case 'json':
		$this->json=new Services_JSON();
		break;
	}
}

//-----
// Parse metadata (returns array)

protected function parse($buf)
{
switch($this->syntax)
	{
	case 'yaml':
		return \Symfony\Component\Yaml\parse($buf);
		break;

	case 'json':
		return $json->decode($buf);
		break;
	}
}

//-----
// Return if element does not exist

public static function optional_element($arr,$key)
{
try
	{
	return self::element($arr,$key);
	}
catch(Exception $e) { return null; }
}

//-----
// Element must exist

public static function element($arr,$key)
{
if (!array_key_exists($key,$arr))
	throw new Exception("$key: Key not found");

return $arr[$key];
}

//-----
// Return empty string if file does not exist

protected function optional_file_contents($rpath)
{
try
	{
	return $this->file_contents($rpath);
	}
catch(Exception $e) { return ''; }
}

//-----
// File must exists

protected function file_contents($rpath)
{
$path=$this->source_dir.'/'.$rpath;

if (!file_exists($path)) throw new Exception("$path: File not found");

$data=trim(file_get_contents($path));
if ($data!=='') $data .= "\n";
return $data;
}

//-----

protected function read_source_data()
{
try {
	$data=$this->parse($this->file_contents($current_file='global.yml'));

	//--- Basic information

	$this->name=self::element($data,'name');

	//--- autoconf-related information

	$info=self::optional_element($data,'autoconf',array());
	if (!is_array($info))
		throw new Exception("'autoconf' element should be an array");
	$this->autoconf=$info;

	//--- Header/trailer code
	
	$rpath=self::optional_element($data,'header_file');
	if (!is_null($rpath)) $this->header=$this->file_contents($rpath);
	
	$rpath=self::optional_element($data,'trailer_file');
	if (!is_null($rpath)) $this->trailer=$this->file_contents($rpath);

	//--- Functions

	$funcs=self::optional_element($data,'functions');
	if (is_null($funcs)) $funcs=array();
	if (!is_array($funcs))
		throw new Exception("'functions' element must be an array");
	$this->functions=array();
	foreach($funcs as $name)
		{
		if (array_key_exists($name,$this->functions))
			throw new Exception("Function $name already defined");
		try
			{
			$buf=$this->file_contents($name.'.func');
			$this->functions[$name]=new ExtGenFunction($buf);
			}
		catch(Exception $e)
			{
			throw new Exception($e->getMessage()." while defining function $name()");
			}
		}

	//--- Constants

	$constants=self::optional_element($data,'constants');
	if (is_null($constants)) $constants=array();
	if (!is_array($constants))
		throw new Exception("'constants' element must be an array");
	$this->constants=array();
	foreach($constants as $def)
		{
		try
			{
			$name=self::element($def,'name');
			$type=strtolower(self::element($def,'type'));
			if ($type!=null)
			$obj=new ExtGenConstant($def
			$this->functions[$name]=new ExtGenFunction($buf);
			}
		catch(Exception $e)
			{
			throw new Exception($e->getMessage()." while defining function $name()");
			}
		}
	foreach($constants as $def)
		{
		try
		$name=self::element($def,'name');
		$this->constants[]=new ExtGenConstant($def);
		}

	



	}
catch(Exception $e)
	{
	throw new Exception("$current_file: ".$e->getMessage());
	}
}

//============================================================================
} // End of class
?>
