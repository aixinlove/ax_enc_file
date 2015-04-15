#ifndef __ax_enc_file__
#define __ax_enc_file__
#include <stdint.h>
struct ax_file_header_t{
    int16_t version;
    int16_t enc_type;
    int64_t content_offset;
    int64_t content_len;
    int64_t raw_len;
    int64_t meta_offset;
    int64_t meta_len;
    int64_t create_time;
    int64_t last_access_time;
    char _private[64];
};

typedef void (ax_file_progress_cb)(float progress,void *ud);
int ax_file_encode(char *inpath,char *outpath,char *password,char *func,char *desc,ax_file_progress_cb on_progress,void* ud);
int ax_file_decode(char *inpath,char *outpath,char *passwrod,ax_file_progress_cb on_progress,void* ud);
int ax_file_read_desc(char *inpath,char *buffer,int bufferlen);
int ax_file_read_header(char *inpath,struct ax_file_header_t* header);
int ax_file_get_enctrypt_type(struct ax_file_header_t* header,char *enctype,int enclen);
#endif
