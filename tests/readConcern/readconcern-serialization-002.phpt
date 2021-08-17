--TEST--
MongoDB\Driver\ReadConcern serialization (__serialize and __unserialize)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '7.4.0'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    new MongoDB\Driver\ReadConcern(),
    new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::LINEARIZABLE),
    new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::LOCAL),
    new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::MAJORITY),
    new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::AVAILABLE),
    new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::SNAPSHOT),
];

foreach ($tests as $test) {
    var_dump($test);
    echo $s = serialize($test), "\n";
    var_dump(unserialize($s));
    echo "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\ReadConcern)#%d (%d) {
}
O:26:"MongoDB\Driver\ReadConcern":0:{}
object(MongoDB\Driver\ReadConcern)#%d (%d) {
}

object(MongoDB\Driver\ReadConcern)#%d (%d) {
  ["level"]=>
  string(12) "linearizable"
}
O:26:"MongoDB\Driver\ReadConcern":1:{s:5:"level";s:12:"linearizable";}
object(MongoDB\Driver\ReadConcern)#%d (%d) {
  ["level"]=>
  string(12) "linearizable"
}

object(MongoDB\Driver\ReadConcern)#%d (%d) {
  ["level"]=>
  string(5) "local"
}
O:26:"MongoDB\Driver\ReadConcern":1:{s:5:"level";s:5:"local";}
object(MongoDB\Driver\ReadConcern)#%d (%d) {
  ["level"]=>
  string(5) "local"
}

object(MongoDB\Driver\ReadConcern)#%d (%d) {
  ["level"]=>
  string(8) "majority"
}
O:26:"MongoDB\Driver\ReadConcern":1:{s:5:"level";s:8:"majority";}
object(MongoDB\Driver\ReadConcern)#%d (%d) {
  ["level"]=>
  string(8) "majority"
}

object(MongoDB\Driver\ReadConcern)#%d (%d) {
  ["level"]=>
  string(9) "available"
}
O:26:"MongoDB\Driver\ReadConcern":1:{s:5:"level";s:9:"available";}
object(MongoDB\Driver\ReadConcern)#%d (%d) {
  ["level"]=>
  string(9) "available"
}

object(MongoDB\Driver\ReadConcern)#%d (%d) {
  ["level"]=>
  string(8) "snapshot"
}
O:26:"MongoDB\Driver\ReadConcern":1:{s:5:"level";s:8:"snapshot";}
object(MongoDB\Driver\ReadConcern)#%d (%d) {
  ["level"]=>
  string(8) "snapshot"
}

===DONE===
