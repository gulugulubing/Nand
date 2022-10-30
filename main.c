#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include "jackheader.h"//��ͬһ��workspace��ͷ�ļ�����Ҫ����һ��,.c���ļ�������


int main()
{
    struct _finddata_t fileinfo;
    char vcode[N];//��������jack
    char tcode[N];//��������T.xml
    int i;//token������
    char flname[50];//XML���ļ���
    char flnameT[50];
    TOKEN tok[M];//�ṹ������
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
                continue;//ע���в�����
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
            WriteToken(tok,fb,i);//����tokenд��xml
            fclose(fp);
            fclose(fb);
            if((fp=fopen(flnameT,"r"))==NULL)  //����T.XML
            {
                printf("Failure to open \n");
                exit(0);
            }
            if((fb=fopen(flname,"a"))==NULL)//д��.XML
            {
                printf("Failure to open \n");
                exit(0);
            }
            //xml�ĵ�һ�в��ö�ȡ
            //moveToNextLine(fp);����Ҫ�ˣ�fgetsһ�ξͻ��Զ�����һ��
            fgets(tcode,N,fp);
            fgets(tcode,N,fp);
            //printf("%s",tcode);
            CompileClass(tcode,fb,fp);//����fp��λ����Ϣ�Ѿ�ָ���˵����У���Ϊ�����Ѿ�fgets������
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
    while(*p==' '||*p=='\t')// \t��tab�ַ�
        p++;
    while((*q++=*p++))
        ;
}
