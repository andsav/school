#!/bin/sh

echo "\n"

rm submit/*.tar
for D in *; do
    if [ -d "${D}" ] && [ "${D}" != "submit" ]; then
        echo "Sploit ${D}:"
        cd ${D}
        tar cvf ../submit/sploit${D}.tar *
        cd ../ 
        echo "\n"
    fi
done

scp submit/*.tar a3savin@linux.student.cs.uwaterloo.ca:~/cs458/a2
scp submit/*.tar a3savin@ugster21.student.cs.uwaterloo.ca:~/a2

