<?php
//=============================================================================
/**
* This class generates code for every PHP 5.x versions
*
* @copyright Francois Laupretre <TODO>
* @license TODO
* @category php-ext-gen
* @package php-ext-gen
*/
//============================================================================

class ExtGenGeneratorPHP5 extends ExtGenGenerator
{

//----- Properties

//---------
// Must not define a constructor !

public function init()
{
}

//---------
// Allows to read specific files from input dir (path in $this->source_dir).
// If this is not needed, remove this method.

public function read_source_data()
{
parent::read_source_data();

// Read flavor-specific data from input dir
}

//---------
// Output directory: ($this->dest_dir)

public function generate()
{
parent::generate();

//-----
// PHP5-specific stuff

foreach($this->functions as $func) $func->generate();

foreach($this->extra_files as $file) $file->generate();

$this->renderer->render_to_file('main.twig.c','extgen_php_'.$this->name.'.c');

$this->renderer->render_to_file('config.twig.m4','config.m4');
}

//============================================================================
} // End of class
?>
