--TEST--
MongoDB\BSON\PackedArray::get() index access returns BSON structures
--FILE--
<?php

$array = MongoDB\BSON\PackedArray::fromPHP([(object) ['foo' => 'bar']]);
$child = $array->get(0);
unset($array);
var_dump($child);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(8) "document"
  ["value"]=>
  object(MongoDB\BSON\Document)#%d (%d) {
    ["data"]=>
    string(24) "EgAAAAJmb28ABAAAAGJhcgAA"
  }
}
===DONE===
