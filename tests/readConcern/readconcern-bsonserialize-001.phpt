--TEST--
MongoDB\Driver\ReadConcern::bsonSerialize()
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
    echo toJSON(fromPHP($test)), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
{ }
{ "level" : "linearizable" }
{ "level" : "local" }
{ "level" : "majority" }
{ "level" : "available" }
{ "level" : "snapshot" }
===DONE===
