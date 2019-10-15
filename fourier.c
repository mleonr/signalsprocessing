#include<stdio.h>
#include<math.h>
#include<sys/stat.h>
#include<stdlib.h>
#define M_PI 3.14159265358979323846

typedef struct{
	long double r,j;
} complex;

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
	struct stat buf;
	stat(argv[1],&buf);
	int sint = sizeof(short int),sdouble = sizeof(long double),arrsize=buf.st_size/sint,nsam;
	short int *wavFile;
	
	wavFile=(short int*)malloc(buf.st_size);
	if(wavFile==NULL)exit(0);
  	
  	FILE *fp;
	fp = fopen(argv[1],"r");
	fread(wavFile,sint,arrsize,fp);
	fclose(fp);
	nsam = nSamples(wavFile);
	
	long double *x;
	x = (long double*)malloc(sdouble*nsam);
	complex *y;
	y = (complex*)malloc(nsam*sizeof(complex));
	for(int i=22,c=0;i<(22+nsam);i++,c++){
		x[c]=(long double)wavFile[i];
		y[c].r=0.0;
		y[c].j=0.0;
	}

	for(int k=0;k<nsam;k++){
		for(int n=0;n<nsam;n++){
			y[k].r+=(x[n]*cos(2*M_PI*k*n/nsam));
			y[k].j+=(x[n]*sin(2*M_PI*k*n/nsam));
		}
	}

	unsigned int fileSize,val=176400;
	//118 = header size + footer size = 44+74
	fileSize=118+nsam*4;
	
	short int *outFile;
	outFile=(short int*)malloc(fileSize);
	
	for(int i=0;i<22;i++)outFile[i]=wavFile[i];
	
	outFile[2]=((fileSize-8)& 0xffff);//ChunkSize less
	outFile[3]=((fileSize-8)>>16);//ChunkSize 
	outFile[11]=2;//Stereo 
	outFile[14]=val & 0xffff;//ByteRate less
	outFile[15]=val>>16;//ByteRate
	outFile[16]=4;//BlocklAlign
	outFile[20]=((nsam*2)& 0xffff);//Subchunk2Size less
	outFile[21]=((nsam*2)>>16);//Subchunk2Size

	for(int i=22,k=0;i<(22+nsam*2);i+=2,k++){
		outFile[i]=(short int)(y[k].r/nsam);
		outFile[i+1]=(short int)(y[k].j/nsam);
	}

	for(int i=22+nsam*2,c=arrsize-33;i<fileSize/2;i++,c++)outFile[i]=wavFile[c];
	
	fp = fopen(argv[2],"w");
	fwrite(outFile,sint,fileSize/2,fp);
	fclose(fp);

	return 0;
}
