#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include "jackheader.h"//在同一个workspace下头文件还是要包含一下,.c的文件不用了


int main()
{
    struct _finddata_t fileinfo;
    char vcode[N];//用来读入jack
    char tcode[N];//用来读入T.xml
    int i;//token的索引
    char flname[50];//XML的文件名
    char flnameT[50];
    TOKEN tok[M];//结构体数组
    long fHandle;


    if((fHandle=_findfirst("*.jack",&fileinfo))==-1L)
        printf("current fold has no jack file");
    else
    {
        do{
            i=0;
            if((fp=fopen(fileinfo.name,"r"))==NULL)
            {
                printf("Failure to open \n");
                exit(0);
            }

            while(fgets(vcode,N,fp)!=NULL)
            {
                dblank(vcode);
                if(vcode[0]=='/'||vcode[0]=='\n'||vcode[0]=='*')
                continue;//注释行不处理
                printf("%s",vcode);
                i=tokenizer(vcode,tok,i);
            }

            //printf("%d\n",i);
            //for(int j=0;j<i;j++)
            // printf("%d\n",tok[j].tokentype);
            //for(int j=0;j<i;j++)
            //printf("%s\n",tok[j].tokenword);
            strcpy(flname,fileinfo.name);
            strcpy(flnameT,fileinfo.name);
            char *p=flname;
            while(*p!='.')
                p++;
            *p='\0';
            strcat(flname,".vm");

            p=flnameT;
            while(*p!='.')
                p++;
            *p='\0';
            strcat(flnameT,"T.xml");

            if((fb=fopen(flnameT,"a"))==NULL)
            {
                printf("Failure to open \n");
                exit(0);
            }
            WriteToken(tok,fb,i);//生成token写入xml
            fclose(fp);
            fclose(fb);
            if((fp=fopen(flnameT,"r"))==NULL)  //读入T.XML
            {
                printf("Failure to open \n");
                exit(0);
            }
            if((fb=fopen(flname,"a"))==NULL)//写入.XML
            {
                printf("Failure to open \n");
                exit(0);
            }
            //xml的第一行不用读取
            //moveToNextLine(fp);不需要了，fgets一次就会自动到下一行
            fgets(tcode,N,fp);
            fgets(tcode,N,fp);
            //printf("%s",tcode);
            CompileClass(tcode,fb,fp);//这里fp的位置信息已经指向了第三行，因为上面已经fgets两次了
            fclose(fp);
            fclose(fb);
        }while(_findnext(fHandle,&fileinfo)==0);
    }
    return 0;

}

void dblank(char *symbol)
{
    char*p=symbol;
    char*q=symbol;
    while(*p==' '||*p=='\t')// \t是tab字符
        p++;
    while((*q++=*p++))
        ;
}
