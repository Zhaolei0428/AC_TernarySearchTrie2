#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>

#define maxn 2000000    //һ��ڵ������� 
#define maxnodenum 5000000    //���ڵ��� 
#define maxpnum 1300000       //���ģʽ���� 
#define buffersize 50000000  //��������������С 


typedef struct{
	char *P;
	int length;
}Pattern, *Pa;              //�洢ģʽ���Ľṹ 

Pa Patterns[maxpnum];       //ģʽ������ 
char* sline[maxpnum];       //��ȡģʽ����ָ������ 
char buffer[buffersize];    //�洢��������buffer 

//************TernarySearchTrie�������Ľṹ******************** 
typedef struct TSNode{
	char data;                          //�ڵ�洢���ֽ�����
	int flag;                           //flagΪ1��ʾ�ýڵ����,���ڵ�Ϊ-1 
	int lchild, rchild;                 //ͬ���ֽ�TSNode 
	int next;                         //��һ�ֽڶ�Ӧ��TSNode 
	int faillink;                     //ʧЧ����
//	int outlink;                      //�������
	int stateId;                        //״̬id�����Ǵ�����Ϊ0��������Ϊ��id 
} TSNode, *TSTree;
TSNode ts[maxnodenum+1]; 

//************AC�Զ����ṹ******************* 
typedef struct {
  int root;                         //���������ڵ� 
  long startPoint, cNum;  //�ļ�λ����ʼ�㣬���������� 
  int  nodeNum;                       //������
  char* S;                             //���������� 
  int currentState;         //��ǰ�������Ľڵ�
} AC_STRUCT;


//***********ΪAC�Զ��������ڴ�**************** 
AC_STRUCT *ac_alloc();
//ΪAC�Զ������ģʽ��
int ac_add_string(AC_STRUCT *node, char *P, int M, int id);
/*
��trie�����faillink�����outputlink��ʵ���Զ��� 
���ִ��󷵻�0�����򷵻�1 
*/ 
int ac_implement(AC_STRUCT* node);
//����֮ǰ��AC�Զ�����ʼ��
void search_init(AC_STRUCT* node, long cNum, char* S); 
//AC����
void ac_search(AC_STRUCT* node);

//***********���нṹ************************** 
typedef struct{
	int queue[maxn];
	int head,tail;
} Queue;
//���г�ʼ�� 
Queue* que_init();
//��ӣ��ɹ�����1�����򷵻�0 
int enqueue(Queue* q, int node);
//���Ӳ��� 
int dequeue(Queue* q);
//�пղ��� 
int empty(Queue* q);

//���� 
void quickSort(int left, int right); 
//��ģʽ��ϴ�� 
void flushP(int left,int right);

