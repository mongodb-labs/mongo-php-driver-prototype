--TEST--
Decimal128: [basx677] Zeros
--DESCRIPTION--
Generated by scripts/convert-bson-corpus-tests.php

DO NOT EDIT THIS FILE
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$canonicalBson = hex2bin('1800000013640000000000000000000000000000002E3000');
$canonicalExtJson = '{"d" : {"$numberDecimal" : "0E-9"}}';
$degenerateExtJson = '{"d" : {"$numberDecimal" : "0.00E-7"}}';

// Canonical BSON -> Native -> Canonical BSON
echo bin2hex(fromPHP(toPHP($canonicalBson))), "\n";

// Canonical BSON -> Canonical extJSON
echo json_canonicalize(toCanonicalExtendedJSON($canonicalBson)), "\n";

// Canonical extJSON -> Canonical BSON
echo bin2hex(fromJSON($canonicalExtJson)), "\n";

// Degenerate extJSON -> Canonical BSON
echo bin2hex(fromJSON($degenerateExtJson)), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
1800000013640000000000000000000000000000002e3000
{"d":{"$numberDecimal":"0E-9"}}
1800000013640000000000000000000000000000002e3000
1800000013640000000000000000000000000000002e3000
===DONE===