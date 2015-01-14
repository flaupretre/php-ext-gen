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

class ExtGenFunction
{

//----- Properties

private static $return_types=array('null','bool','int','double','string','array');

public $name;
public $arguments;
public $return_type;
public $body;
public $body_start_line; // Line number where function body starts

//---------

public function __construct($name,$buf,$parser)
{
PHO_Display::trace("Defining function $name");

$pos=strpos($buf,"\n%%");
if ($pos===false) throw new Exception('%% not found in function definition file');

$this->body=substr($buf,$pos+3);
$header=substr($buf,0,$pos);
$a=count_chars($header);
$this->body_start_line=$a[10]+2; // Count of LF chars in header gives start line

$def=$parser->decode($header);

$return_type=ExtGen::optional_element($def,'return_type');
if (is_null($return_type)) $return_type='null';
$return_type=strtolower($return_type);
if (array_search($return_type,self::$return_types)===false)
	throw new Exception("$return_type: Unsupported function return type");
$this->return_type=$return_type;

// Args

$argsdef=ExtGen::optional_element($def,'arguments');
$this->arguments=array();
if (!is_null($argsdef)) 
	{
	if (!is_array($argsdef)) throw new Exception('Function arguments must be an array');
	$seen_optional=false;
	foreach($argsdef as $argname => $argdef)
		{
		PHO_Display::trace("Defining argument $argname");
		if (array_key_exists($argname,$this->arguments))
			throw new Exception("$argname: Argument already defined");
		$obj=new ExtGenCallArg($argdef);
		if (($seen_optional)&&(!$obj->optional))
			throw new Exception('Cannot define a mandatory argument after an optional one');
		if ($obj->optional) $seen_optional=true;
		$this->arguments[$argname]=$obj;
		}
	}
}

//------------
// Expand function body through renderer

public function expand($renderer)
{
$this->body=$renderer->render_string($this->name.' function body',$this->body);
}

//============================================================================
} // End of class
?>
