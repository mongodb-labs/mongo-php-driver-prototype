--TEST--
MongoDB\Driver\ClientEncryption::createDataKey() "keyAltNames" option
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongocrypt(); ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(CSFLE_KEY_VAULT_DATABASE_NAME, CSFLE_KEY_VAULT_COLLECTION_NAME);
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

$clientEncryption = $manager->createClientEncryption([
  'keyVaultNamespace' => CSFLE_KEY_VAULT_NS,
  'kmsProviders' => ['local' => ['key' => new MongoDB\BSON\Binary(CSFLE_LOCAL_KEY, 0)]],
]);

$keyId = $clientEncryption->createDataKey('local', ['keyAltNames' => ['foo', 'bar']]);
$key = $clientEncryption->getKey($keyId);

var_dump($key->_id == $keyId);
var_dump(in_array('foo', $key->keyAltNames));
var_dump(in_array('bar', $key->keyAltNames));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(true)
bool(true)
===DONE===
