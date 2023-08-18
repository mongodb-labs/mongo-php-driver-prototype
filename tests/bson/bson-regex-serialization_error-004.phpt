--TEST--
MongoDB\BSON\Regex unserialization does not allow pattern or flags to contain null bytes (__serialize and __unserialize)
--DESCRIPTION--
BSON Corpus spec prose test #1
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    unserialize('O:18:"MongoDB\BSON\Regex":2:{s:7:"pattern";s:7:"regexp' . "\0" . '";s:5:"flags";s:1:"i";}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    unserialize('O:18:"MongoDB\BSON\Regex":2:{s:7:"pattern";s:6:"regexp";s:5:"flags";s:2:"i' . "\0" . '";}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Pattern cannot contain null bytes
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Flags cannot contain null bytes
===DONE===
