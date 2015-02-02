# Display info about a previously registered subscriber

# void subscriber_info(resource res)

arguments:
  res:
    type: resource
    resource_type: subscriber

#------------------------------ Function code ------------------------------

{% block user_body %}
EG_RESOURCE_STRUCT(subscriber) *info;

if (!EG_ARG_RESOURCE_IS_VALID(res)) EG_EXCEPTION_ABORT("Invalid resource");

info=(EG_RESOURCE_STRUCT(subscriber) *)EG_RES_PTR(res);
php_printf("Name: " EG_FORMAT_STRING "\n",info->name);
php_printf("Name length: " EG_FORMAT_SIZE "\n",info->nlen);
php_printf("Age: " EG_FORMAT_INT "\n",info->age);
php_printf("Subscription ID: " EG_FORMAT_INT "\n",info->subscriber_id);
php_printf("Registered on: %s",ctime(&(info->creation_time)));

{% endblock user_body %}
