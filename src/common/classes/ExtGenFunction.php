<?php
//=============================================================================
/**
* This class stores a PHP function
*
* @copyright Francois Laupretre <TODO>
* @license TODO
* @category php-ext-gen
* @package php-ext-gen
*/
//============================================================================

abstract class ExtGenFunction
{

//----- Properties

private static $return_types=array('null','bool','int','double','string','array');

public $name;
public $filename;
public $dest_filename;

public $arguments;
public $return_type;
public $user_code;
public $required_args_count;	// Count of non-optionala args
public $args_count;

public $gen;

//---------

abstract public function prepare();
abstract public function generate();

//---------

public function __construct($gen,$name)
{
PHO_Display::trace("Defining function $name");

$this->gen=$gen;
$this->name=$name;
$this->filename=$name.'.func.c';
$this->dest_filename='extgen_'.$this->filename;

$buf=$gen->file_contents($this->filename);

$pos=strpos($buf,"{%");
if ($pos===false)
	{
	$meta=$buf;
	$user_code='';
	}
else
	{
	$meta=substr($buf,0,$pos);
	$user_code=substr($buf,$pos);
	}
$this->user_code=trim($user_code)."\n";
$def=$gen->parser->decode($meta);

$return_type=ExtGen::optional_element($def,'return_type');
if (is_null($return_type)) $return_type='null';
$return_type=strtolower($return_type);
if (array_search($return_type,self::$return_types)===false)
	throw new Exception("$return_type: Unsupported function return type");
$this->return_type=$return_type;

// Args

$argsdef=ExtGen::optional_element($def,'arguments');
if (is_null($argsdef)) $argsdef=array();
$this->arguments=array();
$this->args_count=$this->required_args_count=0;
if (!is_array($argsdef)) throw new Exception('Function arguments must be an array');
$seen_optional=false;
$class=$this->gen->getclass('CallArg');
foreach($argsdef as $argname => $argdef)
	{
	PHO_Display::trace("Defining argument $argname");
	if (array_key_exists($argname,$this->arguments))
		throw new Exception("$argname: Argument already defined");
	$obj=new $class($this,$argdef);
	if ($obj->optional) $seen_optional=true;
	else
		{
		if ($seen_optional)
			throw new Exception('Cannot define a rquired argument after an optional one');
		$this->required_args_count++;
		}
	$this->arguments[$argname]=$obj;
	$this->args_count++;
	}
}

//============================================================================
} // End of class
?>
