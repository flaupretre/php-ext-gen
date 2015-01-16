{% include 'common_header.twig.c' %}

/*=======================================================================*/
/* The main C source file for extension {{ name }} */
/*=======================================================================*/

{% include 'includes.twig.c' %}
{% include 'compatibility_macros.twig.h' %}
{% include 'extgen.twig.h' %}

#define PHP_{{ uname }}_VERSION "{{ version }}"
#define PHP_{{ uname }}_EXTNAME "{{ name }}"

zend_module_entry {{ name }}_module_entry;
#define phpext_{{ name }}_ptr &{{ name }}_module_entry

/*---------------------------------------------------------------*/
/* Module global data */

{% include 'module_globals.twig.c' %}

/*---------------------------------------------------------------*/
/* Fixed values */

{% include 'fixed_values.twig.c' %}

/*---------------------------------------------------------------*/
/*--- Header code */

{{ global_data.header }}

/*---------------------------------------------------------------*/
/* extgen functions */

{% include 'extgen.twig.c' %}

/*---------------------------------------------------------------*/
/* Extension functions */

{% for fname,function in functions %}
#include "{{ function.dest_filename }}"
{% endfor %}

/* Trailer code */

{{ global_data.trailer }}

/*---------------------------------------------------------------*/
/* PHP_Mxxx functions */

{% include 'php_mfunctions.twig.c' %}

/*---------------------------------------------------------------*/
/*-- Module definition --*/

{% include 'module_definition.twig.c' %}

/*====================================================================*/