#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jackheader.h"

int tokenizer(char *vcode,TOKEN tok[],int i)//ע������char token[][100]����д��char **token,��Ȼ��ôд���ᱨ�������е�����strcpyʱ�ᱨ��
{
    char keyword[21][20]={"class","constructor","function","method","field","static","var","int","char","boolean","void","true","false","null","this","let","do","if","else","while","return"};
    char symbol[20]="{}()[].,;+-*/&|<>=~";
    int tempi=0;
    char*p=vcode;
    char q[100];//�����ȡ�����ִ��������ؼ��֣����ֳ�������ʶ�����ַ�������
    int flag;

    while(*p!='\n')
    {
        while(*p==' '||*p=='\t')
        p++;

        if(*p=='"')//����˫��������ַ���
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
            p++;//�ƶ����ڶ���˫���ź���
            tok[i].tokenword[nj+1]='\0';
            //printf("%s\n",tok[i].tokenword);
            i++;
        }
        else
        {
            flag=0;//������ʾ�ҵ��ؼ��֣�����ҵ��ؼ���Ϊ1,ֱ��continue�����¸�����������������ַ�������ʾ��������
            //while(*p==' ')
              //  p++;
            if((*p=='/'&&p[1]=='/')||*p=='\n')
                break;//����ÿһ��ĩβ��ע��,�Լ�ÿһ��ĩβ�пո�Ȼ���ٻ��е��������Ȼp��ָ�����벻���ĵط�
            if(*p=='{'||*p=='}'||*p=='('||*p==')'||*p=='['||*p==']'||*p=='.'||*p==','||*p==';'||*p=='+'||*p=='-'||*p=='*'||*p=='/'||*p=='&'||*p=='|'||*p=='<'||*p=='>'||*p=='='||*p=='~')
            {
                for(int k=0;k<21;k++)
                {
                    if((*p-symbol[k])==0)
                    {
                        //printf("%c\n",*p);
                        tok[i].tokentype=2;//������2
                        tok[i].tokenword[0]=*p;
                        tok[i].tokenword[1]='\0';//���ǰѵ����ַ�������ַ���
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
                            tok[i].tokentype=1;//�ؼ�����1
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
                        tok[i].tokentype=3;//�ؼ�����3 ���ֳ���
                        strcpy(tok[i].tokenword,q);
                        i++;
                    }
                    //else if(*q=='"')
                    //{
                       // printf("string");
                       // tok[i].tokentype=4;//�ؼ���4 �ַ�������
                       // printf("string");
                       // for(int j=0;q[j+1]!='"';j++)
                       //     tok[i].tokenword[j]=q[j+1];
                       // i++;
                   // }
                    else
                    {
                        tok[i].tokentype=5;//�ؼ���5 ��ʾ��
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
            if(strcmp(tok[j].tokenword,"<")==0)//����.xmlʱ�����Ҫ
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
