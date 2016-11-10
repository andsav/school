all:
	sbt compile

push:
	rsync -r src file.txt Makefile sender.sh receiver.sh a3savin@linux.student.cs.uwaterloo.ca:~/cs456/a2_s/ --delete

pull:
	rsync -r a3savin@linux.student.cs.uwaterloo.ca:~/cs456/a2_s/src/* ./src/

zip:
	tar cvfz A2.tgz README Makefile src/ *.sh

testN:
	./nEmulator 9991 127.0.0.1 9994 9993 127.0.0.1 9992 1 0 1

testR:
	./receiver.sh 127.0.0.1 9993 9994 file2.txt

testS:
	./sender.sh 127.0.0.1 9991 9992 file.txt
