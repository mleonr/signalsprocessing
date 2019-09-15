#include<stdio.h>
#include<sys/stat.h>
#include<math.h>
void voldown(short int * arr){
	char sint[5];
	sint[0]=arr[20] & 0xff;
	sint[1]=(arr[20]>>8)& 0xff;
	sint[2]=arr[21] & 0xff;
	sint[3]=(arr[21]>>8)& 0xff;
	int chunk2s=0,ipow=0;
	for(int i=0;i<5;i++){
		int val=(int)sint[i],mod=0;
		while(val!=0){
			mod=val%16;
			val/=16;
			chunk2s+=(pow(16,ipow)*mod);
			ipow++;
		}
			
	}

	for(int i=22;i<(22+(chunk2s/2));i++){
		arr[i]=arr[i]/2;
	}
}
int  main(int argc, char*argv[]){
	struct stat buf;
	stat(argv[1],&buf);
	int sint = sizeof(short int),nsamples,arrsize=buf.st_size/sint;
	short int samples[arrsize];
	FILE *fp;
	fp = fopen(argv[1],"r");
	if(fp){
		nsamples=fread(samples,sint,arrsize,fp);	
		fclose(fp);
	}
	voldown(samples);
	fp = fopen(argv[2],"w");	
	if(fp){
		nsamples=fwrite(samples,sint,arrsize,fp);	
		fclose(fp);
	}

}