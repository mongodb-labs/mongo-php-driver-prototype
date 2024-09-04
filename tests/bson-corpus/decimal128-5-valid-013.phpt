--TEST--
Decimal128: [decq130] fold-downs (more below) (Clamped)
--DESCRIPTION--
Generated by scripts/convert-bson-corpus-tests.php

DO NOT EDIT THIS FILE
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$canonicalBson = hex2bin('18000000136400000000807F1BCF85B27059C8A43CFEDF00');
$canonicalExtJson = '{"d" : {"$numberDecimal" : "-1.230000000000000000000000000000000E+6144"}}';
$degenerateExtJson = '{"d" : {"$numberDecimal" : "-1.23E+6144"}}';

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
18000000136400000000807f1bcf85b27059c8a43cfedf00
{"d":{"$numberDecimal":"-1.230000000000000000000000000000000E+6144"}}
18000000136400000000807f1bcf85b27059c8a43cfedf00
18000000136400000000807f1bcf85b27059c8a43cfedf00
===DONE===