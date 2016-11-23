#include <cstdio>
#include <iostream>
#include <vector>
#include <cassert>
#include <cstring>
#include <cmath>
#include "rdtscp_timer/timing.hh"

#define MAX_POW_SIZE 22
int THREAD_NUM;

using namespace std;
double times[64][MAX_POW_SIZE];
double clock_speed_ghz;

static void *do_malloc (void *arg) 
{
    vector<char*> ptrs;
    int id = *((int*)arg);
    Timer t(clock_speed_ghz);
    size_t SIZE;
    for (int i = 2; i <= MAX_POW_SIZE; i++) {
	void *vptr = NULL;
	char *a = NULL;
	SIZE = pow(2,i);

	t.tick();
	vptr = malloc(SIZE);
	memset(vptr, rand(), SIZE); // make sure we actually have pages allocated
	t.tock();

	assert(!(reinterpret_cast<unsigned long long>(a) & 0xFFFFF));
			
	#if defined(__GNUC__)
	    __builtin___clear_cache((void *) vptr, (void *) vptr + (SIZE));
	#endif
			
	times[id][i-1] = t.get_time();
	ptrs.push_back(a);
    }
}

// Time the allocations of various sizes
static void testAllocation() 
{
    FILE *fp;
    int id[THREAD_NUM];
    pthread_t threads[THREAD_NUM];
    fp = fopen("bench_output","a+");

    /* Separate size to each thread */
    for (int i = 0; i < THREAD_NUM; i++){
	id[i] = i;
	pthread_create(&threads[i], NULL, do_malloc, (void*)&id[i]); 
    }
    for (int i = 0; i < THREAD_NUM; i++)
	pthread_join(threads[i], NULL);

    /* Compute total time executed */
    double sum = 0.0;
    for (int i = 0; i < THREAD_NUM; i++)
	for (int j = 1; j < MAX_POW_SIZE; j++)
	    sum += times[i][j];

    fprintf(fp, "%d %.10lf\n", THREAD_NUM, sum);
    fclose(fp);
}

int main(int argc, char **argv) 
{
    clock_speed_ghz = 1.6;
    THREAD_NUM = atoi(argv[1]);
    testAllocation();
    return 0;
}
