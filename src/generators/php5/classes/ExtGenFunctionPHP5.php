<?php
//=============================================================================
/**
* This class stores a PHP5 function
*
* @copyright Francois Laupretre <TODO>
* @license TODO
* @category php-ext-gen
* @package php-ext-gen
*/
//============================================================================

class ExtGenFunctionPHP5 extends ExtGenFunction
{

//----- Properties

public $parse_format;		// string

//---------

public function __construct($gen,$name)
{
parent::__construct($gen,$name);
}

//----------

public function prepare()
{
$parse_format='';
$num=0;

foreach($this->arguments as $argname => $arg)
	{
	$arg->prepare(); /* Prepare data */
	
	if ($num === $this->required_args_count) $parse_format .= '|';
	$parse_format .= 'z';
	if ($arg->optional) $parse_format.='!';
	$num++;
	}

$this->parse_format=$parse_format;
}

//----------
// user code is a a twig template containing blocks. Whe have it extend
// 'function.twig.c'. This way, every part of the function code can be overriden
// by user code.

public function generate()
{
$user_code="{% extends 'function.twig.c' %}\n".$this->user_code;

$buf=$this->gen->renderer->render_string($this->filename,$user_code
	,array('func' =>$this, 'global' => $this->gen));
$this->gen->file_write($this->dest_filename,$buf);
}

//============================================================================
} // End of class
?>
