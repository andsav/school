all:
	rustc -O router.rs -o router

push:
		rsync -r * a3savin@linux.student.cs.uwaterloo.ca:~/cs456/a3_r/ --delete

pull:
		rsync -r a3savin@linux.student.cs.uwaterloo.ca:~/cs456/a3_r/* ./

zip:
		tar cvfz A3.tgz *

