#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jackheader.h"
//并没有能实现嵌套的symboltable，jack语言反正不支持，所以偷懒了。
//这里的方法是要嵌入到Compile中的，用在compile变量的地方，但并不要输出vm代码，但要建立变量表，后面compile时使用
//因为要求子程序中的变量是跟着调用的子程序生或死的 动态的，这就和上面compile过程是动态保持了一致，因此没法静态的读取.xml后建立所有的变量表
//动态建立变量表时就会遇到函数中的参数都要更具上一步变化，和函数外部数据大量交互，因此大量指针运用，感觉不太简洁
FILE *Define(char *tcode,SYMBOL symtbl[],int *num,FILE *fp,int *indexofstatic, int *indexoflocal,int *indexofarg,int*indexoffield)
{


    GetElement(tcode);
    //printf("%s %d %d\n",tcode,*indexoflocal,*num);
    if(strcmp(tcode,"field")==0||strcmp(tcode,"var")==0||strcmp(tcode,"static")==0)//这种情况下连续三个表情依次为kind type name
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
        if(strcmp(tcode,"var")==0)//var按习惯写成local
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
        while(tcode[9]==',')//后面跟着的是同一kind，同一type变量情况,直到出线;结束
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
    else if(tcode[0]!=')')//处理函数参数且不为空的情况，应该只有在compile函数参数时才启动//这里应该只有int char boolean 类 四种情况
    {
        if(strcmp(funKind,"method")==0)//clsName就是this变量的type
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
        while(tcode[9]==',')//后面跟着还有arg
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
    else if(tcode[0]==')'&&strcmp(funKind,"method")==0)//methond只有this一个参数
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




void GetElement(char *tcode)//提前标签中的元素
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
