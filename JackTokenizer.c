#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jackheader.h"

int tokenizer(char *vcode,TOKEN tok[],int i)//注意这里char token[][100]不能写成char **token,虽然那么写不会报错，但运行到下面strcpy时会报错
{
    char keyword[21][20]={"class","constructor","function","method","field","static","var","int","char","boolean","void","true","false","null","this","let","do","if","else","while","return"};
    char symbol[20]="{}()[].,;+-*/&|<>=~";
    int tempi=0;
    char*p=vcode;
    char q[100];//处理读取到的字串，包括关键字，数字常量，标识名，字符串常量
    int flag;

    while(*p!='\n')
    {
        while(*p==' '||*p=='\t')
        p++;

        if(*p=='"')//处理双引号里的字符串
        {
            tok[i].tokentype=4;
            p++;
            int nj=0;
            for(int j=0;*p!='"';j++)
            {
                tok[i].tokenword[j]=*p;
                p++;
                nj=j;
            }
            p++;//移动到第二个双引号后面
            tok[i].tokenword[nj+1]='\0';
            //printf("%s\n",tok[i].tokenword);
            i++;
        }
        else
        {
            flag=0;//用来表示找到关键字，如果找到关键字为1,直接continue处理下个，否则继续看看是字符串、标示符或数字
            //while(*p==' ')
              //  p++;
            if((*p=='/'&&p[1]=='/')||*p=='\n')
                break;//处理每一行末尾的注释,以及每一行末尾有空格，然后再换行的情况，不然p会指向意想不到的地方
            if(*p=='{'||*p=='}'||*p=='('||*p==')'||*p=='['||*p==']'||*p=='.'||*p==','||*p==';'||*p=='+'||*p=='-'||*p=='*'||*p=='/'||*p=='&'||*p=='|'||*p=='<'||*p=='>'||*p=='='||*p=='~')
            {
                for(int k=0;k<21;k++)
                {
                    if((*p-symbol[k])==0)
                    {
                        //printf("%c\n",*p);
                        tok[i].tokentype=2;//符号是2
                        tok[i].tokenword[0]=*p;
                        tok[i].tokenword[1]='\0';//还是把单个字符处理成字符串
                        i++;
                        break;
                    }
                }
                p++;
            }
            else
            {
                q[tempi++]=*p++;
                if(*p=='\n'||*p==' '||*p=='{'||*p=='}'||*p=='('||*p==')'||*p=='['||*p==']'||*p=='.'||*p==','||*p==';'||*p=='+'||*p=='-'||*p=='*'||*p=='/'||*p=='&'||*p=='|'||*p=='<'||*p=='>'||*p=='='||*p=='~')
                {
                    q[tempi]='\0';
                    tempi=0;
                    //printf("%s\n",q);
                    for(int k=0;k<21;k++)
                    {
                        if((strcmp(q,keyword[k]))==0)
                        {
                            tok[i].tokentype=1;//关键字是1
                            strcpy(tok[i].tokenword,q);
                            i++;
                            flag=1;
                            break;
                        }
                    }
                    if(flag==1)
                        continue;
                    if(*q>=48&&*q<=57)
                    {
                        tok[i].tokentype=3;//关键字是3 数字常量
                        strcpy(tok[i].tokenword,q);
                        i++;
                    }
                    //else if(*q=='"')
                    //{
                       // printf("string");
                       // tok[i].tokentype=4;//关键字4 字符串常量
                       // printf("string");
                       // for(int j=0;q[j+1]!='"';j++)
                       //     tok[i].tokenword[j]=q[j+1];
                       // i++;
                   // }
                    else
                    {
                        tok[i].tokentype=5;//关键字5 标示符
                        strcpy(tok[i].tokenword,q);
                        i++;
                    }

                }
            }

        }
    }
    return i;
}

void WriteToken(TOKEN tok[],FILE* fb,int i)
{
    fputs("<tokens>\n",fb);
    for(int j=0;j<i;j++)
    {
        if(tok[j].tokentype==1)
        {
            fputs("<keyword> ",fb);
            fputs(tok[j].tokenword,fb);
            fputs(" </keyword>\n",fb);
        }
        else if(tok[j].tokentype==2)
        {
            fputs("<symbol> ",fb);
            if(strcmp(tok[j].tokenword,"<")==0)//生成.xml时候才需要
                fputs("&lt;",fb);
            else if(strcmp(tok[j].tokenword,">")==0)
                fputs("&gt;",fb);
            else if(strcmp(tok[j].tokenword,"&")==0)
                fputs("&amp;",fb);
            else
                fputs(tok[j].tokenword,fb);
            fputs(" </symbol>\n",fb);
        }
        else if(tok[j].tokentype==3)
        {
            fputs("<integerConstant> ",fb);
            fputs(tok[j].tokenword,fb);
            fputs(" </integerConstant>\n",fb);
        }
        else if(tok[j].tokentype==4)
        {
            fputs("<stringConstant> ",fb);
            fputs(tok[j].tokenword,fb);
            fputs(" </stringConstant>\n",fb);
        }
        else if(tok[j].tokentype==5)
        {
            fputs("<identifier> ",fb);
            fputs(tok[j].tokenword,fb);
            fputs(" </identifier>\n",fb);
        }

    }
    fputs("</tokens>\n",fb);
}
