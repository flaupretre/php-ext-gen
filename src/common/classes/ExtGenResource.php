<?php
//=============================================================================
/**
* This class stores a resource type
*
* @copyright Francois Laupretre <TODO>
* @license TODO
* @category php-ext-gen
* @package php-ext-gen
*/
//============================================================================

abstract class ExtGenResource
{

//----- Properties

public $name;
public $filename;
public $dest_filename;

public $display_string;	// String to display in var_dump and reflection

public $user_code;

public $gen;

//---------

abstract public function prepare();
abstract public function generate();

//---------

public function __construct($gen,$name)
{
PHO_Display::trace("Defining resource $name");

$this->gen=$gen;
$this->name=$name;
$this->filename='resource.'.$name.'.c';
$this->dest_filename='extgen_'.$this->filename;

$buf=$gen->file_contents($this->filename);

$pos=strpos($buf,"{%");
if ($pos===false)
	{
	$meta=$buf;
	$user_code='';
	}
else
	{
	$meta=substr($buf,0,$pos);
	$user_code=substr($buf,$pos);
	}
$this->user_code=trim($user_code)."\n";
$def=$gen->parser->decode($meta);

$s=ExtGen::optional_element($def,'display_string');
if (is_null($s)) $s=$name;
$this->display_string=$s;
}

//============================================================================
} // End of class
?>
