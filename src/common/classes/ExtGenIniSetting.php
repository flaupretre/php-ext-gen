<?php
//=============================================================================
/**
* This class stores an ini entry
*
* @copyright Francois Laupretre <TODO>
* @license TODO
* @category php-ext-gen
* @package php-ext-gen
*/
//============================================================================

class ExtGenIniSetting
{

private static $info=array(
	 'bool'   => array('vartype' => 'eg_bool'   , 'def' => '0', 'modify' => 'OnUpdateBool')
	,'int'    => array('vartype' => 'eg_int'    , 'def' => '0', 'modify' => 'OnUpdateLong')
	,'float'  => array('vartype' => 'eg_float'  , 'def' => '0', 'modify' => 'OnUpdateDouble')
	,'string' => array('vartype' => 'eg_str_val', 'def' => '' , 'modify' => 'OnUpdateString')
	);

//----- Properties

public $gen;

public $name;		// Full name
public $type;		// {bool,int,float,string}

public $default;
public $access;		// String
public $vartype;	// String
public $modify;		// String - Modify callback
public $std_macro;	// STD_PHP_INI_xxx macro to use

//---------

public function __construct($gen,$name,$def)
{
$this->gen=$gen;
$this->name=$name;

$this->type=ExtGen::element($def,'type');
if (!array_key_exists($this->type,self::$info))
	throw new Exception($this->type.": Unsupported ini type");
$info=self::$info[$this->type];

$default=ExtGen::optional_element($def,'default');
if (is_null($default)) $default=self::$info['def'];
$this->default=$default;

$this->access=ExtGen::optional_element($def,'access');
if (is_null($this->access)) $this->access='PHP_INI_ALL';

$this->vartype=$info['vartype'];

$this->modify=ExtGen::optional_element($def,'modify');
if (is_null($this->modify)) $this->modify=$info['modify'];

$this->std_macro='STD_PHP_INI_'.(($this->type=='bool') ? 'ENTRY' : 'BOOLEAN');
}

//-----

protected function prepare()
{
}

//============================================================================
} // End of class
?>
