#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jackheader.h"
//��û����ʵ��Ƕ�׵�symboltable��jack���Է�����֧�֣�����͵���ˡ�
//����ķ�����ҪǶ�뵽Compile�еģ�����compile�����ĵط���������Ҫ���vm���룬��Ҫ��������������compileʱʹ��
//��ΪҪ���ӳ����еı����Ǹ��ŵ��õ��ӳ����������� ��̬�ģ���ͺ�����compile�����Ƕ�̬������һ�£����û����̬�Ķ�ȡ.xml�������еı�����
//��̬����������ʱ�ͻ����������еĲ�����Ҫ������һ���仯���ͺ����ⲿ���ݴ�����������˴���ָ�����ã��о���̫���
FILE *Define(char *tcode,SYMBOL symtbl[],int *num,FILE *fp,int *indexofstatic, int *indexoflocal,int *indexofarg,int*indexoffield)
{


    GetElement(tcode);
    //printf("%s %d %d\n",tcode,*indexoflocal,*num);
    if(strcmp(tcode,"field")==0||strcmp(tcode,"var")==0||strcmp(tcode,"static")==0)//�������������������������Ϊkind type name
    {
        char tempkind[N];
        char temptype[N];

        if(strcmp(tcode,"field")==0)
        {
            symtbl[*num].index=(*indexoffield)++;

        }
        else if(strcmp(tcode,"static")==0)
        {
            symtbl[*num].index=(*indexofstatic)++;

        }
        else if(strcmp(tcode,"var")==0)
        {
            symtbl[*num].index=(*indexoflocal)++;

        }
        //printf("kind:%s\n",tcode);
        if(strcmp(tcode,"var")==0)//var��ϰ��д��local
        {
            strcpy(tempkind,"local");
            strcpy(symtbl[*num].kind,"local");
            printf("symtbl %s\n",symtbl[*num].kind);
        }
        else
        {
            strcpy(tempkind,tcode);
            strcpy(symtbl[*num].kind,tcode);
        }


        fgets(tcode,N,fp);
        GetElement(tcode);
         //printf("type:%s\n",tcode);
        strcpy(temptype,tcode);
        strcpy(symtbl[*num].type,tcode);

        fgets(tcode,N,fp);
        GetElement(tcode);
        //printf("name:%s\n",tcode);
        strcpy(symtbl[*num].name,tcode);


        fgets(tcode,N,fp);
        while(tcode[9]==',')//������ŵ���ͬһkind��ͬһtype�������,ֱ������;����
        {

            strcpy(symtbl[++(*num)].kind,tempkind);
            strcpy(symtbl[*num].type,temptype);

            fgets(tcode,N,fp);
            GetElement(tcode);
            strcpy(symtbl[*num].name,tcode);
            if(strcmp(tempkind,"field")==0)
                symtbl[*num].index=(*indexoffield)++;
            else if(strcmp(tempkind,"static")==0)
                symtbl[*num].index=(*indexofstatic)++;
            else if(strcmp(tempkind,"local")==0)
                symtbl[*num].index=(*indexoflocal)++;
            fgets(tcode,N,fp);
        }
        ++(*num);
    }
    else if(tcode[0]!=')')//�����������Ҳ�Ϊ�յ������Ӧ��ֻ����compile��������ʱ������//����Ӧ��ֻ��int char boolean �� �������
    {
        if(strcmp(funKind,"method")==0)//clsName����this������type
        {
            strcpy(symtbl[(*num)++].kind,"argument");
            strcpy(symtbl[(*num)++].type,clsName);
           strcpy(symtbl[(*num)++].name,"this");
           symtbl[(*num)++].index=(*indexofarg)++;
        }

        strcpy(symtbl[*num].kind,"argument");
        strcpy(symtbl[*num].type,tcode);

        fgets(tcode,N,fp);
        GetElement(tcode);
        strcpy(symtbl[*num].name,tcode);

        symtbl[*num].index=(*indexofarg)++;

        fgets(tcode,N,fp);
        while(tcode[9]==',')//������Ż���arg
        {
            strcpy(symtbl[++(*num)].kind,"argument");

            fgets(tcode,N,fp);
            GetElement(tcode);
            strcpy(symtbl[*num].type,tcode);

            fgets(tcode,N,fp);
            GetElement(tcode);
            strcpy(symtbl[*num].name,tcode);

            symtbl[*num].index=(*indexofarg)++;
            fgets(tcode,N,fp);
        }
        ++(*num);
    }
    else if(tcode[0]==')'&&strcmp(funKind,"method")==0)//methondֻ��thisһ������
    {

        strcpy(symtbl[*num].kind,"argument");
        strcpy(symtbl[*num].type,clsName);
        strcpy(symtbl[*num].name,"this");
        symtbl[*num].index=(*indexofarg)++;
        ++(*num);
    }
    return fp;

}

int VarCount(char *kind)
{
    int count=0;
    if(strcmp(kind,"static")==0||strcmp(kind,"field")==0)
    {
        for(int i=0;i<NumClassVar;i++)
        {
            if(strcmp(classtable[i].kind,kind)==0)
                    count=classtable[i].index+1;
        }
    }
    else if(strcmp(kind,"argument")==0||strcmp(kind,"local")==0)
    {
        for(int i=0;i<NumSubVar;i++)
        {
            if(strcmp(subtable[i].kind,kind)==0)
                    count=subtable[i].index+1;
        }
    }
    return count;
}

char *KindOf(char *name)
{
    char *p=NULL;
    for(int i=0;i<NumSubVar;i++)
    {
        if(strcmp(subtable[i].name,name)==0)
         {
             p=subtable[i].kind;
             return p;
         }
    }
    for(int i=0;i<NumClassVar;i++)
    {
        if(strcmp(classtable[i].name,name)==0)
         {
             p=classtable[i].kind;
             return p;
         }
    }
    return p;
}

char *TypeOf(char *name)
{
    char *p=NULL;
    for(int i=0;i<NumSubVar;i++)
    {
        if(strcmp(subtable[i].name,name)==0)
         {
             p=subtable[i].type;
             return p;
         }
    }
    for(int i=0;i<NumClassVar;i++)
    {
        if(strcmp(classtable[i].name,name)==0)
         {
             p=classtable[i].type;
             return p;
         }
    }
    return p;
}

int IndexOf(char *name)
{
    int p;
    for(int i=0;i<NumSubVar;i++)
    {
        if(strcmp(subtable[i].name,name)==0)
         {
             p=subtable[i].index;
             return p;
         }
    }
    for(int i=0;i<NumClassVar;i++)
    {
        if(strcmp(classtable[i].name,name)==0)
         {
             p=classtable[i].index;
             return p;
         }
    }
    return -1;
}




void GetElement(char *tcode)//��ǰ��ǩ�е�Ԫ��
{
    char *p=tcode;
    char *q=tcode;

    while(*p!=' ')
        p++;
    p++;
    while(*p!=' '||(*p==' '&&(p[1]!='<')))
    {
        *q=*p;
        p++;
        q++;
    }
    *q='\0';
}
