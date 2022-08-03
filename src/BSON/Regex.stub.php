<?php

/**
  * @generate-class-entries static
  * @generate-function-entries static
  */

namespace MongoDB\BSON;

final class Regex implements RegexInterface, \JsonSerializable, Type, \Serializable
{
    public final function __construct(string $pattern, string $flags = '') {}

    public final function getPattern(): string {}

    public final function getFlags(): string {}

    public final function __toString(): string {}

    public static function __set_state(array $properties): Regex {}

    final public function serialize(): string {}

    /** @param string $serialized */
    final public function unserialize($serialized): void {}

    final public function __unserialize(array $data): void {}

    final public function __serialize(): array {}

#if PHP_VERSION_ID >= 80000
    final public function jsonSerialize(): mixed {}
#else
    /** @return mixed */
    final public function jsonSerialize() {}
#endif
}
