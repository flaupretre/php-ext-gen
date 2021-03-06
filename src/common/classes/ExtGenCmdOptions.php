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

class ExtGenCmdOptions extends \Phool\Options\Base
{

// Short/long modifier args

protected $opt_modifiers=array(
	array('short' => 'v', 'long' => 'verbose' , 'value' => false),
	array('short' => 'q', 'long' => 'quiet'   , 'value' => false),
	array('short' => 'f', 'long' => 'format'  , 'value' => true),
	array('short' => 's', 'long' => 'syntax'  , 'value' => true),
	array('short' => 'd', 'long' => 'debug'   , 'value' => false),
	);

// Option values

protected $options=array(
	'format' => NULL,
	'syntax' => 'yaml',
	'options' => 0
	);



//-----------------------

protected function processOption($opt,$arg)
{
switch($opt)
	{
	case 'v':
		\Phool\Display::incVerbose();
		break;

	case 'q':
		\Phool\Display::decVerbose();
		break;

	case 'f':
		$this->options['format']=$arg;
		break;

	case 's':
		$this->options['syntax']=$arg;
		break;

	case 'd':
		$this->options['options'] |= ExtGenOptions::DEBUG;
		break;
	}
}

//---------

//============================================================================
} // End of class
?>
