<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\Driver\Monitoring;

final class CommandSucceededEvent
{
    final private function __construct() {}

    final public function getCommandName(): string {}

    final public function getDatabaseName(): string {}

    final public function getDurationMicros(): int {}

    final public function getHost(): string {}

    final public function getOperationId(): string {}

    final public function getPort(): int {}

    final public function getReply(): object {}

    final public function getRequestId(): string {}

    /** @deprecated */
    final public function getServer(): \MongoDB\Driver\Server {}

    final public function getServiceId(): ?\MongoDB\BSON\ObjectId {}

    final public function getServerConnectionId(): ?int {}
}
