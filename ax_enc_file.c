#include "ax_enc_file.h"
#include "ax_encript/ax_encript.h"

//private functions
#define _ax_file_version 0x0001
#define _ax_file_buffer_size (sizeof(ax_encript_block_t)*1000)
int _ax_file_exist(char *path){
	
	return 0;
}
int _ax_file_size(char *path){
	
	return 0;
}

struct ax_file_header_t{
  int16_t version;
  int16_t enc_type;
  int16_t content_offset;
  int16_t content_len;
  int16_t raw_len;
  int16_t meta_offset;
  int16_t meta_len;
  char _private[32];
};
ax_encript_func_t _ax_file_enc_func_name_to_value(char *name){
   
  return ax_encript_func_xor;
}
//interface functions
int ax_file_encode(char *inpath,char *outpath,char *password,char *func,char *desc,ax_file_progress_cb on_progress,void* ud){
	if(_ax_file_exist(inpath)){
	  FILE *infile=fopen(inpath,"r");
	  FILE *outfile=fopen(outpath,"w+");
	  struct ax_file_header_t header;
	  header.version=_ax_file_version;
	  header.enc_type=_ax_file_enc_func_name_to_value(func);
	  header.content_offset=sizeof(header);
	  header.raw_len=_ax_file_size(inpath);
	  header.content_len=0;
	  header.meta_offset=0;
	  header.meta_len=0;
	  //write header to output file
	  fwrite(&header,sizeof(header),1,outfile);
	  //encode file and write to output file
	  //loop to read block buffer
	  char inbuffer[_ax_file_buffer_size];
	  char outbuffer[_ax_file_buffer_size];
	  //write meta data to file
	  int readlen=0;
	  do{
	    int outbufferlen=0;
	    readlen=fread(inbuffer,1,sizeof(inbuffer),infile);
	    //encript buffer to out buffer
	    int blockcount=readlen/sizeof(ax_encript_block_t);
	    for(int i=0;i<blockcount;i++){
	      //encript one block
	      
	      //write out block to out buffer
	      
	    }
	    //write outbuffer to outfile
	    fwrite(outbuffer,1,outbufferlen,outfile);
	  }while(readlen!=0);
	  //update file header

	  //close infile
	  fclose(infile);
	  //close outfile
	  fclose(outfile);
	  return 0;
	}else{
		return -1;//file not exist
	}		
}
int ax_file_decode(char *inpath,char *outpath,char *passwrod,ax_file_progress_cb on_progress,void* ud){
  if(_ax_file_exist(inpath)){
    FILE *infile=fopen(inpath,"r");
    FILE *outfile=fopen(outpath,"w");
    struct ax_file_header_t header;

    fclose(infile);
    fclose(outfile);
    return 0;
  }else{
    
    return -1;
  }
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


