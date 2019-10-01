#include<stdio.h>
#include<math.h>
#include<sys/stat.h>
int nSamples(short int * arr){
	unsigned short int  a1=(unsigned short int)arr[21];
	int mod=0,chunk2s=0,ipow=4;
	while(a1!=0){
		mod=a1%16;
		a1/=16;
		chunk2s+=(pow(16,ipow)*mod);
		ipow++;
	}
	return ((chunk2s+(int)arr[20])/2);
}
int main(int argc,char*argv[]){
	long double h[100];
	for(int i=0;i<100;i++) h[i]=exp((i)*(-0.2849517201));
	struct stat buf;
	stat(argv[1],&buf);
	int sint = sizeof(short int),arrsize=buf.st_size/sint,nsam;
	short int wavFile[arrsize];
	FILE *fp;
	fp = fopen(argv[1],"r");
	fread(wavFile,sint,arrsize,fp);
	fclose(fp);
	nsam = nSamples(wavFile);
	long double x[nsam],y[nsam];
	for(int i=22,c=0;i<(22+nsam);i++,c++){
		x[c]=((long double)wavFile[i])/32768.0;
		y[c]=0.0;
	}
	for(int i=0;i<nsam;i++)for(int j=0;j<100;j++)y[i]+=(x[i-j]*h[j]);
	for(int i=22,c=0;i<(22+nsam);i++,c++) wavFile[i]=(short int)(y[c]*32768.0);
	fp = fopen(argv[2],"w");
	fwrite(wavFile,sint,arrsize,fp);
	fclose(fp);
	return 0;
}
