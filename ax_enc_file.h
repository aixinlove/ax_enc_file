#ifndef __ax_enc_file__
#define __ax_enc_file__
typedef void (ax_file_progress_cb)(float progress,void *ud);
int ax_file_encode(char *inpath,char *outpath,char *password,char *func,char *desc,ax_file_progress_cb on_progress,void* ud);
int ax_file_decode(char *inpath,char *outpath,char *passwrod,ax_file_progress_cb on_progress,void* ud);
int ax_file_read_desc(char *inpath);
int ax_file_read_enc_type(char *inpath,char *type);
int ax_file_read_file_len(char *inpath);


#endif
