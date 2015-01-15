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
//---- All generator classes extend ExtGenGenerator
//---- None of these classes has a constructor

abstract class ExtGenGenerator
{

//----- Properties

public $source_dir;	// Source directory
public $dest_dir;	// Output directory
public $renderer;
public $parser;		// Metadata parser

//-- These properties are available for templates

public $software;	// Information about the software

public $engine;		// Target PHP engine. Array('name', 'version')
public $generator;	// generator name
public $flags;		// flags defined in global defs
public $cli_options; // Options sent from command line


public $name;		// Extension name
public $uname;		// The same converted to uppercase

public $version;	// Extension version

public $autoconf;	// Autoconf-related info

public $global_data;
public $extra_files; // array of ExtGenExtraFile instances

public $functions;	// Function table. Array of ExtGenFunctions instances
public $constants;	// Constant table. Array of ExtGenConstant instances

//---------
// If $format=null -> automatic

public static function get_generator($format,$source_dir,$dest_dir,$options)
{
if (!is_dir($source_dir))
	throw new Exception($source_dir.': directory not found');
if (!is_dir($dest_dir))
	throw new Exception($dest_dir.': directory not found');


// Compute engine and engine version

if (is_null($format))
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
$generator=null;
switch($engine)
	{
	case 'hhvm':
		$generator='hhvm';
		break;
	case 'php':
		if (strpos($version,'5.')===0) $generator='php5';
		if (strpos($version,'7.')===0) $generator='php7';
		break;
	default:
		throw new Exception("$engine: Unsupported PHP engine");
	}

if(is_null($generator)) throw new Exception("$format: Unsupported output format");
PHO_Display::trace("Using $generator generator");

$gen_class='ExtGenGenerator'.strtoupper($generator);
$obj=new $gen_class();

PHO_Display::trace("Generating for (engine=$engine ; version=$version)");

$obj->generator=$generator;
$obj->options=new ExtGenOptions($options);
$obj->engine=array('name' => $engine,'version' => $version);
$obj->source_dir=$source_dir;
$obj->dest_dir=$dest_dir;
$obj->parser=ExtGenMetaParser::create($obj);
$obj->renderer=new ExtGenRenderer($obj);
$obj->software=array('version'  => ExtGen::version());

$obj->init();

return $obj;
}

//-----
// Method called by the generator before it starts its specific processing

protected function generate()
{
PHO_Display::info('Generating code...');

$this->expand();
}

//-----
// Expand all strings that need to go through twig (C code and others)

private function expand()
{
$this->global_data->expand($this->renderer);
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

public function getclass($type)
{
$class='ExtGen'.$type.strtoupper($this->generator);
if (!class_exists($class,1)) $class='ExtGen'.$type;
return $class;
}

//-----

protected function read_source_data()
{
PHO_Display::info('Reading source files...');

// Read data from global definition file

try {
	$data=$this->parser->decode_file('global');

	//--- Basic information

	$name=ExtGen::element($data,'name');
	$allowed='ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_';
	if (strspn($name,$allowed)!==strlen($name))
		throw new Exception("$name: Invalid extension name");
	$this->name=$name;
	$this->uname=strtoupper($name);

	//--- Flags

	$flags=ExtGen::optional_element($data,'flags');
	if (is_null($flags)) $flags=array();

	if (!array_key_exists('debug',$flags)) $flags['debug']=false;
	
	$this->flags=$flags;

	//--- autoconf-related information

	$def=ExtGen::optional_element($data,'autoconf');
	if (is_null($def)) $def=array();
	$class=$this->getclass('Autoconf');
	$this->autoconf=new $class($def);

	//--- Extra files
	
	$extra_files=ExtGen::optional_element($data,'extra_files');
	if (is_null($extra_files)) $extra_files=array();
	$class=$this->getclass('ExtraFile');
	$this->extra_files=array();
	foreach($extra_files as $name => $def)
		$this->extra_files[$name]=new $class($this,$name,$def);

	//--- Get function names

	$funcnames=ExtGen::optional_element($data,'functions');
	if (is_null($funcnames)) $funcnames=array();
	if (!is_array($funcnames))
		throw new Exception("'functions' element must be an array");

	//--- Constants

	$constants=ExtGen::optional_element($data,'constants');
	if (is_null($constants)) $constants=array();
	if (!is_array($constants))
		throw new Exception("'constants' element must be an array");
	$this->constants=array();
	$class=$this->getclass('Constant');
	foreach($constants as $name => $def)
		{
		try	{
			if (array_key_exists($name,$this->constants))
				throw new Exception("Constant $name already defined");
			$this->constants[$name]=new $class($name,$def);
			} catch(Exception $e) {	throw new Exception($e->getMessage()." (while defining constant $name)"); }
		}
	} catch(Exception $e) { throw new Exception($e->getMessage().' (while reading global definition file)'); }

// Functions
// declared here because outside of 'global' exception catch block

$class=$this->getclass('Function');
$this->functions=array();
foreach($funcnames as $fname)
	{
	if (array_key_exists($fname,$this->functions))
		throw new Exception("Function $fname already defined");
	try	{
		$this->functions[$fname]=new $class($this,$fname);
		} catch(Exception $e) { throw new Exception($e->getMessage()." (while defining function $fname)"); }
	}

// Global data

$this->global_data=new ExtGenGlobalData($this);
}

//-----
// Write a file to dest dir

public function write_file($fname,$buf)
{
$this->renderer->reset_line_info($buf,$fname);
file_put_contents($this->dest_dir.'/'.$fname,$buf);
}

//============================================================================
} // End of class
?>
