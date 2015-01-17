# int newt_centered_window ( int $width , int $height [, string $title=null ] )

arguments:
  width:
    type: int
  height:
    type: int
  title:
    type: string
    nullok: true

{% block body %}
ret.int_val=(eg_int)newtCenteredWindow((long)width, (long)height, title);
{% endblock %}
