--TEST--
MongoDB\Driver\TopologyDescription debug output
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

class MySubscriber implements MongoDB\Driver\Monitoring\SDAMSubscriber
{
    public function topologyChanged(MongoDB\Driver\Monitoring\TopologyChangedEvent $event)
    {
        $topologyDescription = $event->getNewDescription();
        var_dump($topologyDescription);
    }
}

$subscriber = new MySubscriber;
MongoDB\Driver\Monitoring\addSubscriber($subscriber);

$command = new MongoDB\Driver\Command(['ping' => 1]);
$manager->executeCommand(DATABASE_NAME, $command);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\TopologyDescription)#%d (%d) {
  %a
}
object(MongoDB\Driver\TopologyDescription)#%d (%d) {
  %a
}
object(MongoDB\Driver\TopologyDescription)#%d (%d) {
  %a
}
===DONE===
