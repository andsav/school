#!/usr/bin/env bash

for i in $(seq 1 5); do
    ruby router.rb $i 127.0.0.1 9999 999$i &
done
