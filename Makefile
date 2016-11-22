TARGETS = testalloc
OBJS = testalloc.o
CXX := g++
LD := g++
CFLAGS = -lpthread -lm
LDFLAGS = 

TIMER_DIR = ./rdtscp_timer
EXEC = timer testalloc

all: $(EXEC)

timer:
	cd $(TIMER_DIR); $(MAKE)

testalloc: testalloc.cc timer
	g++ $(TIMER_DIR)/timing.o testalloc.cc -std=c++0x -lrt -o testalloc $(CFLAGS)

get_output:
	gcc -g $@.c -o $@

bench: $(EXEC) get_output
	for i in 1 2 4 8 16 32 64; do \
	    for j in `seq 2 1 22`; do \
		./testalloc $$i $$j; \
	    done \
	done
	./get_output

clean:
	rm -rf testalloc
	rm -rf get_output
	rm -rf output_frag output_nofrag output 
	rm -rf output_1 output_2 output_4 output_8 output_16 output_32 output_64
