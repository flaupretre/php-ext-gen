
{% include 'function_declarations.twig.c' %}

/*----------------------*/

zend_module_entry {{ name }}_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	PHP_{{ uname }}_EXTNAME,
	extgen_functions,
	PHP_MINIT({{ name }}),
	PHP_MSHUTDOWN({{ name }}),
	PHP_RINIT({{ name }}),
	PHP_RSHUTDOWN({{ name }}),
	PHP_MINFO({{ name }}),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_{{ uname }}_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};
