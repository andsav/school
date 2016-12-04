#!/bin/bash

while true; do
    curl -c test.in http://localhost:4555

    cat test.in | perl -wlne 'print $1 if /\"(.+)\"/' >> test.out

    ./decrypt $(cat test.in | perl -wlne 'print $1 if /\"(.+)\"/') >> test.out
done
