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
public $parse_arguments;	// string

//---------

public function __construct($gen,$name)
{
parent::__construct($gen,$name);
}

//----------

public function generate()
{
$parse_format='';
$pargs=array();
$num=0;

foreach($this->arguments as $argname => $arg)
	{
	$arg->generate(); /* Prepare data */
	
	if ($num === $this->required_args_count) $parse_format .= '|';
	$parse_format .= $arg->parse_format;
	foreach($arg->parse_arguments as $parg)
		{
		$pargs[]='&'.$argname.$parg;
		}
	$num++;
	}

$this->parse_format=$parse_format;

$this->parse_arguments=trim(implode(', ',$pargs));
if ($this->parse_arguments !== '')
	$this->parse_arguments=', '.$this->parse_arguments;

//------ Rendering
//------ $this->body extends 'function.twig.c'. This way, every
//------ part of the function can be overriden by user code.

$buf=$this->gen->renderer->render_string($this->filename,$this->body
	,array('func' =>$this, 'global' => $this->gen));
$this->gen->write_file($this->dest_filename,$buf);
}

//============================================================================
} // End of class
?>
