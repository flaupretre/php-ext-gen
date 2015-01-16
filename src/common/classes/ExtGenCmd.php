<?php
//=============================================================================
/**
* This class handles CLI commands.
*
* @copyright Francois Laupretre <TODO>
* @license TODO
* @category php-ext-gen
* @package php-ext-gen
*/
//============================================================================

class ExtGenCmd
{
//---------

private static function error_abort($msg,$usage=true)
{
if ($usage) $msg .= " - Use 'help' command for syntax";
throw new Exception($msg);
}

//---------

private static function usage()
{
echo "
Available commands :

    - build [-o <format>] [-d] [<src-path>] [<dest-path>]

        Build ready-to-compile extension source files.

        <src-path> is a path to an existing directory containing the input
        definition files needed to build the extension source files. If not
		set, defaults to the current working directory.

        <dest-path> is the path where the output files will be
        created. If not set, the default is to generate files in
        a subdirectory named 'gen' under <src-path>.
        WARNING: *NEVER* use the same directory for source and destination (some
		files could be overwritten).

        Options :
            -o <format> : The PHP engine and version the extension is
			    generated for. The default is to generate for the PHP engine
				and version that is running this program.
				<format> must be provided as '<engine>:<version>'.
                Supported engines: php, hhvm
			-d: Activates debug mode

    - help : Display this message

Global options :

  -v : Increase verbose level (can be set more than once)
  -q : Decrease verbose level (can be set more than once)

More information at <TODO>\n\n";
}

//---------
// Main
// Options can be located before AND after the action keyword.

public static function run($args)
{
$op=new ExtGenCmdOptions;
$op->parse_all($args);
$action=(count($args)) ? array_shift($args) : 'help';

switch($action)
    {
    case 'build':
		$argc=count($args);
        if (count($args)>2) self::error_abort("build requires 1 or 2 arguments ($argc given)");
		$source_dir=(($argc > 0) ? $args[0] : '.');
		$dest_dir=(($argc>1) ? $args[1] : ($source_dir."/gen"));
		if ($argc>1) $dest_dir=$args[1];
		else
			{ // When dafault, create dir if not existing
			$dest_dir=$source_dir."/gen";
			if (!is_dir($dest_dir)) mkdir($dest_dir);
			if (!is_dir($dest_dir))
				throw new Exception("$dest_dir: cannot create directory");
			}
		if ($source_dir==$dest_dir)
			throw new Exception('Source and destination must be different');

		$worker=ExtGenGenerator::get_generator($op->option('format')
			,$source_dir,$dest_dir,$op->option('options'));
		$worker->read_source_data();
		$worker->output();
        break;

     case 'help':
        self::usage();
        break;

    default:
        self::error_abort("Unknown action: '$action'");
    }
}

//============================================================================
} // End of class
?>
