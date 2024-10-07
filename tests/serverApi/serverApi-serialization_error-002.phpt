--TEST--
MongoDB\Driver\ServerApi unserialization errors (__serialize and __unserialize)
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    unserialize('O:24:"MongoDB\Driver\ServerApi":1:{s:7:"version";i:0;}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    unserialize('O:24:"MongoDB\Driver\ServerApi":2:{s:7:"version";s:1:"1";s:6:"strict";i:0;}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    unserialize('O:24:"MongoDB\Driver\ServerApi":2:{s:7:"version";s:1:"1";s:17:"deprecationErrors";i:0;}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\ServerApi initialization requires "version" field to be string
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\ServerApi initialization requires "strict" field to be bool or null
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\ServerApi initialization requires "deprecationErrors" field to be bool or null
===DONE===
