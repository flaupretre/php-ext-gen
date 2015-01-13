<?php
//=============================================================================
/**
* This class is the base class of every generator
*
* @copyright Francois Laupretre <TODO>
* @license TODO
* @category php-ext-gen
* @package php-ext-gen
*/
//============================================================================
//---- All generator classes extend ExtGen through ExtGenGenerator
//---- None of these classes has a constructor

abstract class ExtGenGenerator
{

//----- Properties

public $engine;		// Target PHP engine
public $engine_version;	// Target engine version
public $source_dir;	// Source directory
public $dest_dir;	// Output directory

protected $parser;	// Metadata parser

public $name;		// Extension name
public $version;	// Extension version
public $namespace;	// Namespace (null if not defined)

public $autoconf;	// Autoconf-related info

public $global_data;

public $functions;	// Function table. Array of ExtGenFunctions instances
public $constants;	// Constant table. Array of ExtGenConstant instances

//---------

abstract public function init();
abstract public function generate();

//-----
// If $format=null -> automatic

public static function get_generator($format,$source_dir,$dest_dir)
{
if (!is_dir($source_dir))
	throw new Exception($source_dir.': directory not found');
if (!is_dir($dest_dir))
	throw new Exception($dest_dir.': directory not found');

// Compute engine and engine version

if (is_null($formatt))
	{	// Determine from running engine
	if (defined('HHVM_VERSION'))
		{
		$engine='hhvm';
		$version=HHVM_VERSION;
		}
	else	// PHP interpreter
		{
		$engine='php';
		$version=phpversion();
		}
	}
else
	{
	list($engine,$version)=explode(':',$format,2);
	if (($engine==='')||($version===''))
		throw new Exception("$format: Invalid output format");
	}

$engine=strtolower($engine);
$gen=null;
switch($engine)
	{
	case 'hhvm':
		$gen='hhvm';
		break;
	case 'php':
		if (strpos($version,'5.')===0) $gen='php5';
		if (strpos($version,'7.')===0) $gen='php7';
		break;
	default:
		throw new Exception("$engine: Unsupported PHP engine");
	}

if(is_null($gen)) throw new Exception("$format: Unsupported output format");
PHO_Display::trace("Using $gen generator");

$gen_class='ExtGenGenerator'.strtoupper($gen);
$obj=new $gen_class();

PHO_Display::trace("Generating for (engine=$engine ; version=$version)");

$obj->engine=$engine;
$obj->engine_version=$version;
$obj->source_dir=$source_dir;
$obj->dest_dir=$dest_dir;
$obj->parser=new ExtGenMetaParser($source_dir);

$obj->init();
return $obj;
}

//-----
// Method called when a generator is starting its generate() method
// Contains common pre-generate processing

protected function init_generate()
{
PHO_Display::info('Generating code...');
}

//-----
// Return empty string if file does not exist

public function optional_file_contents($rpath)
{
try
	{
	return $this->file_contents($rpath);
	}
	catch(Exception $e) { return ''; }
}

//-----
// File must exists

public function file_contents($rpath)
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
PHO_Display::info('Reading source files...');

try {
	$data=$this->parser->decode($this->file_contents($current_file='global.yml'));

	//--- Basic information

	$this->name=ExtGen::element($data,'name');
	$this->namespace=ExtGen::optional_element($data,'namespace');

	//--- autoconf-related information

	$def=ExtGen::optional_element($data,'autoconf');
	$this->autoconf=(is_null($autoconf) ? null : new ExtGenAutoconf($def));

	//--- Get function names

	$funcs=ExtGen::optional_element($data,'functions');
	if (is_null($funcs)) $funcs=array();
	if (!is_array($funcs))
		throw new Exception("'functions' element must be an array");

	//--- Constants

	$constants=ExtGen::optional_element($data,'constants');
	if (is_null($constants)) $constants=array();
	if (!is_array($constants))
		throw new Exception("'constants' element must be an array");
	$this->constants=array();
	foreach($constants as $name => $def)
		{
		try	{
			if (array_key_exists($name,$this->constants))
				throw new Exception("Constant $name already defined");
			$obj=new ExtGenConstant($name,$def);
			$this->constants[$name]=$obj;
			} catch(Exception $e) {	throw new Exception($e->getMessage()." (while defining constant $name)"); }
		}
	} catch(Exception $e) { throw new Exception("$current_file: ".$e->getMessage()); }

// Functions

$this->functions=array();
foreach($funcs as $name)
	{
	if (array_key_exists($name,$this->functions))
		throw new Exception("Function $name already defined");
	try	{
		$buf=$this->file_contents('func_'.$name.'.c');
		$this->functions[$name]=new ExtGenFunction($name,$buf,$this->parser);
		} catch(Exception $e) { throw new Exception($e->getMessage()." (while defining function $name)"); }
	}

// Global data

$this->global_data=new ExtGenGlobalData($this);
}

//============================================================================
} // End of class
?>
