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

  - build -f <format> [-s <syntax>] <src-path> <dest-path>

        Build ready-to-compile extension source files.

        <src-path> is a path to an existing directory containing the input
        definition files needed to build the extension source files.

        <dest-path> is the path where the resulting source files will be
        created. For security reasons, if <dest-path> is an already-existing
		file or directory, the command is aborted.

        Options :
            -f <format> : Output format. The PHP flavor we are building for.
                Possible values:
                - php5: Any 5.x version of the default PHP interpreter(available
                        from www.php.net).
                - php7: version 7 of the default interpreter.
                - hhvm: The HHVM environment.
			-s <syntax> : Metadata syntax
				Possible values:
				- yaml (default)
				- json

    - help

        Display this message

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
        if (count($args)!=2) self::error_abort('build requires 2 arguments');
        list($source_dir,$dest_dir)=$args;
		$format=$op->option('format');
		if (is_null($format)) self::error_abort('no format provided');
		ExtGen::build($source_dir,$dest_dir,$format,$op->option['syntax']);
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
