#include "ax_enc_file.h"
#include "ax_encript/ax_encript.h"
#include <math.h>
//private functions
#define _ax_file_version (0x0001)
#define _ax_file_buffer_block_count (1000)
#define _ax_file_buffer_size (sizeof(ax_encript_block_t)*_ax_file_buffer_block_count)
#define _ax_file_num_of_passwrod (2)
#define _ax_min(a,b) (a)>(b)?(b):(a)
int _ax_file_exist(char *path){
    FILE *f=fopen(path, "r");
    if (f!=NULL) {
        fclose(f);
        return 1;
    }
    return 0;
}
int _ax_file_size(char *path){
    FILE *fp=fopen(path, "r");
    fseek(fp, 0L, SEEK_END);
    int size=ftell(fp);
    fclose(fp);
    return size;
}

struct ax_file_header_t{
    int16_t version;
    int16_t enc_type;
    int16_t content_offset;
    int16_t content_len;
    int16_t raw_len;
    int16_t meta_offset;
    int16_t meta_len;
    char _private[64];
};
extern ax_encript_func_t ax_enc_func_name_to_value(char *name);
void _ax_file_init_password(ax_encript_block_t pwd[],char *password){
    
}
//interface functions
int ax_file_encode(char *inpath,char *outpath,char *password,char *func,char *desc,ax_file_progress_cb on_progress,void* ud){
    if(_ax_file_exist(inpath)){
        FILE *infile=fopen(inpath,"r");
        FILE *outfile=fopen(outpath,"w+");
        struct ax_file_header_t header;
        header.version=_ax_file_version;
        header.enc_type=ax_enc_func_name_to_value(func);
        header.content_offset=sizeof(header);
        header.raw_len=_ax_file_size(inpath);
        header.content_len=0;
        header.meta_offset=0;
        header.meta_len=0;
        //write header to output file
        fwrite(&header,sizeof(header),1,outfile);
        //encode file and write to output file
        //loop to read block buffer
        ax_encript_block_t inbuffer[_ax_file_buffer_block_count];
        ax_encript_block_t outbuffer[_ax_file_buffer_block_count];
        ax_encript_block_t passwordblock[_ax_file_num_of_passwrod];
        _ax_file_init_password(passwordblock, password);
        int readlen=0;
        do{
            readlen=fread(inbuffer,1,_ax_file_buffer_size,infile);
            //encript buffer to out buffer
            int blockcount=ceil((float)readlen/(float)sizeof(ax_encript_block_t));
            for(int i=0;i<blockcount;i++){
                //encript one block
                ax_encript_do(&(inbuffer[i]), &(passwordblock[i%_ax_file_num_of_passwrod]), &(outbuffer[i]), ax_encript_type_enc, header.enc_type);
            }
            //write outbuffer to outfile
            fwrite(outbuffer,sizeof(ax_encript_block_t),blockcount,outfile);
        }while(readlen!=0);
        //write meta message
        
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
        fread(&header, sizeof(header), 1, infile);
        
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


