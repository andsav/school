all:
	@g++ *.cc -o test/gen
	@ls -l test/gen
gen:
	@cd test; \
	time cat ./input | ./scan | ./parse | ./gen
asm:
	@cd test; \
	cat input | ./scan | ./parse | ./gen | ./asm > 1.mips
	@ls -l test/1.mips

tst:
	@cd test; \
	cat input | ./scan | ./parse > fullinput; \
	mv fullinput ..
	@g++ -g *.cc

uw:
	@cd test; \
	cat ./input | ./scan | ./parse | ./gen > 1.mips; \
	scp 1.mips a3savin@linux.student.cs.uwaterloo.ca:~/cs241/test

zip:
	zip submit.zip *.cc *h
