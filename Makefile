CFLAGS=-std=c11 -g -static
IMAGE_TAG = compilerbook

build:
	docker build -t $(IMAGE_TAG) .

exec:
	docker run --rm -it -v $(HOME)/9cc:/9cc $(IMAGE_TAG)

exec-test:
	docker run --rm -v $(HOME)/9cc:/9cc -w /9cc $(IMAGE_TAG) make test

9cc: 9cc.c

test: 9cc
	./test.sh

clean:
	rm -f 9cc *.o *~ tmp*

.PHONY: test clean exec exec-test build
