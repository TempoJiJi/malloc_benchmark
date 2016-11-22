#include <stdio.h>

int main()
{
    char file[10];
    FILE *fp,*fw = fopen("output","w+");
    int threads[7] = {1,2,4,8,16,32,64};
    double times[7][21];

    for (int i=0;i<7;i++) {
	sprintf(file, "output_%d", threads[i]);
	    //printf("opening %s\n",file);
	fp = fopen(file, "r");
	if(!fp)
	    printf("%s\n",file);
	for (int j=0;j<21;j++) {
	    double k;
	    int tmp;
	    fscanf(fp, "%dbytes %lf\n",&tmp,&k);
	    times[i][j] = k;
	}
	fclose(fp);
    }

    for(int i=0;i<21;i++) {
	fprintf(fw, "%.10lf %.10lf %.10lf %.10lf %.10lf %.10lf %.10lf\n",
			times[0][i], times[1][i], times[2][i], times[3][i], 
			    times[4][i], times[5][i],times[6][i]);
    }
    fclose(fw);
}
