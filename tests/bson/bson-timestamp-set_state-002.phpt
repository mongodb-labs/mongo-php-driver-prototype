--TEST--
MongoDB\BSON\Timestamp::__set_state() (64-bit)
--SKIPIF--
<?php if (8 !== PHP_INT_SIZE) { die('skip Only for 64-bit platform'); } ?>
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$tests = [
    [4294967295, 0],
    [0, 4294967295],
];

foreach ($tests as $test) {
    list($increment, $timestamp) = $test;

    var_export(MongoDB\BSON\Timestamp::__set_state([
        'increment' => $increment,
        'timestamp' => $timestamp,
    ]));
    echo "\n\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
MongoDB\BSON\Timestamp::__set_state(array(
   'increment' => 4294967295,
   'timestamp' => 0,
))

MongoDB\BSON\Timestamp::__set_state(array(
   'increment' => 0,
   'timestamp' => 4294967295,
))

===DONE===
