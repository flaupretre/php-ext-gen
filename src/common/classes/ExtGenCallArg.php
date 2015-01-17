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

abstract class ExtGenCallArg
{

//----- Properties

private static $scalar_types=array('bool','int','double','string');

public $type;				// 'zval', 'array', scalar type, or 'mixed/<scalar_type>
public $byref;				// bool - pass by ref ?

public $zval=false;			// Arg is bare zval ?

public $accept_array=false;
public $accept_scalar=false;
public $scalar_type=false;	// Scalar type (string)
public $mixed=false;		// shortcut for (accept_array && accept_scalar)
public $nullok=false;		// accept null and transmit to function body ?

public $optional=false;		// bool
public $default=null;	// Default value: null or C string to insert in code

public $func;				// Function this arg belongs to

//---------

public function __construct($function,$def)
{
$this->func=$function;

$this->byref=ExtGen::optional_element($def,'byref');
if (is_null($this->byref)) $this->byref=false;

$nullok_supported=false;

$this->optional=ExtGen::optional_element($def,'optional');
if (is_null($this->optional)) $this->optional=false;

$default=ExtGen::optional_element($def,'default');

$this->type=$type=strtolower(ExtGen::element($def,'type'));

switch($type)
	{
	case 'zval':
		$this->zval=true;
		if (!is_null($default)) throw new Exception('A zval argument cannot have a default value');
		break;

	case'bool':
	case'int':
	case'double':
		$this->accept_scalar=true;
		$this->scalar_type=$type;
		break;

	case'string':
		$nullok_supported=true;
		$this->accept_scalar=true;
		$this->scalar_type='bool';
		break;

	case'array':
		$nullok_supported=true;
		$this->accept_array=true;
		if (!is_null($default)) throw new Exception('An array argument cannot have a default value');
		break;

	case'array|bool':
	case'array|int':
	case'array|double':
	case'array|string':
		$nullok_supported=true;
		$this->accept_array=$this->accept_scalar=true;
		$this->scalar_type=substr($type,6);
		break;

	default:
		throw new Exception("$type: Unsupported argument type");
	}

//---
	
$nullok=ExtGen::optional_element($def,'nullok');
if (is_null($nullok)) $nullok=false;
if ($nullok && (! $nullok_supported))
	throw new Exception("nullok can be set for this argument type ($type)");
$this->nullok=$nullok;

$this->default=$default;

}

//-----

protected function prepare()
{
}

//============================================================================
} // End of class
?>
