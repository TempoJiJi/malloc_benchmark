TARGETS = testalloc
OBJS = testalloc.o
CXX := g++
LD := g++
CFLAGS = -lpthread -lm
LDFLAGS = 

TIMER_DIR = ./rdtscp_timer
EXEC = timer testalloc bench

all: $(EXEC)

timer:
	cd $(TIMER_DIR); $(MAKE)

testalloc: testalloc.cc timer
	g++ $(TIMER_DIR)/timing.o testalloc.cc -std=c++0x -lrt -o testalloc $(CFLAGS)

get_output:
	gcc -g $@.c -o $@

bench:
	g++ $(TIMER_DIR)/timing.o bench.cc -std=c++0x -lrt -o bench $(CFLAGS)

benchmark: $(EXEC)
	for i in 1 2 4 8 16 32 64; do \
	    ./bench $$i; \
	done

# For different malloc implement,
#   Please Change the ./get_output argv
# @Ptmalloc = 0, @Supermalloc = 1, @Allocator = 2
sizebench: $(EXEC) get_output
	for i in 1 2 4 8 16 32 64; do \
	    for j in 2 4 5 6 8 9 10 12 20 22; do \
		./testalloc $$i $$j; \
	    done \
	done
	./get_output 0
	rm -rf output_1 output_2 output_4 output_8 output_16 output_32 output_64

clean:
	rm -rf testalloc
	rm -rf get_output
	rm -rf bench
	rm -rf output_frag output_nofrag 
	rm -rf output_1 output_2 output_4 output_8 output_16 output_32 output_64
