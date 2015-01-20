/* ini entries */

PHP_INI_BEGIN()

{% for ini in ini_settings %}
	{{ ini.std_macro }}("{{ name }}.{{ ini.name }}", "{{ ini.default }}",
		{{ ini.access }}, {{ ini.cbk }}, {{ ini.name }}, zend_{{ name }}_globals,
		{{ name }}_globals)
{% endfor %}

PHP_INI_END()
