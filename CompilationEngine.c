#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jackheader.h"



FILE *CompileClass(char *tcode,FILE *fx,FILE *fp)
{


    fgets(tcode,N,fp);
    //fputs(tcode,fx);//����

    GetElement(tcode);//����Ͱ�������ȡ����������method���this��kind
    strcpy(clsName,tcode);


    fgets(tcode,N,fp);//{


    NumClassVar=0;
    indexofstatic = 0;
    indexoffield = 0;
    fgets(tcode,N,fp);
    //printf("%s",tcode);
    while((tcode[10]=='f'&&tcode[11]=='i')||(tcode[10]=='s'&&tcode[11]=='t'))//��static����field
    {
       fp=CompileClassVarDec(tcode,fx,fp);
       fgets(tcode,N,fp);
       //printf("%s",tcode);
    }

    //for(int i=0;i<NumClassVar;i++)
    //{
      //  printf("CN:%s   ",classtable[i].name);
      //  printf("CK:%s   ",classtable[i].kind);
       // printf("CT:%s   ",classtable[i].type);
       // printf("CI:%d\n",classtable[i].index);
    //}
    //printf("%d  %s   %s    %d\n",VarCount("field"),KindOf("y"),TypeOf("y"),IndexOf("y"));//ÿ����һ�����еı��������table�������

    while(tcode[9]!='}')
    {
        printf("first %s",tcode);
        fp=CompileSubroutineDec(tcode,fx,fp);
        fgets(tcode,N,fp);
        printf("end %s",tcode);
    }

    return fp;//��ʵ���Բ�return����Ϊ���ж�ȡ���Ѿ����
}

FILE *CompileClassVarDec(char *tcode,FILE *fx,FILE *fp)
{
    fp=Define(tcode,classtable,&NumClassVar,fp,&indexofstatic,&indexoflocal,&indexofarg,&indexoffield);
    return fp;
}

FILE *CompileSubroutineDec(char *tcode,FILE *fx,FILE *fp)
{
   NumSubVar=0;//subtable��������,ÿ�α����ӳ���ʱ�⼸������
   indexofarg = 0;
   indexoflocal =0;

   //fputs("<subroutineDec>\n",fx);
   //printf("subroufirst %s ",tcode);
   //fputs(tcode,fx);//keyword function / method/constructor
   GetElement(tcode);
   strcpy(funKind,tcode);


   fgets(tcode,N,fp);
   //fputs(tcode,fx);//void/type

   GetElement(tcode);//void/type
   strcpy(funType,tcode);//void/type

   fgets(tcode,N,fp);
   //fputs(tcode,fx);//id

   strcpy(funName,tcode);

   fgets(tcode,N,fp);
   //fputs(tcode,fx);//(

   fgets(tcode,N,fp);//
   printf("paralist %s",tcode);
   fp=CompileParameterList(tcode,fx,fp);//Compileparaʱ��ΪҪ����)�Ž�����ע����ˣ���Ϊ)������һ���Ŷ�ȡ

   fgets(tcode,N,fp);//����paralistʱ����)
   //fputs(tcode,fx);//)


   fgets(tcode,N,fp);
   printf("bodyfirst %s",tcode);
   fp=CompileSubroutineBody(tcode,fx,fp);
   //fputs("</subroutineDec>\n",fx);
   return fp;
}

FILE *CompileSubroutineBody(char *tcode,FILE *fx,FILE *fp)
{
    //fputs("<subroutineBody>\n",fx);
    //fputs(tcode,fx);//{
    char tempclsname[N];
    strcpy(tempclsname,clsName);//ÿ����һ�Σ�clsname��Ҫ���±�
    fgets(tcode,N,fp);//��ȡvar
    printf("inbody %s",tcode);
    while(tcode[10]=='v')
    {
        fp=CompilevarDec(tcode,fx,fp);
        fgets(tcode,N,fp);
    }
    for(int i=0;i<NumSubVar;i++)
    {
        printf("SN:%s   ",subtable[i].name);
        printf("SK:%s   ",subtable[i].kind);
       printf("ST:%s   ",subtable[i].type);
        printf("SI:%d\n",subtable[i].index);
    }
    printf("%d  %s   %s    %d\n",VarCount("local"),KindOf("game"),TypeOf("game"),IndexOf("game"));//ÿ����һ���ӳ����еı��������table�������

    GetElement(funName);
    strcat(tempclsname,".");
    strcat(tempclsname,funName);//clsName.funName.
    printf("before writefuntion %s\n",tempclsname);
    writeFunction(tempclsname,VarCount("local"),fx);
    printf("AFTER writefuntion \n");
    fp=CompileStatements(tcode,fx,fp);

    fgets(tcode,N,fp);
    //fputs(tcode,fx);//}
    //fputs("</subroutineBody>\n",fx);
    return fp;
}

