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
parent::init_generate();

//-----
// PHP5-specific stuff



}

//============================================================================
} // End of class
?>
