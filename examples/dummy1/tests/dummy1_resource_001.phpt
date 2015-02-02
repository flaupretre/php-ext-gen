--TEST--
Check resource registration/access/delete
--FILE--
<?php
echo "-- Registering 1\n";

$rs1=new_subscriber('John Doe',51);
var_dump($rs1);

echo "-- Registering 2\n";

$rs2=new_subscriber('Albert Smith',37);
var_dump($rs2);

echo "-- Display info on user 1\n";

subscriber_info($rs1);

echo "-- Display info on user 2\n";

subscriber_info($rs2);

echo "-- Delete resource 1\n";

var_dump(subscriber_delete($rs1));

echo "-- Display info on invalid resource (1)\n";

$e=false;
try {
subscriber_info($rs1);
} catch(Exception $e) { $e=true; }
echo "Exception: ".($e ? 'Yes' : 'No')."\n";

echo "-- Attempt to delete invalid resource (1)\n";

$e=false;
try {
subscriber_delete($rs1);
} catch(Exception $e) { $e=true; }
echo "Exception: ".($e ? 'Yes' : 'No')."\n";

echo "-- Display info on user 2\n";

subscriber_info($rs2);

echo "-- Registering 1 again - must use same number\n";

$rs1=new_subscriber('John Doe',51);
var_dump($rs1);

echo "-- Display info on user 1\n";

subscriber_info($rs1);

echo "-- Registering duplicate 1\n";

$rs11=new_subscriber('John Doe',51);
var_dump($rs11);

echo "-- Display info on user 11\n";

subscriber_info($rs11);

echo "-- Delete resource 1\n";

var_dump(subscriber_delete($rs1));

echo "-- Display info on user 11\n";

subscriber_info($rs11);

?>
===DONE===
--EXPECTF--
-- Registering 1
resource(%d) of type (Subscriber info)
-- Registering 2
resource(%d) of type (Subscriber info)
-- Display info on user 1
Name: John Doe
Name length: 8
Age: 51
Subscription ID: 12345
Registered on: %s
-- Display info on user 2
Name: Albert Smith
Name length: 12
Age: 37
Subscription ID: 12346
Registered on: %s
-- Delete resource 1
bool(true)
-- Display info on invalid resource (1)
Exception: Yes
-- Attempt to delete invalid resource (1)
Exception: Yes
-- Display info on user 2
Name: Albert Smith
Name length: 12
Age: 37
Subscription ID: 12346
Registered on: %s
-- Registering 1 again - must use same number
resource(%d) of type (Subscriber info)
-- Display info on user 1
Name: John Doe
Name length: 8
Age: 51
Subscription ID: 12345
Registered on: %s
-- Registering duplicate 1
resource(%d) of type (Subscriber info)
-- Display info on user 11
Name: John Doe
Name length: 8
Age: 51
Subscription ID: 12345
Registered on: %s
-- Delete resource 1
bool(true)
-- Display info on user 11
Name: John Doe
Name length: 8
Age: 51
Subscription ID: 12345
Registered on: %s
===DONE===
