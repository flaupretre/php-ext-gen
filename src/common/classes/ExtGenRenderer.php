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

private static $env_options=array(
	'strict_variables' => true,
	'cache' => false,
	'autoescape' => false);

//-----

public function __construct($gen)
{
$this->gen=$gen;
}

//-----

private function default_loader()
{
$src_base=dirname(dirname(dirname(__FILE__)));

return new Twig_Loader_Filesystem(array(
	$src_base.'/generators/'.$this->gen->generator.'/templates',
	$src_base.'/common/templates'));
}

//-----

private function twig()
{
if (!$this->twig_env)
	{
	$this->twig_env=new Twig_Environment($this->default_loader()
		,self::$env_options);
	}
return $this->twig_env;
}

//-----
// Unfortunately, twig does not support using an object as context, so we need
// to convert to an array of object properties. The default context is the
// generator object.

public function fix_context($context)
{
if (is_null($context)) $context=$this->gen;

if (is_object($context))
	{
	$c=array();
	foreach(get_object_vars($context) as $prop => $val)
		$c[$prop]=$context->$prop;
	$context=$c;
	}

return $context;
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
return $this->twig()->render($template,$this->fix_context($context))."\n";
}

//-----
// $emul_fname is a virtual file name used for error messages
// Need to chain loaders as we also want access to file system templates

public function render_string($emul_fname,$buf,$context=null)
{
$loader1=new Twig_Loader_Array(array($emul_fname => $buf));
$loader2=$this->default_loader();
$loader=new Twig_Loader_Chain(array($loader1,$loader2));

$twig=new Twig_Environment($loader,self::$env_options);

return $twig->render($emul_fname,$this->fix_context($context))."\n";
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
