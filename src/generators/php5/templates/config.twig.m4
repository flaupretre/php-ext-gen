
PHP_ARG_ENABLE({{ name }}, whether to enable the {{ name }} extension,
[  --enable-{{ name }}    Enable the {{ name }} extension])

if test "$PHP_{{ uname }}" != "no"; then
  AC_DEFINE(HAVE_{{ uname }}, 1, [Whether you have the {{ name }} extension])
  PHP_NEW_EXTENSION({{ name }}, extgen_php_{{ name }}.c, $ext_shared)
fi
