--TEST--
Decimal128: [decq609] fold-down full sequence (Clamped)
--DESCRIPTION--
Generated by scripts/convert-bson-corpus-tests.php

DO NOT EDIT THIS FILE
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$canonicalBson = hex2bin('18000000136400000000A0CA17726DAE0F1E430100FE5F00');
$canonicalExtJson = '{"d" : {"$numberDecimal" : "1.00000000000000000000000000000E+6140"}}';
$degenerateExtJson = '{"d" : {"$numberDecimal" : "1E+6140"}}';

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
18000000136400000000a0ca17726dae0f1e430100fe5f00
{"d":{"$numberDecimal":"1.00000000000000000000000000000E+6140"}}
18000000136400000000a0ca17726dae0f1e430100fe5f00
18000000136400000000a0ca17726dae0f1e430100fe5f00
===DONE===