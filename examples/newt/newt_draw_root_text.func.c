#proto: void newt_draw_root_text(int $left, int $top, string $text)

arguments:
  left:
    type: int
  top:
    type: int
  text:
    type: string

{% block body %}
newtDrawRootText ((long)left, (long)top, text);
{% endblock %}
