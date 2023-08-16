--TEST--
MongoDB\BSON\Javascript::__set_state()
--FILE--
<?php

$tests = [
    ['function foo(bar) { return bar; }', null],
    ['function foo(bar) { return bar; }', []],
    ['function foo() { return foo; }', ['foo' => 42]],
    ['function foo() { return id; }', ['id' => new MongoDB\BSON\ObjectId('53e2a1c40640fd72175d4603')]],
];

foreach ($tests as $test) {
    list($code, $scope) = $test;

    var_export(MongoDB\BSON\Javascript::__set_state([
        'code' => $code,
        'scope' => $scope,
    ]));
    echo "\n\n";
}

// Test with missing scope field
var_export(MongoDB\BSON\Javascript::__set_state([
    'code' => 'function foo(bar) { return bar; }',
]));
echo "\n\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%r\\?%rMongoDB\BSON\Javascript::__set_state(array(
   'code' => 'function foo(bar) { return bar; }',
   'scope' => NULL,
))

%r\\?%rMongoDB\BSON\Javascript::__set_state(array(
   'code' => 'function foo(bar) { return bar; }',
   'scope' =>%w
  %r\(object\)? %rarray(
  %r\)?%r),
))

%r\\?%rMongoDB\BSON\Javascript::__set_state(array(
   'code' => 'function foo() { return foo; }',
   'scope' =>%w
  %r\(object\)? %rarray(
     'foo' => 42,
  %r\)?%r),
))

%r\\?%rMongoDB\BSON\Javascript::__set_state(array(
   'code' => 'function foo() { return id; }',
   'scope' =>%w
  %r\(object\)? %rarray(
     'id' =>%w
    %r\\?%rMongoDB\BSON\ObjectId::__set_state(array(
       'oid' => '53e2a1c40640fd72175d4603',
    )),
  %r\)?%r),
))

%r\\?%rMongoDB\BSON\Javascript::__set_state(array(
   'code' => 'function foo(bar) { return bar; }',
   'scope' => NULL,
))

===DONE===
