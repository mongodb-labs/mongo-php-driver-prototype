--TEST--
Javascript Code with Scope: Non-empty code string, empty scope
--DESCRIPTION--
Generated by scripts/convert-bson-corpus-tests.php

DO NOT EDIT THIS FILE
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$canonicalBson = hex2bin('1A0000000F610012000000050000006162636400050000000000');
$canonicalExtJson = '{"a" : {"$code" : "abcd", "$scope" : {}}}';

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
1a0000000f610012000000050000006162636400050000000000
{"a":{"$code":"abcd","$scope":{}}}
1a0000000f610012000000050000006162636400050000000000
===DONE===