FILE *CompilevarDec(char *tcode,FILE *fx,FILE *fp)
{
    //fputs("<varDec>\n",fx);

    fp=Define(tcode,subtable,&NumSubVar,fp,&indexofstatic,&indexoflocal,&indexofarg,&indexoffield);

    //fputs("</varDec>\n",fx);
    return fp;
}



FILE *CompileStatements(char *tcode,FILE *fx,FILE *fp)
{
    //fputs("<statements>\n",fx);
    while(tcode[9]!='}')
    {
        if(tcode[10]=='r'&&tcode[11]=='e')
        {
           fp=CompileReturn(tcode,fx,fp);
        }
        else if(tcode[10]=='w'&&tcode[11]=='h')
        {
            fp=CompileWhile(tcode,fx,fp);
        }
        else if(tcode[10]=='i'&&tcode[11]=='f')
        {
            fp=CompileIf(tcode,fx,fp);
        }
        else if(tcode[10]=='l'&&tcode[11]=='e')
        {
             fp=CompileLet(tcode,fx,fp);
        }
        else if(tcode[10]=='d'&&tcode[11]=='o')
        {
            fp=CompileDo(tcode,fx,fp);
        }
        fgets(tcode,N,fp);//����Ҫ���һ���ж�statements�Ƿ������
    }
    fseek(fp,-22,1);//�Ѿ�������<symbol> } </symbol>,�������Ҫ�ں��������ٶ��������˻�ȥ20���ֽڣ������س��ˣ�
    //fputs("</statements>\n",fx);
    return fp;
}


FILE *CompileWhile(char *tcode,FILE *fx,FILE *fp)
{
    static int W=0;
    char strOfW[10];
    char wlabel1[20]="WHILE_EXP";
    char wlabel2[20]="WHILE_END";
    sprintf(strOfW,"%d",W);
    W++;
    strcat(wlabel1,strOfW);
    strcat(wlabel2,strOfW);

    writeLabel(wlabel1,fx);


    //fputs("<whileStatement>\n",fx);
    //fputs(tcode,fx);//while

    fgets(tcode,N,fp);
   // fputs(tcode,fx);//{

    fgets(tcode,N,fp);
    fp=CompileExpression(tcode,fx,fp);

    fgets(tcode,N,fp);
    //fputs(tcode,fx);//}

    writeIf(wlabel2,fx);

    fgets(tcode,N,fp);
    //fputs(tcode,fx);//{

    fgets(tcode,N,fp);
    fp=CompileStatements(tcode,fx,fp);

    fgets(tcode,N,fp);
    //fputs(tcode,fx);//}

    writeGoto(wlabel1,fx);
    writeLabel(wlabel2,fx);

    //fputs("</whileStatement>\n",fx);
    return fp;
}

FILE *CompileLet(char *tcode,FILE *fx,FILE *fp)
{
    //fputs("<letStatement>\n",fx);
    //fputs(tcode,fx);//дһ��terminal token let
    strcpy(subType,"notvoid");//=������subroutinecallһ���з���ֵ
    fgets(tcode,N,fp);//
    //fputs(tcode,fx);//terminal token identifer x
    GetElement(tcode);
    strcpy(letId,tcode);

    fgets(tcode,N,fp);
    if(tcode[9]!='=')//��ȡx���治�ǵȺţ������������
    {
       // fputs(tcode,fx);//[
        writePush(letId,fx);
        fgets(tcode,N,fp);
        fp=CompileExpression(tcode,fx,fp);

        fgets(tcode,N,fp);
        //fputs(tcode,fx);//]
        fputs("add\n",fx);

        fgets(tcode,N,fp);
        //fputs(tcode,fx);//=

        fgets(tcode,N,fp);
        printf("ARRAY before writeEXP %s\n",tcode);
        fp=CompileExpression(tcode,fx,fp);
        printf("ARRAY after writeEXP \n");

        fputs("pop temp 0\n",fx);
        fputs("pop pointer 1\n",fx);
        fputs("push temp 0\n",fx);
        fputs("pop that 0\n",fx);

    }
    else
    {
         //fputs(tcode,fx);//terminal token symbol =

        fgets(tcode,N,fp);
        printf("before writeEXP %s\n",tcode);
        fp=CompileExpression(tcode,fx,fp);
        printf("after writeEXP \n");

        printf("before writeLET %s\n",letId);
        writePop(letId,fx);
        printf("AFTER writeLET \n");
    }
        fgets(tcode,N,fp);

    //fputs(tcode,fx);//terminal token symbol ;

    //fputs("</letStatement>\n",fx);
    return fp;
}

