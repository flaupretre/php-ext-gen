<?php
//=============================================================================
/**
* This class is a Json parser.
*
* @copyright Francois Laupretre <TODO>
* @license TODO
* @category php-ext-gen
* @package php-ext-gen
*/
//============================================================================

class ExtGenMetaParserJson extends ExtGenMetaParser
{

//----- Properties

private $json;

public $file_extension='json';
public $syntax='json';

//---------

public function __construct($gen)
{
parent::__construct();

$this->json=new Services_JSON();
}

//-----

public function decode($buf)
{
return $this->check_decode_result($buf,$this->json->decode($buf));
}

//============================================================================
} // End of class
?>
