#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *fp = fopen("bench_output","r");
    double avg[7][100]; // For thread 1,2,4,8,16,32,64
    int threads = 0;
    int times = 0;
    int tmp;

    while((fscanf(fp, "%d %lf\n", &tmp, &avg[threads][times])) != EOF) {
	times++;
	if (times == 100) {
	    times = 0;
	    threads++;
	}
    }
    fclose(fp);
    fp = fopen("output","w+");

    threads = 1;
    double count = 0.0;
    for (int i = 0; i < 7; i++) {
	for (int j = 0; j < 100; j++) {
	    count += avg[i][j];
	}
	fprintf(fp, "%d %.10lf\n", threads, count / 100);
	count = 0.0;
	threads = threads * 2;
    }
}
