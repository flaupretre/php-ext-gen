# Delete a previously registered subscriber (don't delete persistent data)

# void subscriber_delete(resource res)

arguments:
  res:
    type: resource
    resource_type: subscriber

#------------------------------ Function code ------------------------------

{% block user_body %}
ZEND_RESULT_CODE status;

if (!EG_ARG_RESOURCE_IS_VALID(res)) EG_EXCEPTION_ABORT("Invalid resource");

status=EG_RESOURCE_DELETE(EG_RESOURCE(res));

EG_RETVAL_BOOL(status==SUCCESS);
{% endblock user_body %}
