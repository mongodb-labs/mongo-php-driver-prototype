--TEST--
Binary type: subtype 0x09 Vector FLOAT32
--DESCRIPTION--
Generated by scripts/convert-bson-corpus-tests.php

DO NOT EDIT THIS FILE
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$canonicalBson = hex2bin('170000000578000A0000000927000000FE420000E04000');
$canonicalExtJson = '{"x": {"$binary": {"base64": "JwAAAP5CAADgQA==", "subType": "09"}}}';

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
170000000578000a0000000927000000fe420000e04000
{"x":{"$binary":{"base64":"JwAAAP5CAADgQA==","subType":"09"}}}
170000000578000a0000000927000000fe420000e04000
===DONE===