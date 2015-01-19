#PHP prototype:
#    int newt_centered_window(int $width, int $height [, string $title=null ])
#
# No need for an explicit default value for 'title', as our default for string
# is a NULL pointer and this is the same in newt.
#---------------------------------------------------------------------------- 

arguments:
  width:
    type: int
  height:
    type: int
  title:
    type: string
    optional: true 

{% block user_body %}
EG_FUNC_RETURN_INT(newtCenteredWindow((long)(width->ival),(long)(height->ival)
	,title->sval));
{% endblock %}