FILE *CompileDo(char *tcode,FILE *fx,FILE *fp)
{
    strcpy(subType,"void");//do���溯������Ҫ����ֵ

    //fputs("<doStatement>\n",fx);
    //fputs(tcode,fx);//do

    fgets(tcode,N,fp);//do����һ��id
    GetElement(tcode);
    char subcall[N];
    int flag=0;//�����Ԫ��type��һ����ʱ,flag=1
    if(IndexOf(tcode)!=-1)//����Ǳ����Ļ� �Ǹ������
    {
        strcpy(subcall,TypeOf(tcode));
        flag=1;
        writePush(tcode,fx);//���������ǵ�һ��arg
    }
    else
        strcpy(subcall,tcode);

    fgets(tcode,N,fp);//do����ڶ���Ԫ��
    //printf("%s",tcode);
    if(tcode[9]=='(')//��ȡargu
    {
        printf("do.methodname\n");
        fputs("push pointer 0\n",fx);//do���� ֻ��һ��Ԫ�� ���Ԫ���ƺ�ֻ���Ǹ�method;function����Ҫ���ϱ������,��fraction������ gcd
        char clsN[N];
        strcpy(clsN,clsName);
        strcat(clsN,".");
        strcat(clsN,subcall);

        fgets(tcode,N,fp);
        fp=CompileExpressionList(tcode,fx,fp);//

        narg++;//�����ǵ�һ��arg���ӽ�ȥ
        writeCall(clsN,narg,fx);
    }
    else if(tcode[9]=='.')//����Ҫ��ȡ�ڶ���id
    {
        strcat(subcall,".");
        fgets(tcode,N,fp);
        GetElement(tcode);
        strcat(subcall,tcode);//���class.subroutineName
        //printf("%s\n",subcall);
        fgets(tcode,N,fp);//�������(
        fgets(tcode,N,fp);//��һ��argu
        fp=CompileExpressionList(tcode,fx,fp);//

        if(flag==1)
            narg++;//�����ǵ�һ��arg���ӽ�ȥ
        writeCall(subcall,narg,fx);
    }

   fgets(tcode,N,fp);////Compileexpressionlistʱ��ΪҪ����)�Ž�����ע����ˣ���Ϊ)������һ���Ŷ�ȡ
   //fputs(tcode,fx);//)


    fgets(tcode,N,fp);//
    //fputs(tcode,fx);//;

    //fputs("</doStatement>\n",fx);
    return fp;
}


