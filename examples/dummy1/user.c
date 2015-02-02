
static char *subscriber_key(eg_string name, eg_size nlen, eg_int age, eg_size *keylenp)
{
eg_string p;

p=eg_eallocate(NULL,nlen+2);
p[0]=(char)age;
memmove(p+1,name,nlen+1);
(*keylenp)=(nlen+1);
return p;
}
