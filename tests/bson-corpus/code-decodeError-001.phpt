--TEST--
Javascript Code: bad code string length: 0 (but no 0x00 either)
--DESCRIPTION--
Generated by scripts/convert-bson-corpus-tests.php

DO NOT EDIT THIS FILE
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$bson = hex2bin('0C0000000D61000000000000');

throws(function() use ($bson) {
    MongoDB\BSON\Document::fromBSON($bson)->toPHP();
}, 'MongoDB\Driver\Exception\UnexpectedValueException');

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
===DONE===