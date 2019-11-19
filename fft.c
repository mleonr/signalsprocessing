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

complex* fft(complex* x,int N){
	int a=N/2,ec=0,oc=0;	
	long double cs=0,sn=0;
	complex*x1,*even,*odd,*neven,*nodd,*x_,*y;
	if(N==2){
		x1=(complex*)malloc(2*sizeof(complex));
		x1[0].r=x[0].r+x[1].r;
		x1[0].j=0.0;
		x1[1].r=x[0].r-x[1].r;
		x1[1].j=0.0;
		return x1;	
	}

	even=(complex*)malloc(a*sizeof(complex));
	odd=(complex*)malloc(a*sizeof(complex));
		
	for(int i=0;i<N;i++){
		
		if((i%2)==0){
			even[ec].r=x[i].r;	
			even[ec].j=x[i].j;
			ec++;
		}else{
			odd[oc].r=x[i].r;
			odd[oc].j=x[i].j;
			oc++;			
		}

	}
	
	neven = fft(even,a);
	nodd = fft(odd,a);
	
	x_=(complex*)malloc(N*sizeof(complex));

	for(int i=0;i<a;i++){
		x_[i].r=neven[i].r;
		x_[i].j=neven[i].j;
		x_[i+a].r=nodd[i].r;
		x_[i+a].j=nodd[i].j;
	}
	
	y=(complex*)malloc(N*sizeof(complex));
	
	for(int i=0;i<a;i++){
		cs=cos(2*M_PI*i/N);
		sn=sin(2*M_PI*i/N); 
		y[i].r=x_[i].r+cs*x_[i+a].r+sn*x_[i+a].j;
		y[i].j=x_[i].j+cs*x_[i+a].j-sn*x_[i+a].r;
		y[i+a].r=x_[i].r-cs*x_[i+a].r-sn*x_[i+a].j;
		y[i+a].j=x_[i].j-cs*x_[i+a].j+sn*x_[i+a].r;	
	}

	return y;
}

int main(int argc,char*argv[]){
	struct stat buf;
	stat(argv[1],&buf);
	int sint = sizeof(short int),arrsize=buf.st_size/sint,nsam,nsam2;
	short int *wavFile;
	long double lg,cl;
	
	wavFile=(short int*)malloc(buf.st_size);
	if(wavFile==NULL)exit(0);
  	
  	FILE *fp;
	fp = fopen(argv[1],"r");
	fread(wavFile,sint,arrsize,fp);
	fclose(fp);
	nsam = nSamples(wavFile);
		
	lg = log(nsam)/log(2);
	cl = ceil(lg);	
	
	nsam2 = ((cl-lg)==0) ? nsam : (int)pow(2,cl); 
	
	complex *x,*y;
	
	x = (complex*)malloc(nsam2*sizeof(complex));
	for(int i=22,c=0;i<(22+nsam2);i++,c++){
		x[c].r=(c<nsam)?(long double)wavFile[i]:0.0;
		x[c].j=0.0;
	}
	
	y=fft(x,nsam2);

	unsigned int fileSize,val=176400;
	fileSize=118+nsam2*4;
	
	short int *outFile;
	outFile=(short int*)malloc(fileSize);
	
	for(int i=0;i<(fileSize/2);i++)outFile[i]=0;
	for(int i=0;i<22;i++)outFile[i]=wavFile[i];
	
	outFile[2]=((fileSize-8)& 0xffff);
	outFile[3]=((fileSize-8)>>16);
	outFile[11]=2;
	outFile[14]=val & 0xffff;
	outFile[15]=val>>16;
	outFile[16]=4;
	outFile[20]=((nsam2*4)& 0xffff);
	outFile[21]=((nsam2*4)>>16);

	for(int i=22,k=0;i<(22+nsam2*2);i+=2,k++){
		outFile[i]=(short int)(y[k].r/nsam2);
		outFile[i+1]=(short int)(y[k].j/nsam2);
	}

	for(int i=22+nsam2*2,c=arrsize-37;i<fileSize/2;i++,c++)outFile[i]=wavFile[c];
	
	fp = fopen(argv[2],"w");
	fwrite(outFile,sint,fileSize/2,fp);
	fclose(fp);

	return 0;
}

