<?php
//=============================================================================
/**
* This class stores an extra file (a file to move from source to dest)
*
* @copyright Francois Laupretre <TODO>
* @license TODO
* @category php-ext-gen
* @package php-ext-gen
*/
//============================================================================

class ExtGenExtraFile
{

//----- Properties

public $name;
public $expand;	// whether to expand contents through twig
public $contents;

public $gen;	// Generator

//---------

public function __construct($gen,$name,$def)
{
if (!is_array($def)) throw new Exception('ExtraFile information must be an array');

$this->gen=$gen;

$this->name=$name;

$expand=ExtGen::optional_element($def,'expand');
if (is_null($expand)) $expand=false;
$this->expand=$expand;

$this->contents=$gen->file_contents($name);
}

//---------
// Create the file in dest dir. If expand is set, run it through twig before

public function generate()
{
$buf=$this->contents;
if ($this->expand) $buf=$this->gen->renderer->render_string($this->name,$buf);
$this->gen->write_file($this->name,$buf);
}

//============================================================================
} // End of class
?>
