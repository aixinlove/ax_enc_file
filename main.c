#include <stdio.h>
#include <stdlib.h>
#include "ax_enc_file.h"
void progress(float progress,void *ud){
	printf("->%f\n",progress);
}
int main(){
	printf("encode a file\n");
	ax_file_encode("a.txt","b.txt.ax","12345678901234567890","xor","a test file",progress,NULL);
	printf("now decode\n");
	ax_file_decode("b.txt.ax","c.txt","12345678901234567890",progress,NULL);
	return 0;
}
