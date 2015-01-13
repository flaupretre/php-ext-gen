<?php
//=============================================================================
/**
* This class stores a constant definition
*
* @copyright Francois Laupretre <TODO>
* @license TODO
* @category php-ext-gen
* @package php-ext-gen
*/
//============================================================================

class ExtGenConstant
{

private static $supported_types=array('null','bool','int','double','string','array');

//----- Properties

public $type;
public $computed=false;	// if true, value is a formula, not a literal string.
public $value=null;

//---------

public function __construct($name,$def)
{
if (!is_array($def)) throw new Exception('Constant definition must be an array');

$type=strtolower(ExtGen::element($def,'type'));
if (array_search($type,self::$supported_types)===false)
	throw new Exception("$type: Unsupported type in constant definition");
$this->type=$type;

$this->computed=false;
if (($type==='null')
	&& ((ExtGen::optional_element($def,'computed')!==null)
		|| (ExtGen::optional_element($def,'value')!==null)))
	{
	throw new Exception('computed and value params must not be set in a null constant definition');
	}

$computed=ExtGen::optional_element($def,'computed');
switch($computed)
	{
	case 'use-name':
		$this->computed=true;
		$this->value=$name;
		break;
	case true:
	case 'true':
		$this->computed=true;
		$this->value=ExtGen::element($def,'value');
		break;
	case false:
	case null:
	case 'false':
		$this->computed=false;
		$this->value=ExtGen::element($def,'value');
		break;
	default:
		throw new Exception("$computed: Invalid value for 'computed' param");
	}
}

//============================================================================
} // End of class
?>
