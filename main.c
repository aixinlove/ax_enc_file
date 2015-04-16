#include <stdio.h>
#include <stdlib.h>
#include "ax_enc_file.h"
void progress(float progress,void *ud){
	//printf("->%f\n",progress);
}
int main(){
	printf("encode a file\n");
	ax_file_encode("a.txt","b.txt.ax","12345678901234567890","xxtea","a test file",progress,NULL);
	printf("now decode\n");
	ax_file_decode("b.txt.ax","c.txt","12345678901234567890",progress,NULL);
    
    char desc[512];
    ax_file_read_desc("b.txt.ax", desc, sizeof(desc));
    printf("desc->%s\n",desc);
    struct ax_file_header_t header;
    ax_file_read_header("b.txt.ax", &header);
    char enctype[64];
    ax_file_get_enctrypt_type(&header, enctype, sizeof(enctype));
    printf("enctype->%s\n",enctype);
	return 0;
}
