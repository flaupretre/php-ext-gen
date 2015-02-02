# An example of persistent resource creation

# resource[subscriber] new_subscriber(string name, int age)

arguments:
  name:
    type: string
  age:
    type: int

#------------------------------ Function code ------------------------------

{% block user_body %}
char *key;
eg_size keylen;
EG_RESOURCE_STRUCT(subscriber) *info;

/* Build key */

key=subscriber_key(EG_STRVAL(name),EG_STRLEN(name),EG_IVAL(age),&keylen);
info=EG_RESOURCE_PERSISTENT_FIND(subscriber,key,keylen);

if (!info) {
	/* If not yet registered, build structure... */
	/* In real world, this should be rerieved from a DBMS */
	info=EG_RESOURCE_ALLOC(subscriber,1); /* Persistent */
	info->name=eg_pduplicate(EG_STRVAL(name),EG_STRLEN(name)+1);
	info->nlen=EG_STRLEN(name);
	info->age=EG_IVAL(age);
	info->subscriber_id=next_subscriber_id++;
	time(&(info->creation_time));
	/* ... and register it (persistent) */
	EG_RESOURCE_PERSISTENT_REGISTER(info,key,keylen);
	}

/* Now, return a non-persistent reference */

EG_EALLOCATE(key,0);
EG_RETVAL_RESOURCE(EG_RESOURCE_REGISTER(info));

{% endblock user_body %}
