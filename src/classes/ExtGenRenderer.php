<?php
//=============================================================================
/**
* This class renders output files using twig
*
* @copyright Francois Laupretre <TODO>
* @license TODO
* @category php-ext-gen
* @package php-ext-gen
*/
//============================================================================

class ExtGenRenderer
{

//----- Properties

public $gen;	// The ExtGenGenerator object we are bound to

private $twig_env=null;	// Used through the twig() method only

//-----

public function __construct($gen)
{
$this->gen=$gen;
}

//-----
// Template paths: when explicitely common, prefix with 'common/'
//                 when common is just default -> no prefix

private function twig()
{
if (!$this->twig_env)
	{
	$tbase=dirname(dirname(__FILE__)).'/templates';

	$loader=new Twig_Loader_Filesystem(array(
		$tbase.'/generators/'.$this->gen->generator['name'],
		$tbase,
		$tbase.'/common'));
	$this->twig_env=new Twig_Environment($loader,array(
		'strict_variables' => true,
		'cache' => false,
		'autoescape' => false));
	}
return $this->twig_env;
}

//-----

public function default_context()
{
return $this->gen->default_context();
}

//-----

public function render_to_file($template,$target_file,$context=null)
{
$buf=$this->render($template,$context);
$this->gen->write_file($target_file,$buf);
}

//-----

public function render($template,$context=null)
{
if (is_null($context)) $context=$this->default_context();

return $this->twig()->render($template,$context)."\n";
}

//-----
// $emul_fname is a virtual file name used for error messages

public function render_string($emul_fname,$buf,$context=null)
{
if (is_null($context)) $context=$this->default_context();

$loader=new Twig_Loader_Array(array($emul_fname => $buf));
$twig=new Twig_Environment($loader);
return $twig->render($emul_fname,$context)."\n";
}

//-----

public function reset_line_info(&$buf,$fname)
{
$a=explode("\n",$buf);
foreach($a as $num => $line)
	{
	if (strpos($line,'/*#reset_line_info*/')===0)
		$a[$num]="#line ".($num+1).' "'.$fname.'"';
	}
$buf=implode("\n",$a);
}

//============================================================================
} // End of class
?>
