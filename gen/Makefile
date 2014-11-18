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

zip:
	zip submit.zip *.cc *h
