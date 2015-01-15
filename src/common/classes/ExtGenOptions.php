<?php
//=============================================================================
/**
* This class defines generator options
*
* @copyright Francois Laupretre <TODO>
* @license TODO
* @category php-ext-gen
* @package php-ext-gen
*/
//============================================================================

class ExtGenOptions
{
const DEBUG=1;

//-----------

public $debug;

//-----------

public function __construct($value)
{
$this->debug=$value & self::DEBUG;
}

//============================================================================
} // End of class
?>
