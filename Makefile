TARGETS = sizebenchmark
OBJS = sizebenchmark.o
CXX := g++
LD := g++
CFLAGS = -lpthread -lm
LDFLAGS = 

TIMER_DIR = ./rdtscp_timer
EXEC = timer sizebenchmark benchmark

all: $(EXEC)

timer:
	cd $(TIMER_DIR); $(MAKE)

sizebenchmark: sizebenchmark.cc timer
	g++ $(TIMER_DIR)/timing.o sizebenchmark.cc -std=c++0x -lrt -o sizebenchmark $(CFLAGS)

benchmark:
	g++ $(TIMER_DIR)/timing.o benchmark.cc -std=c++0x -lrt -o benchmark $(CFLAGS)

get_output:
	gcc -g $@.c -o $@

bench: $(EXEC) calculate
	for i in 1 2 4 8 16 32 64; do \
	    for j in `seq 1 1 100`; do \
		./benchmark $$i; \
	    done \
	done
	./calculate

calculate:
	gcc -g $@.c -o $@

plot: bench
	gnuplot plot.gp

# For different malloc implement,
#   Please Change the ./get_output argv
# @Ptmalloc = 0, @Supermalloc = 1, @Allocator = 2
sizebench: $(EXEC) get_output
	for i in 1 2 4 8 16 32 64; do \
	    for j in 2 4 5 6 8 9 10 12 20 22; do \
		./sizebenchmark $$i $$j; \
	    done \
	done
	./get_output 0
	rm -rf output_1 output_2 output_4 output_8 output_16 output_32 output_64

clean:
	rm -rf sizebenchmark calculate get_output benchmark
	rm -rf output bench_output runtime.png output_frag output_nofrag 
	rm -rf output_1 output_2 output_4 output_8 output_16 output_32 output_64
