//
//  main.m
//  axfile
//
//  Created by 彭运筹 on 15/5/2.
//  Copyright (c) 2015年 彭运筹. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#import "ax_enc_file.h"
#define ax_option_len 256

void ax_show_progress(char *prefix,float progress,int len){
    printf("%s%0.2f%% ", prefix,progress);
    printf("\n\033[F\033[J");
}
void onprogress_dec(float progress,void *ud){
    ax_show_progress("decode:",progress,50);
}
void onprogress_enc(float progress,void *ud){
    ax_show_progress("encode:",progress,50);
}
void get_option(char *option,char *key,char *value){
    char *ch= strtok(option, "=");
    if (ch!=NULL) {
        memcpy(key, ch, strlen(ch));
    }
    ch = strtok(NULL, "=");
    if (ch!=NULL) {
        memcpy(value, ch, strlen(ch));
    }
}
int main(int argc, const char * argv[]) {
    struct{
        char infile[ax_option_len];
        char outfile[ax_option_len];
        char password[ax_option_len];
        char cipher[ax_option_len];
        int8_t show_progress;
        int8_t is_decode;
    }options;
    if (argc<=4) {
        printf("usage:\n");
        printf("\tencode->  \n\t%s in=(in path) out=(out path) cipher=[xor|tea|xtea|xxtea] password=your password [progress=true]\n",argv[0]);
        printf("\tdecode->  \n\t%s in=(in path) out=(out path) password=your password [progress=true]\n",argv[0]);
        return -1;
    }
    memset(&options, 0x0, sizeof(options));
    for (int i=1; i<argc; i++) {
        char key[64],value[256];
        memset(key, 0x0, sizeof(key));
        memset(value, 0x0, sizeof(value));
        get_option((char *)argv[i], key, value);
        if (strcasecmp(key, "in")==0) {
            strncpy(options.infile, value, strlen(value));
        }else if (strcasecmp(key, "out")==0) {
            strncpy(options.outfile, value, strlen(value));
        }if (strcasecmp(key, "password")==0) {
            strncpy(options.password, value, strlen(value));
        }if (strcasecmp(key, "progress")==0) {
            if (strcasecmp(value, "true")==0) {
                options.show_progress=1;
            }
        }if (strcasecmp(key, "cipher")==0){
            strncpy(options.cipher, value, strlen(value));
        }if (strcasecmp(key, "type")==0) {
            if (strcasecmp(value, "enc")==0) {
                options.is_decode=0;
            }else{
                options.is_decode=1;
            }
        }
    }
    ax_file_progress_cb *progresscb=NULL;
    if (options.show_progress) {
        if (options.is_decode) {
            progresscb=onprogress_dec;
        }else{
            progresscb=onprogress_enc;
        }
    }
    if (strlen(options.infile)==0) {
        printf("please use in option \n");
        return -1;
    }
    if (strlen(options.outfile)==0) {
        printf("please use out option \n");
        return -1;
    }
    if (strlen(options.password)==0) {
        printf("please use password option \n");
        return -1;
    }
    printf("in:%s\n",options.infile);
    printf("out:%s\n",options.outfile);
    printf("password:%s\n",options.password);
    if (options.is_decode) {
        ax_file_decode(options.infile, options.outfile, options.password, progresscb, NULL);
    }else{
        if (strlen(options.cipher)==0) {
            printf("please use cipher option \n");
            return -1;
        }
        ax_file_encode(options.infile, options.outfile, options.password,options.cipher, "", progresscb, NULL);
    }
    printf("complete!\n");
    return 0;
}
