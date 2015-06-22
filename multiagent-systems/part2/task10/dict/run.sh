#!/bin/bash

if test -z "${RABBITMQ_PORT_5672_TCP_ADDR}" -o -z "${RABBITMQ_PORT_5672_TCP_PORT}"; then
    echo "You must link this container with rabbitmq first."
    exit 1
fi

export RABBITMQ_URL="amqp://${RABBITMQ_PORT_5672_TCP_ADDR}:${RABBITMQ_PORT_5672_TCP_PORT}"

while ! exec 6<>/dev/tcp/${RABBITMQ_PORT_5672_TCP_ADDR}/${RABBITMQ_PORT_5672_TCP_PORT}; do
    echo "$(date) - still trying to connect to rabbitmq at ${RABBITMQ_URL}..."
    sleep 1
done

exec 6>&-
exec 6<&-

nameko run --broker=amqp://guest:guest@$RABBITMQ_PORT_5672_TCP_ADDR:$RABBITMQ_PORT_5672_TCP_PORT/ $1
