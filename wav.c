#include<stdio.h>
#include<sys/stat.h>
#include<math.h>
void voldown(short int * arr){
	unsigned short int  a1=(unsigned short int)arr[21];
	int mod=0,chunk2s=0,ipow=4;
	while(a1!=0){
		mod=a1%16;
		a1/=16;
		chunk2s+=(pow(16,ipow)*mod);
		ipow++;
	}
	for(int i=22;i<(22+((chunk2s+(int)arr[20])/2));i++)arr[i]=arr[i]/2;
}
int  main(int argc, char*argv[]){
	struct stat buf;
	stat(argv[1],&buf);
	int sint = sizeof(short int),arrsize=buf.st_size/sint;
	short int samples[arrsize];
	FILE *fp;
	fp = fopen(argv[1],"r");
	fread(samples,sint,arrsize,fp);	
	fclose(fp);
	voldown(samples);
	fp = fopen(argv[2],"w");	
	fwrite(samples,sint,arrsize,fp);	
	fclose(fp);
}
