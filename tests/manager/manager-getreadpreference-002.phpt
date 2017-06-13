--TEST--
MongoDB\Driver\Manager::getReadPreference()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$test = array(STANDALONE, array('readPreference' => new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_PRIMARY)));

list($uri, $options) = $test;

try {
    $manager = new MongoDB\Driver\Manager($uri, $options);
} catch (\MongoDB\Driver\Exception\InvalidArgumentException $exception) {
    echo $exception->getMessage() . "\n";
    exit;
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Unsupported readPreference type, a string must be given
===DONE===
