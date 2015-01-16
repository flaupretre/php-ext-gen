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

public function output()
{
$this->prepare();
$this->generate();
}

//---------

public function prepare()
{
parent::prepare();

foreach($this->functions as $func) $func->prepare();
foreach($this->extra_files as $file) $file->prepare();
}

//---------
// Output directory: ($this->dest_dir)

public function generate()
{
PHO_Display::info('Generating...');

foreach($this->functions as $func) $func->generate();
foreach($this->extra_files as $file) $file->generate();

$this->renderer->render_to_file('main.twig.c','extgen_php_'.$this->name.'.c');

$this->renderer->render_to_file('config.twig.m4','config.m4');
}

//============================================================================
} // End of class
?>
