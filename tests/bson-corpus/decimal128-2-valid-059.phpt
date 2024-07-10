--TEST--
Decimal128: [decq624] fold-down full sequence
--DESCRIPTION--
Generated by scripts/convert-bson-corpus-tests.php

DO NOT EDIT THIS FILE
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$canonicalBson = hex2bin('18000000136400000040B2BAC9E0191E0200000000FE5F00');
$canonicalExtJson = '{"d" : {"$numberDecimal" : "1.0000000000000000000000E+6133"}}';

// Canonical BSON -> BSON object -> Canonical BSON
echo bin2hex((string) MongoDB\BSON\Document::fromBSON($canonicalBson)), "\n";

// Canonical BSON -> BSON object -> Canonical extJSON
echo json_canonicalize(MongoDB\BSON\Document::fromBSON($canonicalBson)->toCanonicalExtendedJSON()), "\n";

// Canonical extJSON -> BSON object -> Canonical BSON
echo bin2hex((string) MongoDB\BSON\Document::fromJSON($canonicalExtJson)), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
18000000136400000040b2bac9e0191e0200000000fe5f00
{"d":{"$numberDecimal":"1.0000000000000000000000E+6133"}}
18000000136400000040b2bac9e0191e0200000000fe5f00
===DONE===