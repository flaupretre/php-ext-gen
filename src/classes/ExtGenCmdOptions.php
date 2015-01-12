<?php
/**
* This class manages options for ExtGenCmd
*
* @copyright Francois Laupretre <TODO>
* @license TODO
* @category ExtGen
* @package ExtGen
*/
//============================================================================

//-------------

class ExtGenCmdOptions extends PHO_Options
{

// Short/long modifier args

protected $opt_modifiers=array(
	array('short' => 'v', 'long' => 'verbose', 'value' => false),
	array('short' => 'q', 'long' => 'quiet'  , 'value' => false),
	array('short' => 'f', 'long' => 'format'  , 'value' => true),
	array('short' => 's', 'long' => 'syntax'  , 'value' => true),
	);

// Option values

protected $options=array(
	'format' => NULL,
	'syntax' => 'yaml'
	);



//-----------------------

protected function process_option($opt,$arg)
{
switch($opt)
	{
	case 'v':
		PHO_Display::inc_verbose();
		break;

	case 'q':
		PHO_Display::dec_verbose();
		break;

	case 'f':
		$this->options['format']=$arg;
		break;

	case 's':
		$this->options['syntax']=$arg;
		break;
	}
}

//---------

//============================================================================
} // End of class
?>
