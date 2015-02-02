# We decide to cache subscriber information

display_string: "Subscriber info"

{% block user_resource_struct %}
eg_string name;
eg_size nlen;
eg_int age;
eg_int subscriber_id;
time_t creation_time;
{% endblock %}

{# ----- Destructor ------ #}

{% block user_resource_dtor %}
EG_PEALLOCATE(ptr->name,0,persistent);
{% endblock %}
