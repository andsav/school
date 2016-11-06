#!/bin/sh

echo "\n"

rm submit/*.tar
for D in *; do
    if [ -d "${D}" ] && [ "${D}" != "submit" ]; then
        echo "Sploit ${D}:"
        cd ${D}
        mv sploit.sh sploit${D}.sh
        tar cvf ../submit/sploit${D}.tar *
        mv sploit${D}.sh sploit.sh
        cd ../ 
        echo "\n"
    fi
done

scp submit/*.tar a3savin@linux.student.cs.uwaterloo.ca:~/cs458/a2
scp submit/*.tar a3savin@ugster21.student.cs.uwaterloo.ca:~/a2

