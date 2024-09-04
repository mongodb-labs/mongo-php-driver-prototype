--TEST--
Decimal128: [decq651] fold-down full sequence (Clamped)
--DESCRIPTION--
Generated by scripts/convert-bson-corpus-tests.php

DO NOT EDIT THIS FILE
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$canonicalBson = hex2bin('1800000013640000E1F50500000000000000000000FE5F00');
$canonicalExtJson = '{"d" : {"$numberDecimal" : "1.00000000E+6119"}}';
$degenerateExtJson = '{"d" : {"$numberDecimal" : "1E+6119"}}';

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
1800000013640000e1f50500000000000000000000fe5f00
{"d":{"$numberDecimal":"1.00000000E+6119"}}
1800000013640000e1f50500000000000000000000fe5f00
1800000013640000e1f50500000000000000000000fe5f00
===DONE===