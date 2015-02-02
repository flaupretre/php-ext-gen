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
// Supported types :
//		- zval
//		- bool
//		- int
//		- float
//		- string
//		- array
//		- array|null
//		- array|bool
//		- array|int
//		- array|float
//		- array|string
//============================================================================

class ExtGenCallArg
{

//----- Properties

//TODO: resources

private static $scalar_types=array('bool','int','float','string','resource');

public $name;
public $type;
public $byref;				// bool - pass by ref ?

public $zval=false;			// Arg is bare zval ?

public $accept_array=false;
public $accept_scalar=false;
public $accept_resource=false;
public $scalar_type=false;	// Scalar type
public $resource_type=null;
public $mixed=false;		// shortcut for (accept_array && accept_scalar)

public $nullok=false;		// accept null as special case ? (array only)
public $nulltype;			// Type to set if we receive a null zval pointer

public $optional=false;		// bool
public $default=null;		// Default value: null or C string to insert in code

public $func;				// Function this arg belongs to

//---------

public function __construct($function,$name,$def)
{
$this->name=$name;
$this->func=$function;

$this->byref=ExtGen::optional_element($def,'byref');
if (is_null($this->byref)) $this->byref=false;

$this->optional=ExtGen::optional_element($def,'optional');
if (is_null($this->optional)) $this->optional=false;

$this->default=ExtGen::optional_element($def,'default');

$this->type=$type=ExtGen::element($def,'type');

$this->resource_type=ExtGen::optional_element($def,'resource_type');

switch($type)
	{
	case 'zval':
		$this->zval=true;
		if (!is_null($this->default)) throw new Exception('zval args cannot have a default value');
		break;

	case 'resource':
		$this->accept_resource=true;
		// No break here
	case'bool':
	case'int':
	case'float':
	case'string':
		$this->accept_scalar=true;
		$this->scalar_type=$type;
		break;

	case'array|null':
		$this->nullok=true;
		$this->nulltype='array';
	case'array':
		$this->accept_array=true;
		if (!is_null($this->default)) throw new Exception('An array argument cannot have a default value');
		break;

	case 'resource':
		$this->accept_resource=true;
		// No break here
	case'array|bool':
	case'array|int':
	case'array|float':
	case'array|string':
		$this->accept_array=$this->accept_scalar=true;
		$this->scalar_type=substr($type,6);
		break;

	default:
		throw new Exception("$type: Unsupported argument type");
	}

if (!is_null($this->default))
	$this->default=$this->func->gen->compute_immediate_value($this->scalar_type
		,$this->default,$name);

if ((!($this->accept_resource)) && (!is_null($this->resource_type)))
	throw new Exception('Cannot set resource type on non resource');
}

//-----

protected function prepare()
{
}

//============================================================================
} // End of class
?>
