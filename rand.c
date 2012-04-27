#include <stdio.h>
#include <stdlib.h>
int main(){
	int num;
	FILE *fp;
	int i=0;
	int max = 10;
	
	fp=fopen("/dev/urandom","r");
	
	if (fp==NULL){
		printf("effed");
		exit(1);	
	}
	
	while(i<8){
		num = fgetc(fp);
		srand(num);
		
    	printf("%d\n",rand()%max);
    	i++;
	}
}
