#proto: void newt_draw_root_text(int $left, int $top, string $text)

arguments:
  left:
    type: int
  top:
    type: int
  text:
    type: string

{% block user_body %}
newtDrawRootText((long)(left->ival),(long)(top->ival), text->sval);
{% endblock %}
