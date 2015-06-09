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
public $ini_settings; // Ini settings. Array of ExtGenIniSetting instances
public $resources; 	// Resource types. Array of ExtGenResource instances

//---------

abstract public function output();

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
$gen_class='ExtGenGenerator'.strtoupper($generator);
if (!class_exists($gen_class,1))
	throw new Exception("$generator: this generator is not available yet");

\Phool\Display::trace("Using $generator generator");

$obj=new $gen_class();

\Phool\Display::trace("Generating for (engine=$engine ; version=$version)");

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

protected function prepare()
{
$this->global_data->prepare();
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
$path=$this->src_path($rpath);

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

public function read_source_data()
{
\Phool\Display::info('Reading source files...');

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

	$this->version=ExtGen::element($data,'version');

	//--- Flags

	$flags=ExtGen::optional_element($data,'flags');
	if (is_null($flags)) $flags=array();

	if (!array_key_exists('minfo_displays_ini',$flags)) $flags['minfo_displays_ini']=false;
	
	$this->flags=$flags;

	//--- autoconf-related information

	$def=ExtGen::optional_element($data,'autoconf');
	if (is_null($def)) $def=array();
	$class=$this->getclass('Autoconf');
	$this->autoconf=new $class($def);

	//--- INI settings

	$ini=ExtGen::optional_element($data,'ini');
	if (is_null($ini)) $ini=array();
	if (!is_array($ini))
		throw new Exception("'ini/settings' element must be an array");
	$this->ini=array();
	$class=$this->getclass('IniSetting');
	foreach($ini as $name => $def)
		{
		try	{
			if (array_key_exists($name,$this->ini))
				throw new Exception("$name: Ini setting already defined");
			$this->ini_settings[$name]=new $class($this,$name,$def);
			} catch(Exception $e) {	throw new Exception($e->getMessage()." (while defining '$name' ini setting)"); }
		}

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

	//--- Get resource names

	$rscnames=ExtGen::optional_element($data,'resources');
	if (is_null($rscnames)) $rscnames=array();
	if (!is_array($rscnames))
		throw new Exception("'resources' element must be an array");

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
			$this->constants[$name]=new $class($this,$name,$def);
			} catch(Exception $e) {	throw new Exception($e->getMessage()." (while defining constant $name)"); }
		}
	} catch(Exception $e) { throw new Exception($e->getMessage().' (while reading global definition file)'); }

// Functions
// declared here because outside of 'global' exception catch block

$class=$this->getclass('Function');
$this->functions=array();
foreach($funcnames as $name)
	{
	if (array_key_exists($name,$this->functions))
		throw new Exception("Function $name already defined");
	try	{
		$this->functions[$name]=new $class($this,$name);
		} catch(Exception $e) { throw new Exception($e->getMessage()." (while defining function $name)"); }
	}

// Resources

$class=$this->getclass('Resource');
$this->resources=array();
foreach($rscnames as $name)
	{
	if (array_key_exists($name,$this->resources))
		throw new Exception("Resource $name already defined");
	try	{
		$this->resources[$name]=new $class($this,$name);
		} catch(Exception $e) { throw new Exception($e->getMessage()." (while defining resource $name)"); }
	}

// Global data

$this->global_data=new ExtGenGlobalData($this);
}

//-----
// Convert metadat immediate value to a string to insert in C code
//
// Special syntax for string ('"' chars are interpreted by parser)
// <string> means formula to insert without '<>'
// Other values are escaped as literal C strings and encapsulated in '"' chars
//
// Special value: '<name>' means to insert the variable name in code.

public function compute_immediate_value($type,$value,$name)
{
if ($value==='<name>') return $name;

if ($type=='string')
	{
	$len=strlen($value);
	if (($len>2) && (substr($value,0,1)=='<') && substr($value,$len-1,1)=='>')
		$value=substr($value,1,$len-2);
	else $value='"'.str_replace
		(array("\\","'",'"'),array("\\\\","\\'",'\\"'),$value).'"';
	}

return $value;
}

//-----

public function src_path($fname)
{
return $this->source_dir.'/'.$fname;
}

//-----

public function src_file_exists($rpath)
{
return file_exists($this->src_path($rpath));
}

//-----

public function dst_path($fname)
{
return $this->dest_dir.'/'.$fname;
}

//-----
// Write a file to dest dir

public function file_write($fname,$buf)
{
$this->renderer->reset_line_info($buf,$fname);
file_put_contents($this->dst_path($fname),$buf);
}

//---------
// Recursive copy with expansion if set

public function file_copy($rdir,$fname,$expand,$optional=false)
{
if ((!$this->src_file_exists($rdir.$fname)) && $optional) return;

$src=$this->src_path($rdir.$fname);
$dst=$this->dst_path($rdir.$fname);

if (is_dir($src))
	{
    $dir = opendir($src);
    while(($entry=readdir($dir))!==false)
		{
		if (($entry==='.')||($entry==='..')) continue;
		$this->file_copy($rdir.$fname.'/',$entry,$expand);
		}
	closedir($dir);
	}
else
	{
	$contents=$this->file_contents($rdir.$fname);
	if ($expand) $contents=$this->renderer->render_string($rdir.$fname,$contents);
	$ddir=dirname($dst);
	if (!is_dir($ddir)) @mkdir($ddir,0755,true);
	$this->file_write($rdir.$fname,$contents);
	}
}

//============================================================================
} // End of class
?>
