--TEST--
Int32 type: MaxValue
--DESCRIPTION--
Generated by scripts/convert-bson-corpus-tests.php

DO NOT EDIT THIS FILE
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$canonicalBson = hex2bin('0C000000106900FFFFFF7F00');
$canonicalExtJson = '{"i" : {"$numberInt": "2147483647"}}';
$relaxedExtJson = '{"i" : 2147483647}';

// Canonical BSON -> BSON object -> Canonical BSON
echo bin2hex((string) MongoDB\BSON\Document::fromBSON($canonicalBson)), "\n";

// Canonical BSON -> BSON object -> Canonical extJSON
echo json_canonicalize(MongoDB\BSON\Document::fromBSON($canonicalBson)->toCanonicalExtendedJSON()), "\n";

// Canonical BSON -> BSON object -> Relaxed extJSON
echo json_canonicalize(MongoDB\BSON\Document::fromBSON($canonicalBson)->toRelaxedExtendedJSON()), "\n";

// Canonical extJSON -> BSON object -> Canonical BSON
echo bin2hex((string) MongoDB\BSON\Document::fromJSON($canonicalExtJson)), "\n";

// Relaxed extJSON -> BSON object -> Relaxed extJSON
echo json_canonicalize(MongoDB\BSON\Document::fromJSON($relaxedExtJson)->toRelaxedExtendedJSON()), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
0c000000106900ffffff7f00
{"i":{"$numberInt":"2147483647"}}
{"i":2147483647}
0c000000106900ffffff7f00
{"i":2147483647}
===DONE===