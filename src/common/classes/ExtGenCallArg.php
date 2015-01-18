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
//		- double
//		- string
//		- array
//		- array|null
//		- array|bool
//		- array|int
//		- array|double
//		- array|string
//============================================================================

class ExtGenCallArg
{

//----- Properties

private static $scalar_types=array('bool','int','double','string');

public $type;
public $byref;				// bool - pass by ref ?

public $zval=false;			// Arg is bare zval ?

public $accept_array=false;
public $accept_scalar=false;
public $scalar_type=false;	// Scalar type
public $mixed=false;		// shortcut for (accept_array && accept_scalar)

public $nullok=false;		// accept null as special case ? (array only)
public $nulltype;			// Type to set if we receive a null zval pointer

public $optional=false;		// bool
public $default=null;		// Default value: null or C string to insert in code

public $func;				// Function this arg belongs to

//---------

public function __construct($function,$def)
{
$this->func=$function;

$this->byref=ExtGen::optional_element($def,'byref');
if (is_null($this->byref)) $this->byref=false;

$this->optional=ExtGen::optional_element($def,'optional');
if (is_null($this->optional)) $this->optional=false;

$this->default=ExtGen::optional_element($def,'default');

$this->type=$type=strtolower(ExtGen::element($def,'type'));

switch($type)
	{
	case 'zval':
		$this->zval=true;
		if (!is_null($this->default)) throw new Exception('zval args cannot have a default value');
		break;

	case'bool':
	case'int':
	case'double':
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

	case'array|bool':
	case'array|int':
	case'array|double':
	case'array|string':
		$this->accept_array=$this->accept_scalar=true;
		$this->scalar_type=substr($type,6);
		break;

	default:
		throw new Exception("$type: Unsupported argument type");
	}

// Special syntax for default string ('"' chars are interpreted by parser)
// <string> means formula to insert without '<>'
// Other values are escaped as literal C strings and encapsulated in '"' chars

if ((!is_null($this->default))&&($this->scalar_type=='string'))
	{
	$len=strlen($this->default);
	if (($len>2) && (substr($this->default,0,1)=='<') && substr($this->default,$len-1,1)=='>')
		$this->default=substr($this->default,1,$len-2);
	else $this->default='"'.str_replace
		(array("\\","'",'"'),array("\\\\","\\'",'\\"'),$this->default).'"';
	}
}

//-----

protected function prepare()
{
}

//============================================================================
} // End of class
?>
