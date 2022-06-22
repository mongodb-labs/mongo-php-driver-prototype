--TEST--
MongoDB\Driver\Manager::__construct(): incomplete auto encryption options
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongocrypt(); ?>
--FILE--
<?php
require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    [],
    ['keyVaultNamespace' => CSFLE_KEY_VAULT_NS],
];

foreach ($tests as $driverOptions) {
    echo throws(function() use ($driverOptions) {
        $manager = create_test_manager(null, [], ['autoEncryption' => $driverOptions]);
    }, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Key vault namespace option required

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
KMS providers option required

===DONE===
