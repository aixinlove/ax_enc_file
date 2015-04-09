#include "ax_enc_file.h"
#include "ax_encript/ax_encript.h"

//private functions

int _ax_file_exist(char *path){
	
	return 0;
}
int _ax_file_size(char *path){
	
	return 0;
}



//interface functions
int ax_file_encode(char *inpath,char *outpath,char *password,char *func,char *desc,ax_file_progress_cb on_progress,void* ud){
	if(_ax_file_exist(inpath)){

	}else{
		return -1;//file not exist
	}		
	return 0;
}
int ax_file_decode(char *inpath,char *outpath,char *passwrod,ax_file_progress_cb on_progress,void* ud){
	
	return 0;
}
int ax_file_read_desc(char *inpath){

	return 0;
}
int ax_file_read_enc_type(char *inpath,char *type){

	return 0;
}
int ax_file_read_file_len(char *inpath){

	return 0;
}


