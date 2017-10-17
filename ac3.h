#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>

#define maxn 2000000    //一层节点的最大数 
#define maxnodenum 5000000    //最大节点数 
#define maxpnum 1300000       //最大模式串数 
#define buffersize 50000000  //搜索串缓存区大小 


typedef struct{
	char *P;
	int length;
}Pattern, *Pa;              //存储模式串的结构 

Pa Patterns[maxpnum];       //模式串数组 
char* sline[maxpnum];       //读取模式串的指针数组 
char buffer[buffersize];    //存储搜索串的buffer 

//************TernarySearchTrie三叉树的结构******************** 
typedef struct TSNode{
	char data;                          //节点存储的字节数据
	int flag;                           //flag为1表示该节点存在,根节点为-1 
	int lchild, rchild;                 //同级字节TSNode 
	int next;                         //下一字节对应的TSNode 
	int faillink;                     //失效链接
//	int outlink;                      //输出链接
	int stateId;                        //状态id，不是串结束为0，串结束为串id 
} TSNode, *TSTree;
TSNode ts[maxnodenum+1]; 

//************AC自动机结构******************* 
typedef struct {
  int root;                         //三叉树根节点 
  long startPoint, cNum;  //文件位置起始点，搜索串长度 
  int  nodeNum;                       //结点个数
  char* S;                             //搜索串序列 
  int currentState;         //当前搜索到的节点
} AC_STRUCT;


//***********为AC自动机分配内存**************** 
AC_STRUCT *ac_alloc();
//为AC自动机添加模式串
int ac_add_string(AC_STRUCT *node, char *P, int M, int id);
/*
对trie树添加faillink，完成outputlink，实现自动机 
出现错误返回0，否则返回1 
*/ 
int ac_implement(AC_STRUCT* node);
//搜索之前对AC自动机初始化
void search_init(AC_STRUCT* node, long cNum, char* S); 
//AC搜索
void ac_search(AC_STRUCT* node);

//***********队列结构************************** 
typedef struct{
	int queue[maxn];
	int head,tail;
} Queue;
//队列初始化 
Queue* que_init();
//入队，成功返回1，否则返回0 
int enqueue(Queue* q, int node);
//出队操作 
int dequeue(Queue* q);
//判空操作 
int empty(Queue* q);

//快排 
void quickSort(int left, int right); 
//将模式串洗牌 
void flushP(int left,int right);

