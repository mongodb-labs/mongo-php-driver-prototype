--TEST--
Decimal128: [decq842] VG testcase
--DESCRIPTION--
Generated by scripts/convert-bson-corpus-tests.php

DO NOT EDIT THIS FILE
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$canonicalBson = hex2bin('180000001364000000FED83F4E7C9FE4E269E38A5BCD1700');
$canonicalExtJson = '{"d" : {"$numberDecimal" : "7.049000000000010795488000000000000E-3097"}}';

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
180000001364000000fed83f4e7c9fe4e269e38a5bcd1700
{"d":{"$numberDecimal":"7.049000000000010795488000000000000E-3097"}}
180000001364000000fed83f4e7c9fe4e269e38a5bcd1700
===DONE===