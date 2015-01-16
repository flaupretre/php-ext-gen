<?php
//=============================================================================
/**
* This class stores a PHP5 call argument
*
* @copyright Francois Laupretre <TODO>
* @license TODO
* @category php-ext-gen
* @package php-ext-gen
*/
//============================================================================

class ExtGenCallArgPHP5 extends ExtGenCallArg
{

//----- Properties

public $vars;	// array('type' => C type, 'ext' => C name extension)
public $parse_format;
public $parse_arguments;	// array of name extensions
public $receive_zval;
public $zval_type;

//---------

public function __construct($function,$def)
{
parent::__construct($function,$def);
}

//----------
// Prepare data for code generation. When logic is complex, it is easier to
// do it in PHP.

public function prepare()
{
parent::prepare();

//----

$vars=array();
$parse_format='';
$parse_arguments=array();
$receive_zval=false;
$scalar_format='';
$scalar_arguments=array();
$zval_type=0;

if ($this->accept_array)
	{
	$vars[]=array('eg_array *','_array');
	$zval_type='EG_IS_ARRAY';
	}
if ($this->accept_scalar)
	{
	$scalar_argument=array('');
	$zval_type='EG_IS_'.strtoupper($this->scalar_type);
	switch($this->scalar_type)
		{
		case 'bool':
			$vars[]=array('eg_bool','');
			$scalar_format='b';
			break;
		case 'int':
			$vars[]=array('long','');
			$scalar_format='l';
			break;
		case 'double':
			$vars[]=array('double','');
			$scalar_format='d';
			break;
		case 'string':
			$vars[]=array('char *','');
			$vars[]=array('int','_len');
			$scalar_format='s';
			$scalar_argument[]='_len';
			break;
		}
	}

if (($this->nullok)||($this->mixed)||($this->ref))
	{ // Must receive zval

	$this->receive_zval=true;
	$vars[]=array('zval *','_zval');
	$parse_format='z';
	$parse_arguments=array('_zval');
	if ($this->nullok)
		{
		$parse_format .= '!';
		$vars[]=array('int','_is_null');
		}
	if ($this->mixed)
		{
		$vars[]=array('int','_is_array');
		}
	}
else
	{
	$parse_format=$scalar_format;
	$parse_arguments=$scalar_arguments;
	}

$this->vars=array();
foreach($vars as $var)
	{
	list($type,$ext)=$var;
	$this->vars[]=array('type' => $type, 'ext' => $ext);
	}

$this->parse_format    =$parse_format;
$this->parse_arguments =$parse_arguments;
$this->receive_zval    =$receive_zval;
$this->zval_type=$zval_type;
}

//============================================================================
} // End of class
?>
