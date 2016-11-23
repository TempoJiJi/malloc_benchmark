#include <stdio.h>
#include <stdlib.h>

#define PT_MALLOC 0
#define SUPER_MALLOC 1
#define ALLOCATOR 2

int main(int argc, char *argv[])
{
    char file[10];
    int f = atoi(argv[1]);
    FILE *fp,*fw;
    switch (f) {
	case PT_MALLOC:
	    fw = fopen("output_ptm","w+");
	    break;
	case SUPER_MALLOC:
	    fw = fopen("output_super","w+");
	    break;
	case ALLOCATOR:
	    fw = fopen("output_alloc","w+");
	    break;
    }
    int threads[7] = {1,2,4,8,16,32,64};
    double times[7][21];
    int bytes_count[10];

    for (int i=0;i<7;i++) {
	sprintf(file, "output_%d", threads[i]);
	    //printf("opening %s\n",file);
	fp = fopen(file, "r");
	if(!fp)
	    printf("%s\n",file);
	for (int j=0;j<10;j++) {
	    double k;
	    int tmp;
	    fscanf(fp, "%dbytes %lf\n",&tmp,&k);
	    times[i][j] = k;
	    bytes_count[j] = tmp;
	}
	fclose(fp);
    }

    for(int i=0;i<10;i++) {
	fprintf(fw, "%d %.10lf %.10lf %.10lf %.10lf %.10lf %.10lf %.10lf\n",
			bytes_count[i], times[0][i], times[1][i], times[2][i], 
			    times[3][i], times[4][i], times[5][i], times[6][i]);
    }
    fclose(fw);
}
