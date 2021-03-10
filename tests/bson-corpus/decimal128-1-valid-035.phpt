--TEST--
Decimal128: Scientific - Largest
--DESCRIPTION--
Generated by scripts/convert-bson-corpus-tests.php

DO NOT EDIT THIS FILE
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$canonicalBson = hex2bin('18000000136400FFFFFFFF638E8D37C087ADBE09EDFF5F00');
$canonicalExtJson = '{"d" : {"$numberDecimal" : "9.999999999999999999999999999999999E+6144"}}';

// Canonical BSON -> Native -> Canonical BSON
echo bin2hex(fromPHP(toPHP($canonicalBson))), "\n";

// Canonical BSON -> Canonical extJSON
echo json_canonicalize(toCanonicalExtendedJSON($canonicalBson)), "\n";

// Canonical extJSON -> Canonical BSON
echo bin2hex(fromJSON($canonicalExtJson)), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
18000000136400ffffffff638e8d37c087adbe09edff5f00
{"d":{"$numberDecimal":"9.999999999999999999999999999999999E+6144"}}
18000000136400ffffffff638e8d37c087adbe09edff5f00
===DONE===