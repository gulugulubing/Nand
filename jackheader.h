#define N 200
//N����̫�����1000������,̫С����100��pong��ball���еľ���̫��������ȥ
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

SYMBOL classtable[M];//static field������
SYMBOL subtable[M];//arg local������
int NumClassVar;//classtalbe��������
int NumSubVar;//subtable��������
int indexofstatic;
int indexoffield;
int indexofarg;
int indexoflocal;

int narg;//call��Ҫ��arg����
char clsName[N];
char funType[N];//���庯��ʱ ������return�Ĵ�����Ҫ��voidҪ���⴦��
char subType[N];//call����ʱ �������ǲ���Ҫ������ֵpop temp 0,�����ƺ�ֻ�ܸ����﷨�жϣ�����DO ����ľ���û�з���ֵ��
char funName[N];
char funKind[N];//method�Ļ� ��һ������Ҫ���Լ�
char letId[N];//let�еȺ���ߵı���
int L;//���if��ı�ǩ
int W;//���while��ı�ǩ


void dblank(char []); //ȥ��ÿһ�еĿ�ͷ�ո�
void GetElement(char []);//����T.xmlÿ���У�����Ԫ��������������
int tokenizer(char[],TOKEN tok[] ,int i);
void WriteToken(TOKEN tok[],FILE *fb,int i);
int VarCount(char []);//return the number of varialbes of given kind already defined in current scope
char *KindOf(char []);
char *TypeOf(char []);
int IndexOf(char[]);
FILE *Define(char[],SYMBOL symtbl[],int *num,FILE *fp,int *indexofstatic, int *indexoflocal,int *indextofarg,int*indexoffield);
FILE *CompileExpression(char[],FILE *fx,FILE *fp);//�������char �����¶�ȡ��һ�У��������fp�Ѿ�ָ�����У�����δ��ȡ�ĵ�һ��
FILE *CompileLet(char[],FILE *fx,FILE *fp);
FILE *CompileExpressionList(char[],FILE *fx,FILE *fp);//���һ���������ڼ�¼callʱ��Ҫ��arg����
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
void writeFunction(char [],int nLocals,FILE *fx);//��һ����������Ϣ
void writeReturn(char [],FILE *fx);//���ݵ�һ���������ж��ǲ��Ƿ���ֵ�ǿյĺ���
void writeArithmetic(char[],FILE *fx);
void writeLabel(char[],FILE *fx);
void writeGoto(char[],FILE *fx);
void writeIf(char[],FILE *fx);
//int moveToNextLine(FILE *fp); ����Ҫ�ˣ�fgetsһ�ξͻ��Զ�����һ��
