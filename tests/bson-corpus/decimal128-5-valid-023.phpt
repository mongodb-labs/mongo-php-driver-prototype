--TEST--
Decimal128: [decq190] underflow edge cases (Subnormal)
--DESCRIPTION--
Generated by scripts/convert-bson-corpus-tests.php

DO NOT EDIT THIS FILE
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$canonicalBson = hex2bin('180000001364000100000000000000000000000000008000');
$canonicalExtJson = '{"d" : {"$numberDecimal" : "-1E-6176"}}';
$degenerateExtJson = '{"d" : {"$numberDecimal" : "-1e-6176"}}';

// Canonical BSON -> BSON object -> Canonical BSON
echo bin2hex((string) MongoDB\BSON\Document::fromBSON($canonicalBson)), "\n";

// Canonical BSON -> BSON object -> Canonical extJSON
echo json_canonicalize(MongoDB\BSON\Document::fromBSON($canonicalBson)->toCanonicalExtendedJSON()), "\n";

// Canonical extJSON -> BSON object -> Canonical BSON
echo bin2hex((string) MongoDB\BSON\Document::fromJSON($canonicalExtJson)), "\n";

// Degenerate extJSON -> BSON object -> Canonical BSON
echo bin2hex((string) MongoDB\BSON\Document::fromJSON($degenerateExtJson)), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
180000001364000100000000000000000000000000008000
{"d":{"$numberDecimal":"-1E-6176"}}
180000001364000100000000000000000000000000008000
180000001364000100000000000000000000000000008000
===DONE===