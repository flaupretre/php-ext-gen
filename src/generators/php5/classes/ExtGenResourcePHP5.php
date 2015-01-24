<?php
//=============================================================================
/**
* This class stores a PHP5 resource
*
* @copyright Francois Laupretre <TODO>
* @license TODO
* @category php-ext-gen
* @package php-ext-gen
*/
//============================================================================

class ExtGenResourcePHP5 extends ExtGenResource
{

//----- Properties

//---------

public function __construct($gen,$name)
{
parent::__construct($gen,$name);
}

//----------

public function prepare()
{
}

//----------
// user code is a a twig template containing blocks. We have it extend
// 'resource.twig.c'.

public function generate()
{
$user_code="{% extends 'resource.twig.c' %}\n".$this->user_code;

$buf=$this->gen->renderer->render_string($this->filename,$user_code
	,array('resource' =>$this, 'global' => $this->gen));
$this->gen->file_write($this->dest_filename,$buf);
}

//============================================================================
} // End of class
?>
