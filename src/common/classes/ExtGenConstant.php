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

private static $supported_types=array('null','bool','int','double','string');

//----- Properties

private $gen;

public $name;
public $type;
public $value;

//---------

public function __construct($gen,$name,$def)
{
if (!is_array($def)) throw new Exception('Constant definition must be an array');

$this->name=$name;
$this->gen=$gen;

$type=ExtGen::element($def,'type');
if (array_search($type,self::$supported_types)===false)
	throw new Exception("$type: Unsupported type in constant definition");
$this->type=$type;

$this->value=$gen->compute_immediate_value($type,ExtGen::element($def,'value'),$name);
}

//---------

public function prepare()
{
}

//============================================================================
} // End of class
?>
