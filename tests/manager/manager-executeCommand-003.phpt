--TEST--
MongoDB\Driver\Manager::executeCommand() takes a read preference as legacy option
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_replica_set(); ?>
<?php skip_if_no_secondary(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

$primary   = new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::PRIMARY);
$secondary = new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY);

echo "Testing primary:\n";
$command = new MongoDB\Driver\Command(['ping' => 1]);
$cursor = $manager->executeCommand(DATABASE_NAME, $command, $primary);

echo "is_primary: ", $cursor->getServer()->isPrimary() ? 'true' : 'false', "\n";
echo "is_secondary: ", $cursor->getServer()->isSecondary() ? 'true' : 'false', "\n\n";

echo "Testing secondary:\n";
$command = new MongoDB\Driver\Command(['ping' => 1]);
$cursor = $manager->executeCommand(DATABASE_NAME, $command, $secondary);

echo "is_primary: ", $cursor->getServer()->isPrimary() ? 'true' : 'false', "\n";
echo "is_secondary: ", $cursor->getServer()->isSecondary() ? 'true' : 'false', "\n\n";
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Testing primary:

Deprecated: MongoDB\Driver\Manager::executeCommand(): Passing the "readPreference" option directly is deprecated and will be removed in ext-mongodb 2.0%s
is_primary: true
is_secondary: false

Testing secondary:

Deprecated: MongoDB\Driver\Manager::executeCommand(): Passing the "readPreference" option directly is deprecated and will be removed in ext-mongodb 2.0%s
is_primary: false
is_secondary: true

===DONE===
