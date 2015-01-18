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

//---------

public function __construct($function,$def)
{
parent::__construct($function,$def);
}

//----------
// Prepare data for code generation. When logic is complex, it is easier to
// do it in PHP than in twig.

public function prepare()
{
parent::prepare();

//----

}

//============================================================================
} // End of class
?>
