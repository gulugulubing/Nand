#define N 200
//N设置太大比如1000有问题,太小比如100，pong中ball的有的句子太长读不进去
#define M 10000

FILE *fp,*fb,fx;
typedef struct tokenn
{
    int tokentype;
    char tokenword[N];
}TOKEN;

typedef struct symbol
{
    char name[N];
    char type[N];
    char kind[N];
    int index;
}SYMBOL;

SYMBOL classtable[M];//static field的数组
SYMBOL subtable[M];//arg local的数组
int NumClassVar;//classtalbe的数组编号
int NumSubVar;//subtable的数组编号
int indexofstatic;
int indexoffield;
int indexofarg;
int indexoflocal;

int narg;//call需要的arg数量
char clsName[N];
char funType[N];//定义函数时 决定了return的处理，主要是void要特殊处理
char subType[N];//call函数时 决定了是不是要将返回值pop temp 0,这里似乎只能更加语法判断，比如DO 后面的就是没有返回值的
char funName[N];
char funKind[N];//method的话 第一个变量要是自己
char letId[N];//let中等号左边的变量
int L;//组成if里的标签
int W;//组成while里的标签


void dblank(char []); //去除每一行的开头空格
void GetElement(char []);//消除T.xml每行中，除了元素以外其他内容
int tokenizer(char[],TOKEN tok[] ,int i);
void WriteToken(TOKEN tok[],FILE *fb,int i);
int VarCount(char []);//return the number of varialbes of given kind already defined in current scope
char *KindOf(char []);
char *TypeOf(char []);
int IndexOf(char[]);
FILE *Define(char[],SYMBOL symtbl[],int *num,FILE *fp,int *indexofstatic, int *indexoflocal,int *indextofarg,int*indexoffield);
FILE *CompileExpression(char[],FILE *fx,FILE *fp);//参数里的char 是最新读取的一行，参数里的fp已经指向下行，返回未读取的第一行
FILE *CompileLet(char[],FILE *fx,FILE *fp);
FILE *CompileExpressionList(char[],FILE *fx,FILE *fp);//最后一个参数用在记录call时需要的arg数量
FILE *CompileTerm(char [],FILE *fx,FILE *fp);
FILE *CompileWhile(char [],FILE *fx,FILE *fp);
FILE *CompileReturn(char [],FILE *fx,FILE *fp);
FILE *CompileIf(char [],FILE *fx,FILE *fp);
FILE *CompileDo(char [],FILE *fx,FILE *fp);
FILE *CompileStatements(char [],FILE *fx,FILE *fp);
FILE *CompileClass(char [],FILE *fx,FILE *fp);
FILE *CompileClassVarDec(char [],FILE *fx,FILE *fp);
FILE *CompileSubroutineDec(char [],FILE *fx,FILE *fp);
FILE *CompileParameterList(char [],FILE *fx,FILE *fp);
FILE *CompileSubroutineBody(char [],FILE *fx,FILE *fp);
FILE *CompilevarDec(char [],FILE *fx,FILE *fp);
void writePush(char [],FILE *fx);
void writePop(char [],FILE *fx);
void writeCall(char [],int nArgs,FILE *fx);
void writeFunction(char [],int nLocals,FILE *fx);//第一个是类名信息
void writeReturn(char [],FILE *fx);//根据第一个参数，判断是不是返回值是空的函数
void writeArithmetic(char[],FILE *fx);
void writeLabel(char[],FILE *fx);
void writeGoto(char[],FILE *fx);
void writeIf(char[],FILE *fx);
//int moveToNextLine(FILE *fp); 不需要了，fgets一次就会自动到下一行
