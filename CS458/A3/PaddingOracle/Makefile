test_decrypt:
	./decrypt jJAZKTtoHkl6/S+cps1kDOiRB/kggs/+7le4ESsLRCwznt2/hbuWA4Bgd/Y8jd1sVJyc0qZEPZmIIWwKfsNjBw==

test_encrypt:
	./encrypt '{"first_seen": 1277946061, "is_admin": false}'

ugster: 
	scp * a3savin@ugster21.student.cs.uwaterloo.ca:~/a3 && ssh a3savin@ugster21.student.cs.uwaterloo.ca

tar:
	tar cvf src.tar *.rb decrypt encrypt

submit:
	scp src.tar a3savin@linux.student.cs.uwaterloo.ca:~/cs458/a3