FILE *CompileIf(char *tcode,FILE *fx,FILE *fp)
{
    static int L=0;
    char strOfL[10];
    char label1[20]="IF_TRUE";
    char label2[20]="IF_FALSE";
    sprintf(strOfL,"%d",L);
    L++;
    strcat(label1,strOfL);
    strcat(label2,strOfL);
    //fputs("<ifStatement>\n",fx);
    //fputs(tcode,fx);//if

    fgets(tcode,N,fp);//
    //fputs(tcode,fx);//(

    fgets(tcode,N,fp);//
    fp=CompileExpression(tcode,fx,fp);//

    fgets(tcode,N,fp);//
    //fputs(tcode,fx);//)

    writeIf(label1,fx);

    fgets(tcode,N,fp);//
   // fputs(tcode,fx);//{

    fgets(tcode,N,fp);//
    fp=CompileStatements(tcode,fx,fp);

    fgets(tcode,N,fp);//
    //fputs(tcode,fx);//}

    writeGoto(label2,fx);
    writeLabel(label1,fx);

    fgets(tcode,N,fp);//
    if(tcode[10]=='e'&&tcode[11]=='l'&&tcode[12]=='s'&&tcode[13]=='e')//��else�����
    {
        //fputs(tcode,fx);//else

        fgets(tcode,N,fp);//
        //fputs(tcode,fx);//{

        fgets(tcode,N,fp);//
        fp=CompileStatements(tcode,fx,fp);

        fgets(tcode,N,fp);//
        //fputs(tcode,fx);//}
        writeLabel(label2,fx);
        //fputs("</ifStatement>\n",fx);
        return fp;
    }
    else
    {
        writeLabel(label2,fx);
        long offset=(long) (strlen(tcode)+1);//sizeof �����ո��ֹͣ��,Ҫ��strlen,��Ϊstrlen������'\0�����м�1
        //printf("%s\n",tcode);
        //printf("%ld\n",offset);
        fseek(fp,-offset,1);//����else��������һ�����ַ�Ҫ�˻�
        //fputs("</ifStatement>\n",fx);
        return fp;
    }
}

FILE *CompileReturn(char *tcode,FILE *fx,FILE *fp)
{
    //fputs("<returnStatement>\n",fx);
    //fputs(tcode,fx);//return

    fgets(tcode,N,fp);
    if(tcode[9]!=';')
    {
        fp=CompileExpression(tcode,fx,fp);
        fgets(tcode,N,fp);
    }
    //fputs(tcode,fx);
    writeReturn(funType,fx);
    //fputs("</returnStatement>\n",fx);
    return fp;
}


FILE *CompileExpression(char *tcode,FILE *fx,FILE *fp)//simple version
{
    //fputs("<expression>\n",fx);
    fp=CompileTerm(tcode,fx,fp);

    fgets(tcode,N,fp);
    while(tcode[9]=='+'||tcode[9]=='-'||tcode[9]=='*'||tcode[9]=='/'||(tcode[9]=='&'&&(tcode[10]=='a'||tcode[10]=='l'||tcode[10]=='g'))||tcode[9]=='|'||tcode[9]=='=')
    {
        //printf("aris %c\n",tcode[9]);
        //fputs(tcode,fx);//op

        char tempop[N];
        strcpy(tempop,tcode);
        GetElement(tempop);

        fgets(tcode,N,fp);
        fp=CompileTerm(tcode,fx,fp);

        writeArithmetic(tempop,fx);//�����Ҫ�������ֺ���

        fgets(tcode,N,fp);
    }

    long offset=(long) (strlen(tcode)+1);//sizeof �����ո��ֹͣ��,Ҫ��strlen,��Ϊstrlen������'\0�����м�1
    fseek(fp,-offset,1);//����else��������һ�����ַ�Ҫ�˻�
   // fputs("</expression>\n",fx);
    return fp;
}

FILE *CompileExpressionList(char *tcode,FILE *fx,FILE *fp)//���һ���������ص���ʱ��Ĳ�������
{
    //fputs("<expressionList>\n",fx);
    narg=0;//callʱ��Ҫ��¼��
    while(tcode[9]!=')')
    {
        if(tcode[9]!=',')
         {
            narg++;
            fp=CompileExpression(tcode,fx,fp);
         }
        fgets(tcode,N,fp);
    }
    fseek(fp,-22,1);
    //fputs("</expressionList>\n",fx);
    return fp;
}

FILE *CompileParameterList(char *tcode,FILE *fx,FILE *fp)
{
    //fputs("<parameterList>\n",fx);

    fp=Define(tcode,subtable,&NumSubVar,fp,&indexofstatic,&indexoflocal,&indexofarg,&indexoffield);

    fseek(fp,-22,1);
    //fputs("</parameterList>\n",fx);
    return fp;
}

