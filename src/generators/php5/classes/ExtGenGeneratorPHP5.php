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
foreach($this->constants as $const) $const->prepare();
foreach($this->extra_files as $file) $file->prepare();
}

//---------
// Output directory: ($this->dest_dir)

public function generate()
{
\Phool\Display::info('Generating...');

/* Transfer common files */

$this->renderer->render_to_file('extgen.compatibility.twig.h','extgen.compatibility.h');
$this->renderer->render_to_file('extgen.definitions.twig.h','extgen.definitions.h');
$this->renderer->render_to_file('extgen.includes.twig.h','extgen.includes.h');
$this->renderer->render_to_file('extgen.lib.twig.c','extgen.lib.c');

/* Generate function files */

foreach($this->functions as $func) $func->generate();

/* Generate resource files */

foreach($this->resources as $resource) $resource->generate();

/* Expand and copy extra files */

foreach($this->extra_files as $file) $file->generate();

/* Build and write main source file */

$buf="{% extends 'main.twig.c' %}\n".$this->optional_file_contents('global.twig.c');
$res=$this->renderer->render_string('global.twig.c',$buf);
$this->file_write('extgen_php_'.$this->name.'.c',$res);

/* Build and write autoconf-related stuff */

if ($this->src_file_exists('config.m4')) $this->file_copy('','config.m4',true);
else $this->renderer->render_to_file('config.twig.m4','config.m4');

/* Copy tests, if dir exists */

$this->file_copy('','tests',true,true);
}

//============================================================================
} // End of class
?>
