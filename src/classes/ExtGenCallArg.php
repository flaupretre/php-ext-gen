<?php
//=============================================================================
/**
* This class stores a call argument
*
* @copyright Francois Laupretre <TODO>
* @license TODO
* @category php-ext-gen
* @package php-ext-gen
*/
//============================================================================

class ExtGenCallArg
{

//----- Properties

private static $scalar_types=array('bool','int','double','string');


public $ref;			// true -> pass by ref ; false -> by value
public $type;		 	// Scalar type
public $accept_array;
public $accept_scalar;
public $nullok;			// accept null and transmit to function body ?
public $optional;		// bool
public $default;		// Default value

//---------

public function __construct($def)
{
$this->ref=ExtGen::optional_element($def,'ref');
if (is_null($this->ref)) $this->ref=false;

$type=strtolower(ExtGen::element($def,'type'));
if (substr($type,0,6)=='mixed/')
	{
	$this->accept_array=true;
	$this->accept_scalar=true;
	$scalar_type=substr($type,6);
	self::check_scalar_type($scalar_type);
	$this->scalar_type=$scalar_type;
	}
elseif($type=='array')
	{
	$this->accept_array=true;
	$this->accept_scalar=false;
	}
else
	{
	$this->accept_array=false;
	$this->accept_scalar=true;
	self::check_scalar_type($type);
	$this->scalar_type=$type;
	}

$this->nullok=ExtGen::optional_element($def,'nullok');
if (is_null($this->nullok)) $this->nullok=false;

$default=ExtGen::optional_element($def,'default');
if (is_null($default))
	{
	$this->optional=false;
	}
else
	{
	$this->optional=true;
	if (strtolower($default==='null')) $default=null;
	if (($this->accept_array)&&(!is_null($default)))
		throw new Exception('When an arg can be an array, its default value must be null');
	$this->default=$default;
	}
}

//-----

private static function check_scalar_type($type)
{
if (array_search($type,self::$scalar_types)===false)
	throw new Exception("$type: Invalid argument scalar type");
}

//============================================================================
} // End of class
?>
