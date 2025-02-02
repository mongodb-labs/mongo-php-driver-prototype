--TEST--
MongoDB\Driver\Server::executeQuery() with conflicting read preference for secondary
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_replica_set(); ?>
<?php skip_if_no_secondary(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

$primaryRp = new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::PRIMARY);
$primary = $manager->selectServer($primaryRp);

// Count all data-bearing members to use for the write concern
$dataBearingNodes = count(array_filter($manager->getServers(), function (MongoDB\Driver\Server $server) {
    return ($server->isPrimary() || $server->isSecondary());
}));

$bulk = new \MongoDB\Driver\BulkWrite;
$bulk->insert(['_id' => 1, 'x' => 1]);
$primary->executeBulkWrite(NS, $bulk, ['writeConcern' => new MongoDB\Driver\WriteConcern($dataBearingNodes)]);

$secondaryRp = new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY);
$secondary = $manager->selectServer($secondaryRp);

/* Note: this is testing that the read preference (even a conflicting one) has
 * no effect when directly querying a server, since the secondaryOk flag is always
 * set for hinted queries. */
$cursor = $secondary->executeQuery(NS, new MongoDB\Driver\Query(['x' => 1]), ['readPreference' => $primaryRp]);

var_dump($cursor->toArray());

?>
===DONE===
<?php exit(0); ?>(
--EXPECTF--
array(1) {
  [0]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    int(1)
    ["x"]=>
    int(1)
  }
}
===DONE===
