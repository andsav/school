#!/bin/sh

echo "\n"

for D in *; do
    if [ -d "${D}" ] && [ "${D}" != "submit" ]; then
        echo "Sploit ${D}:"
        tar cvf submit/sploit${D}.tar ${D}/* 
        echo "\n"
    fi
done

