#include <cstdio>
#include <iostream>
#include <vector>
#include <cassert>
#include <cstring>
#include <cmath>
#include "rdtscp_timer/timing.hh"

#define FRAG 1
#define N_ALLOCS 20
int THREAD_NUM;

using namespace std;
int power_size;
double times[64][N_ALLOCS];
double clock_speed_ghz;
size_t SIZE;

static double count_avg (int id) 
{
    double avg = 0.0;
    for (int i = 0; i < N_ALLOCS; i++)
	avg += times[id][i];
    return avg / N_ALLOCS;
}

static void *do_malloc (void *arg) 
{
    vector<char*> ptrs;
    int id = *((int*)arg);
    Timer t(clock_speed_ghz);
    for (int i = 0; i < N_ALLOCS; i++) {
	void *vptr = NULL;
	char *a = NULL;

	t.tick();
	vptr = malloc(SIZE);
	memset(vptr, rand(), SIZE); // make sure we actually have pages allocated
	t.tock();

	assert(!(reinterpret_cast<unsigned long long>(a) & 0xFFFFF));
			
	#if defined(__GNUC__)
	    __builtin___clear_cache((void *) vptr, (void *) vptr + (SIZE));
	#endif
			
	times[id][i] = t.get_time();
	ptrs.push_back(a);
    }
}

// Time the allocations of various sizes
static void testAllocation() 
{
    FILE *fp;
    int id[THREAD_NUM];
    pthread_t threads[THREAD_NUM];
    /* Different thread_num different file */
    char s[10];
    sprintf(s, "output_%d", THREAD_NUM);
    fp = fopen(s,"a+");

    /* Separate size to each thread */
    SIZE = pow(2,power_size);
    for (int i = 0; i < THREAD_NUM; i++){
	id[i] = i;
	pthread_create(&threads[i], NULL, do_malloc, (void*)&id[i]); 
    }
    for (int i = 0; i < THREAD_NUM; i++)
	pthread_join(threads[i], NULL);

    double sum = 0.0;
    for (int i = 0; i < THREAD_NUM; i++)
	sum += count_avg(i);

    fprintf(fp, "%zubytes %.10lf\n", SIZE, sum);
    fclose(fp);
}

int main(int argc, char **argv) 
{
    clock_speed_ghz = 1.6;
    THREAD_NUM = atoi(argv[1]);
    power_size = atoi(argv[2]);
    testAllocation();
    return 0;
}
