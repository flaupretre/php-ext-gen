<?php
//=============================================================================
/**
* This class is a Yaml parser.
*
* @copyright Francois Laupretre <TODO>
* @license TODO
* @category php-ext-gen
* @package php-ext-gen
*/
//============================================================================

class ExtGenMetaParserYaml extends ExtGenMetaParser
{
//----- Properties

public $file_extension='yml';
public $syntax='yaml';


//---------

public function __construct($gen)
{
parent::__construct($gen);
}

//-----
// Parse metadata

public function decode($buf)
{
return $this->check_decode_result($buf,\Symfony\Component\Yaml\Yaml::parse($buf));
}

//============================================================================
} // End of class
?>
