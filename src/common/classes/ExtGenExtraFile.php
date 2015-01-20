<?php
//=============================================================================
/**
* This class stores an extra file (a file to move from source to dest)
*
* @copyright Francois Laupretre <TODO>
* @license TODO
* @category php-ext-gen
* @package php-ext-gen
*/
//============================================================================

class ExtGenExtraFile
{

//----- Properties

public $name;
public $expand;	// whether to expand contents through twig

public $gen;	// Generator

//---------

public function __construct($gen,$name,$def)
{
if (!is_array($def)) throw new Exception('ExtraFile information must be an array');

$this->gen=$gen;

$this->name=$name;

$this->expand=ExtGen::optional_element($def,'expand');
if (is_null($this->expand)) $this->expand=false;
}

//---------

public function prepare()
{
}

//---------
// Create the file in dest dir.
// If expand is set, run contents through twig

public function generate()
{
$this->recursive_copy('', $this->name);
}

//---------
// Recursive copy with expansion if set

public function recursive_copy($rdir,$fname)
{
$src=$this->gen->source_dir.'/'.$rdir.$fname;
$dst=$this->gen->dest_dir.'/'.$rdir.$fname;

if (is_dir($src))
	{
    $dir = opendir($src);
    @mkdir($dst);
    while(($entry=readdir($dir))!==false)
		{
		if (($entry==='.')||($entry==='..')) continue;
		$this->recursive_copy($rdir.$fname.'/',$entry);
		}
	closedir($dir);
	}
else
	{
	$contents=$this->gen->file_contents($rdir.$fname);
	if ($this->expand)
		$contents=$this->gen->renderer->render_string($rdir.$fname,$contents);
	$this->gen->write_file($rdir.$fname,$contents);
	}
}

//============================================================================
} // End of class
?>
