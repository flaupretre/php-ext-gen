/* Defines function {{ fname }} */

PHP_FUNCTION({{ fname }})
{


/*---- Argument parsing */





/*---- Function body */

/* Enclose in braces because of argument declarations */
{
#line {{ function.body_start_line }} "{{ fname }}.func.c"
{{ function.body }}

/*#reset_line_info*/{# This resets the line nummber and filename #}
}

/*---- Return values */





}
