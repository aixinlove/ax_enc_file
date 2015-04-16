#include "ax_enc_file.h"
#include "ax_encript/ax_encript.h"
#include <math.h>
#include <string.h>
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
extern ax_encript_func_t ax_enc_func_name_to_value(char *name);
extern void ax_enc_func_to_name(ax_encript_func_t func,char *name,int namelen);
void _ax_file_init_password(ax_encript_block_t pwd[],char *password){
    int pwdsize=_ax_file_num_of_passwrod*sizeof(ax_encript_block_t);
    int passwrodlen=strlen(password);
    char pwdbuff[pwdsize];
    int filloffset=0;
    int segmentcount=ceil((float)pwdsize/(float)passwrodlen);
    int fullcount=segmentcount-1;
    for (int i=0; i<fullcount; i++) {
        memcpy(pwdbuff+filloffset, password, passwrodlen);
        filloffset+=passwrodlen;
    }
    int remainlen=pwdsize-fullcount*passwrodlen;
    memcpy(pwdbuff+filloffset, password, remainlen);
    memcpy(pwd, pwdbuff, pwdsize);
}
int _ax_notifiy_progress(ax_file_progress_cb progresscb,float progress,void* ud){
    if (progresscb!=NULL) {
        progresscb(progress,ud);
    }
    return 0;
}
//interface functions
/*  file structure
 *  /head/content/meta
 */
int ax_file_encode(char *inpath,char *outpath,char *password,char *func,char *desc,ax_file_progress_cb on_progress,void* ud){
    if(_ax_file_exist(inpath)){
        FILE *infile=fopen(inpath,"rb");
        FILE *outfile=fopen(outpath,"wb+");
        struct ax_file_header_t header;
        header.version=_ax_file_version;
        header.enc_type=ax_enc_func_name_to_value(func);
        header.raw_len=_ax_file_size(inpath);
        header.content_offset=sizeof(header);
        header.content_len=sizeof(ax_encript_block_t)*(int)ceil((float)header.raw_len/(float)sizeof(ax_encript_block_t));
        header.meta_offset=header.content_offset+header.content_len;
        header.meta_len=strlen(desc);
        //write header to output file
        fwrite(&header,sizeof(header),1,outfile);
        //encode file and write to output file
        //loop to read block buffer
        ax_encript_block_t inbuffer[_ax_file_buffer_block_count];
        ax_encript_block_t outbuffer[_ax_file_buffer_block_count];
        ax_encript_block_t passwordblock[_ax_file_num_of_passwrod];
        _ax_file_init_password(passwordblock, password);
        int readlen=0;
        int total_read_len=0;
        int totalblock=0;
        do{
            readlen=fread(inbuffer,1,_ax_file_buffer_size,infile);
            //encript buffer to out buffer
            int blockcount=ceil((float)readlen/(float)sizeof(ax_encript_block_t));
            for(int i=0;i<blockcount;i++){
                //encript one block
                ax_encript_do(&(inbuffer[i]), &(passwordblock[totalblock%_ax_file_num_of_passwrod]), &(outbuffer[i]), ax_encript_type_enc, header.enc_type);
                totalblock+=1;
            }
            //write outbuffer to outfile
            fwrite(outbuffer,sizeof(ax_encript_block_t),blockcount,outfile);
            total_read_len+=readlen;
            _ax_notifiy_progress(on_progress, (float)total_read_len/(float)header.raw_len, ud);
        }while(readlen!=0);
        //write meta message
        fwrite(desc, strlen(desc), 1, outfile);
        //close infile
        fclose(infile);
        //close outfile
        fclose(outfile);
        return 0;
    }else{
        return -1;//file not exist
    }
}
int ax_file_decode(char *inpath,char *outpath,char *password,ax_file_progress_cb on_progress,void* ud){
    if(_ax_file_exist(inpath)){
        FILE *infile=fopen(inpath,"rb");
        FILE *outfile=fopen(outpath,"wb+");
        struct ax_file_header_t header;
        fread(&header, sizeof(header), 1, infile);
        //loop read buffer and decode
        ax_encript_block_t inbuffer[_ax_file_buffer_block_count];
        ax_encript_block_t outbuffer[_ax_file_buffer_block_count];
        ax_encript_block_t passwordblock[_ax_file_num_of_passwrod];
        _ax_file_init_password(passwordblock, password);
        //read content and decode it
        fseek(infile, header.content_offset, SEEK_SET);
        int inreadlen=0;
        int totalblock=0;
        do {
            int readlen=fread(inbuffer, 1, _ax_min(_ax_file_buffer_size, header.content_len-inreadlen), infile);
            int blockcount=ceil((float)readlen/(float)sizeof(ax_encript_block_t));
            for (int i=0; i<blockcount; i++) {
                //decode to outbuffer
                ax_encript_do(&(inbuffer[i]), &(passwordblock[totalblock%_ax_file_num_of_passwrod]), &(outbuffer[i]), ax_encript_type_dec, header.enc_type);
                totalblock+=1;
            }
            //write outbuffer to file
            fwrite(outbuffer, 1, _ax_min(readlen, header.raw_len-inreadlen), outfile);
            inreadlen+=readlen;
            _ax_notifiy_progress(on_progress, (float)inreadlen/(float)header.content_len, ud);
        } while (inreadlen<header.content_len);
        fclose(infile);
        fclose(outfile);
        return 0;
    }else{
        return -1;
    }
}
int ax_file_read_desc(char *inpath,char *buffer,int bufferlen){
    if(_ax_file_exist(inpath)){
        FILE *infile=fopen(inpath, "r");
        struct ax_file_header_t header;
        fread(&header, sizeof(header), 1, infile);
        fseek(infile, header.meta_offset, SEEK_SET);
        fread(buffer, 1, _ax_min(header.meta_len, bufferlen), infile);
        fclose(infile);
    }
    return 0;
}
int ax_file_read_header(char *inpath,struct ax_file_header_t* header){
    if(_ax_file_exist(inpath)){
        FILE *infile=fopen(inpath, "r");
        fread(header, sizeof(header), 1, infile);
        fclose(infile);
    }
    return 0;
}
int ax_file_get_enctrypt_type(struct ax_file_header_t* header,char *enctype,int enclen){
    ax_enc_func_to_name(header->enc_type,enctype,enclen);
    return 0;
}