FILE *CompileTerm(char *tcode,FILE *fx,FILE *fp)
{
    //fputs("<term>\n",fx);
   // fputs(tcode,fx);
    char tempterm[N];
    strcpy(tempterm,tcode);
    if(tcode[9]=='-'||tcode[9]=='~')//unaryOp term
    {
        //printf("intounaryOP %c",tcode[9]);
        char tmp=tcode[9];

        fgets(tcode,N,fp);
        fp=CompileTerm(tcode,fx,fp);//����term

        if(tmp=='~')
            fputs("not\n",fx);
        else
            fputs("neg\n",fx);
    }
    else if(tcode[9]=='(')//(expression)
    {
        fgets(tcode,N,fp);
        fp=CompileExpression(tcode,fx,fp);

        fgets(tcode,N,fp);
       // fputs(tcode,fx);//)
    }//���ϲ���Ҫ����һ��token�����ж���ô��������������˻�ȥ
    else
    {
        char temp1,temp2;
        temp1=tempterm[1];
        temp2=tempterm[2];//�����Ƿ���stringconstant

        GetElement(tempterm);
        fgets(tcode,N,fp);
        if(tcode[9]=='[')//var[expression]
        {
           // fputs(tcode,fx);//[
            writePush(tempterm,fx);
            fgets(tcode,N,fp);
            fp=CompileExpression(tcode,fx,fp);

            fputs("add\n",fx);
            fputs("pop pointer 1\n",fx);
            fputs("push that 0\n",fx);

            fgets(tcode,N,fp);
           // fputs(tcode,fx);//]
        }
        else if(tcode[9]=='.')//className|varName.subroutinename(expressionlist)
        {
           // fputs(tcode,fx);//.

            fgets(tcode,N,fp);
            GetElement(tcode);
            char subName[N];
            strcpy(subName,tcode);
            printf("subName %s\n",subName);

            int flag=0;//�����Ԫ��type��һ����ʱ,flag=1
            printf("callclass1 %s\n",tempterm);
            if(IndexOf(tempterm)!=-1)//����һ������,����Ӧ���������
            {
                printf("callclass2 %s\n",tempterm);
                writePush(tempterm,fx);//���������ǵ�һ��arg,����һ�������
                strcpy(tempterm,TypeOf(tempterm));
                flag=1;
            }

            strcat(tempterm,".");
            strcat(tempterm,subName);
            printf("callName %s\n",tempterm);

            //fputs(tcode,fx);//subroutinName

            fgets(tcode,N,fp);
            //fputs(tcode,fx);//(

            fgets(tcode,N,fp);
            fp=CompileExpressionList(tcode,fx,fp);
            if(flag==1)
                narg++;//�����ǵ�һ��arg���ӽ�ȥ
            writeCall(tempterm,narg,fx);
            fgets(tcode,N,fp);
            //fputs(tcode,fx);//)
        }
        else if(tcode[9]=='(')//subroutinename(expressionlist)
        {
            //fputs(tcode,fx);//(

            fgets(tcode,N,fp);
            fp=CompileExpressionList(tcode,fx,fp);

            fgets(tcode,N,fp);
           // fputs(tcode,fx);//)
        }
        else//constant|varName��ֻ��һ��token
        {
            long offset=(long) (strlen(tcode)+1);//sizeof �����ո��ֹͣ��,Ҫ��strlen,��Ϊstrlen������'\0�����м�1
            fseek(fp,-offset,1);
            //GetElement(tempterm);
            if(temp1=='s'&&temp2=='t')
            {
                fputs("push constant ",fx);
                char Nchar[N];
                sprintf(Nchar,"%d",((int) strlen(tempterm)));
                fputs(Nchar,fx);
                fputs("\n",fx);
                fputs("call String.new 1\n",fx);

                for(int i=0;tempterm[i]!='\0';i++)
                {
                    fputs("push constant ",fx);
                    sprintf(Nchar,"%d",(tempterm[i]-0));
                    fputs(Nchar,fx);
                    fputs("\n",fx);
                    fputs("call String.appendChar 2\n",fx);
                }
            }
            else
                writePush(tempterm,fx);
        }//����Ҫ����ʲô
    }
    //fputs("</term>\n",fx);
    return fp;
}

/*����Ҫ�ˣ�fgetsһ�ξͻ��Զ�����һ��
int moveToNextLine(FILE *fp)
{
    int c;

    if(fp == NULL) return -1;//�ļ�ָ��Ƿ�
    while(1)
    {
        c = fgetc(fp);
        if(c == EOF) return EOF;//���ļ�β����������һ��
        if(c == '\n') break;//������Ҫ�Ļ��з���
    }

    return 0;
}
*/
