all:
	cat README

push:
	rsync -r * a3savin@linux.student.cs.uwaterloo.ca:~/cs456/a3_r/ --delete

pull:
	rsync -r a3savin@linux.student.cs.uwaterloo.ca:~/cs456/a3_r/* ./

clean:
	pkill ruby

zip:
	tar czvf a3.tar.gz *.rb test.sh Makefile README
