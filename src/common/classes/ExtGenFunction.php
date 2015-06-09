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

public $name;
public $filename;
public $dest_filename;

public $arguments;
public $user_code;
public $required_args_count;	// Count of non-optionala args

public $gen;

//---------

abstract public function prepare();
abstract public function generate();

//---------

public function __construct($gen,$name)
{
\Phool\Display::trace("Defining function $name");

$this->gen=$gen;
$this->name=$name;
$this->filename='func.'.$name.'.c';
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

// Args

$argsdef=ExtGen::optional_element($def,'arguments');
if (is_null($argsdef)) $argsdef=array();
$this->arguments=array();
$this->required_args_count=0;
if (!is_array($argsdef)) throw new Exception('Function arguments must be an array');
$seen_optional=false;
$class=$this->gen->getclass('CallArg');
foreach($argsdef as $argname => $argdef)
	{
	\Phool\Display::trace("Defining argument $argname");
	if (array_key_exists($argname,$this->arguments))
		throw new Exception("$argname: Argument already defined");
	try { $obj=new $class($this,$argname,$argdef); }
	catch(Exception $e) { throw new Exception($e->getMessage()." (argument: $argname)"); }
	if ($obj->optional) $seen_optional=true;
	else
		{
		if ($seen_optional)
			throw new Exception('Cannot define a rquired argument after an optional one');
		$this->required_args_count++;
		}
	$this->arguments[$argname]=$obj;
	}
}

//============================================================================
} // End of class
?>
