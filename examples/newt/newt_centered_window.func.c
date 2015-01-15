# int newt_centered_window ( int $width , int $height [, string $title=null ] )

return_type: int

arguments:
  width:
    type: int
  height:
    type: int
  title:
    type: string
    nullok: true

{% block body %}

if (title_is_null) title=NULL;

retval=newtCenteredWindow((long)width, (long)height, title);

{% endblock %}
