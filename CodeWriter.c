#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jackheader.h"

void writePush(char *term,FILE *fx)
{
    if(term[0]>='0'&&term[0]<='9')//常量
    {
        fputs("push constant ",fx);
        fputs(term,fx);
        fputs("\n",fx);
    }
    else if(strcmp(term,"true")==0)
    {
        fputs("push constant 1\n",fx);
        fputs("neg\n",fx);
    }
    else if(strcmp(term,"false")==0||strcmp(term,"null")==0)
    {
        fputs("push constant 0\n",fx);
    }
    else if(strcmp(term,"this")==0)
    {
        fputs("push pointer 0\n",fx);
    }
    else//变量
    {
        char strOfIndex[10];
        fputs("push ",fx);
        if(strcmp(KindOf(term),"field")==0)
        {
            fputs("this",fx);
        }
        else
            fputs(KindOf(term),fx);
        fputs(" ",fx);
        sprintf(strOfIndex,"%d",IndexOf(term));
        fputs(strOfIndex,fx);
        fputs("\n",fx);
    }
}
void writePop(char *term,FILE *fx)
{
    char strOfIndex[10];
    fputs("pop ",fx);
    if(strcmp(KindOf(term),"field")==0)
        fputs("this",fx);
    else
        fputs(KindOf(term),fx);
    fputs(" ",fx);
    sprintf(strOfIndex,"%d",IndexOf(term));
    fputs(strOfIndex,fx);
    fputs("\n",fx);
}
void writeCall(char *fname,int nArgs,FILE *fx)//在CompilationEngine里面的DO和TERM里都有
{
    char strN[10];
    fputs("call ",fx);
    fputs(fname,fx);
    fputs(" ",fx);
    sprintf(strN,"%d",nArgs);
    fputs(strN,fx);
    fputs("\n",fx);

    if(strcmp(subType,"void")==0)
        fputs("pop temp 0\n",fx);

}
void writeFunction(char *fname,int nLocals,FILE *fx)
{
    char strN[10];
    char strNfield[10];
    fputs("function ",fx);
    fputs(fname,fx);
    fputs(" ",fx);
    sprintf(strN,"%d",nLocals);
    fputs(strN,fx);
    fputs("\n",fx);

    if(strcmp(funKind,"constructor")==0)
    {
        fputs("push constant ",fx);
        sprintf(strNfield,"%d",VarCount("field"));
        fputs(strNfield,fx);
        fputs("\n",fx);
        fputs("call Memory.alloc 1\n",fx);
        fputs("pop pointer 0\n",fx);
    }
    else if(strcmp(funKind,"method")==0)
    {
        fputs("push argument 0\n",fx);
        fputs ("pop pointer 0\n",fx);
    }

}
void writeReturn(char *tcode,FILE *fx)
{
    if(strcmp(funType,"void")==0)
        fputs("push constant 0\n",fx);
    fputs("return\n",fx);
}
void writeArithmetic(char *op,FILE *fx)
{
    if(op[0]=='+')
        fputs("add\n",fx);
    else if(op[0]=='*')
        fputs("call Math.multiply 2\n",fx);
    else if(op[0]=='-')
        fputs("sub\n",fx);
    else if(op[0]=='&'&&op[1]=='l')
        fputs("lt\n",fx);
    else if(op[0]=='&'&&op[1]=='g')
        fputs("gt\n",fx);
    else if(op[0]=='=')
        fputs("eq\n",fx);
    else if(op[0]=='&'&&op[1]=='a')
        fputs("and\n",fx);
    else if(op[0]=='|')
        fputs("or\n",fx);
    else if(op[0]=='/')
        fputs("call Math.divide 2\n",fx);



}
void writeLabel(char *label,FILE *fx)
{
    fputs("label ",fx);
    fputs(label,fx);
    fputs("\n",fx);
}

void writeGoto(char *label,FILE *fx)
{
    fputs("goto ",fx);
    fputs(label,fx);
    fputs("\n",fx);
}

void writeIf(char *label,FILE *fx)
{
    fputs("not\n",fx);
    fputs("if-goto ",fx);
    fputs(label,fx);
    fputs("\n",fx);
}
