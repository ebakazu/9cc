CFLAGS=-std=c11 -g -static
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
IMAGE_TAG = compilerbook

9cc: $(OBJS)
	$(CC) -o 9cc $(OBJS) $(LDFLAGS)

$(OBJS): 9cc.h

build:
	docker build -t $(IMAGE_TAG) .

exec:
	docker run --rm -it -v $(PWD):/9cc $(IMAGE_TAG)

exec-test:
	docker run --rm -v $(PWD):/9cc -w /9cc $(IMAGE_TAG) /9cc/test.sh

clean:
	rm -f 9cc *.o *~ tmp*

.PHONY: test clean exec exec-test build
