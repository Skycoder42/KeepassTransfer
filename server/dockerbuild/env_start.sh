#!/bin/sh

echo "[server]" > $KPTSERVER_CONFIG_PATH
echo "host=$KPTSERVER_HOST" >> $KPTSERVER_CONFIG_PATH
echo "port=$KPTSERVER_PORT" >> $KPTSERVER_CONFIG_PATH
echo "local=$KPTSERVER_LOCAL" >> $KPTSERVER_CONFIG_PATH
echo "timeout=$KPTSERVER_TIMEOUT" >> $KPTSERVER_CONFIG_PATH

exec /usr/bin/kptransfer-server
