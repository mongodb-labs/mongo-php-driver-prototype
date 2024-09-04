--TEST--
Top-level document validity: Bad $oid (extra field)
--DESCRIPTION--
Generated by scripts/convert-bson-corpus-tests.php

DO NOT EDIT THIS FILE
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

throws(function() {
    MongoDB\BSON\Document::fromJSON('{"a" : {"$oid" : "56e1fc72e0c917e9c4714161", "unrelated": true}}');
}, 'MongoDB\Driver\Exception\UnexpectedValueException');

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
===DONE===