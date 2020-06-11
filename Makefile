CFLAGS=-std=c11 -g -static

exec:
	docker run --rm -it -v $(HOME)/9cc:/9cc compilerbook

exec-test:
	docker run --rm -v $(HOME)/9cc:/9cc -w /9cc compilerbook make test

9cc: 9cc.c

test: 9cc
	./test.sh

clean:
	rm -f 9cc *.o *~ tmp*

.PHONY: test clean exec exec-test
