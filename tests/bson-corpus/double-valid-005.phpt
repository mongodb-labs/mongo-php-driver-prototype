--TEST--
Double type: 1.2345678921232E+18
--DESCRIPTION--
Generated by scripts/convert-bson-corpus-tests.php

DO NOT EDIT THIS FILE
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$canonicalBson = hex2bin('100000000164002a1bf5f41022b14300');
$canonicalExtJson = '{"d" : {"$numberDouble": "1.2345678921232E+18"}}';
$relaxedExtJson = '{"d" : 1.2345678921232E+18}';

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
100000000164002a1bf5f41022b14300
{"d":{"$numberDouble":"1.2345678921232e+18"}}
{"d":1.2345678921232e+18}
100000000164002a1bf5f41022b14300
{"d":1.2345678921232e+18}
===DONE===