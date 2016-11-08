all:
	sbt compile

submit:
	rsync -r src Makefile sender.sh receiver.sh a3savin@linux.student.cs.uwaterloo.ca:~/cs456/a2_s/ --delete
