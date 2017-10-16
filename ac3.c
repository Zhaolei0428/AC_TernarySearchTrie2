#include "ac3.h"
FILE *resultfp;


int main(int argc, char* argv[])
{
	printf("程序开始\n");
	long lSize=buffersize;
	size_t result;
	DWORD start, end;
	start=GetTickCount();
	AC_STRUCT *ACTree = ac_alloc();
    FILE *fp, *strfp;
    if((fp = fopen(argv[2],"r")) == NULL)
    {
    	printf("file pattern.txt open failed!\n");
    	return 0;
	}
    int i=1,j;
    while(!feof(fp))
    {
    	char* p = malloc(sizeof(char) * 100);
    	fgets(p,100,fp);
    	p[strlen(p)-1]='\0';     //去掉换行符 
    	sline[i++]=p;	
	}
	end=GetTickCount();
	printf("读入结束 %ds\n",(end - start)/1000);
	quickSort(1,--i);     //排序 
	end=GetTickCount();
	printf("排序结束 %ds\n",(end - start)/1000);
	flushP(1,i);          //洗牌 
	end=GetTickCount();
	printf("洗牌结束 %ds\n",(end - start)/1000);
	for(j=1;j<=i;j++)
	{
		ac_add_string(ACTree, Patterns[j]->P, Patterns[j]->length, j);
	}
	fclose(fp);
	end=GetTickCount();
	printf("插入结束 %ds\n",(end - start)/1000);
	
	ac_implement(ACTree);
	end=GetTickCount();
    printf("自动机完成 %ds\n",(end - start)/1000);
    printf("%d\n",ACTree->nodeNum);
	if((strfp = fopen(argv[1],"rb"))== NULL)
    {
    	printf("file string.txt open failed!\n");
    	return 0;
	}
	
	if((resultfp = fopen(argv[3],"w")) == NULL)
    {
    	printf("file result.txt open failed!\n");
    	return 0;
	}
	
//	char str[10000];
	while(!feof(strfp))
	{
        result = fread(buffer,1,lSize,strfp);
        printf("%u\n",result);
		search_init(ACTree, result, buffer);
	    ac_search(ACTree);
	}
	fclose(resultfp);
	fclose(strfp);
	end=GetTickCount();
	printf("time: %d s\n",(end - start)/1000);
	return 0;
}//end main 

//将模式串洗牌 
void flushP(int left,int right)
{
	static int i=1;
	int mid=(left+right)/2;
	Pa p=malloc(sizeof(Pattern));
	p->P=sline[mid];
	p->length = strlen(sline[mid]);
	Patterns[i++]=p;
	if(left<mid)
		flushP(left,mid-1);
	if(mid<right)
		flushP(mid+1,right);
} 

//存储树节点的队列，辅助实现faillink
Queue* que_init()
{
	Queue* q;
	if((q=malloc(sizeof(Queue))) == NULL)
		return NULL;
	memset(q,0,sizeof(Queue));
	q->head=0;
	q->tail=0;
	return q;
}
//队列判空 
int empty(Queue* q)
{
	if(q->head==q->tail)
		return 1;
	return 0;
}

//入队，成功返回1，否则返回0 
inline int enqueue(Queue* q, int node)
{
	if((q->head-q->tail+maxn)%maxn==1)
		return 0;
	else
	{
		q->queue[q->tail]=node;
		q->tail=(q->tail+1)%maxn;
		return 1;
    }
} 
//出队，返回出队节点或NULL（队空） 
int dequeue(Queue* q)
{
	if(q->head==q->tail)
		return -1;
	else
    {
    	int head=q->head;
    	q->head=(head+1)%maxn;
    	return q->queue[head];
	}
}


/*
 * ac_alloc
 *
 * Creates a new AC_STRUCT structure and initializes its fields.
 *
 * Returns:  A dynamically allocated AC_STRUCT structure.
 */
AC_STRUCT * ac_alloc()
{
  AC_STRUCT *node;

  if ((node = malloc(sizeof(AC_STRUCT))) == NULL)
    return NULL;
  memset(node, 0, sizeof(AC_STRUCT));
  
  node->root = 0;
  node->nodeNum=1;    //代表只有根节点 
  ts[0].flag=-1;
  
  int i;
  for(i=0;i<=maxnodenum;i++)
  {
  	    ts[i].next=maxnodenum;
		ts[i].lchild=maxnodenum;
		ts[i].rchild=maxnodenum;
		ts[i].faillink=maxnodenum;
		ts[i].outlink=maxnodenum;
  }
  return node;
}

/*
 * Parameters:   node      -  an AC_STRUCT structure
 *               P         -  the sequence
 *               M         -  the sequence length
 *               id        -  the sequence identifier
 */
int ac_add_string(AC_STRUCT *node, char *P, int M, int id)
{
	int i=0,j,flag,nodenum=node->nodeNum;
	int currentNode, preNode, newNode;
	
    if(nodenum==1)  //还未添加模式串 
    {
    	currentNode = 0;
    	for(i;i<M;i++)
	    {
			ts[nodenum].data = P[i];
			ts[nodenum].flag=1; 
			if(i == M)
				ts[nodenum].stateId = id; 
			else
				ts[nodenum].stateId = 0;
			ts[currentNode].next = nodenum;
			currentNode = nodenum;
			nodenum++;
    	}
    	node->nodeNum=nodenum;
    	return 1;
	}
	
	currentNode = preNode = 1;
	while(currentNode!=maxnodenum && i<M)
	{
		if(P[i]==ts[currentNode].data)
		{
			i++;
			preNode = currentNode;
			currentNode = ts[currentNode].next;
			flag=0;
		}
		else if(P[i]<ts[currentNode].data)
		{
			preNode = currentNode;
			currentNode = ts[currentNode].lchild;
			flag = 1;
		} 
		else
		{
			preNode = currentNode;
			currentNode = ts[currentNode].rchild;
			flag = 2;
		} 
	}
	//串已存在，返回1，标id 
	if(i==M)
	{
		ts[preNode].stateId=id;
		return 1;
	} 
	
	ts[nodenum].data = P[i];
	ts[nodenum].flag=1;
	if(i==M-1)
		ts[nodenum].stateId = id;
	else
		ts[nodenum].stateId = 0;
	currentNode = preNode;
	if(flag==0)
	{
		ts[currentNode].next = nodenum++;
		currentNode = ts[currentNode].next;
		i++;
	}
	else if(flag==1)
	{
		ts[currentNode].lchild = nodenum++;
		currentNode = ts[currentNode].lchild;
		i++;
	}
	else
	{
		ts[currentNode].rchild = nodenum++;
		currentNode = ts[currentNode].rchild;
		i++;
	}
	//插入完毕则返回 
	if(i==M)
	{
		node->nodeNum=nodenum;
		return 1;
	}
		
	//将剩余字符插入 
	for(i;i<M;i++)  
	{
		ts[nodenum].data = P[i];
		ts[nodenum].flag=1;
		if(i == M-1)
			ts[nodenum].stateId = id; 
		else
			ts[nodenum].stateId = 0;
		ts[currentNode].next = nodenum++;
		currentNode = ts[currentNode].next;
    }
    node->nodeNum=nodenum;
    return 1;
}

//对trie树添加faillink，完成outputlink，实现自动机 
int ac_implement(AC_STRUCT* node)
{
	int root,currentNode,preNode,lchild,rchild,state,tempNode;
	root=0;
	int flag;
	Queue* q1, *q2, *q3;
	q1=que_init();      //主要队列 
	q2=que_init();      //辅助队列 
	q3=que_init();      //辅助队列 
	if(ts[root].next!=maxnodenum)
	{
		currentNode=1;
		if(!enqueue(q2,currentNode))
			return 0;
		while(!empty(q2))
		{
			currentNode=dequeue(q2);
			ts[currentNode].faillink=root;
			enqueue(q1,currentNode);
			lchild=ts[currentNode].lchild;
			rchild=ts[currentNode].rchild;
			if(lchild!=maxnodenum)
				enqueue(q2,lchild);
			if(rchild!=maxnodenum)
				enqueue(q2,rchild);
		}
		
		while(!empty(q1))
		{
			preNode=dequeue(q1);
			if(ts[preNode].next!=maxnodenum)
			{
				enqueue(q2,ts[preNode].next);
				while(!empty(q2))
				{
					currentNode=dequeue(q2);
					lchild=ts[currentNode].lchild;
			    	rchild=ts[currentNode].rchild;
			    	if(lchild!=maxnodenum)
				    	enqueue(q2,lchild);
			    	if(rchild!=maxnodenum)
			     		enqueue(q2,rchild);
			     	enqueue(q1,currentNode);
			     	
			     	//计算currentNode的faillink
			     	flag=1;
			     	state=preNode;
					while(flag)
					{
						state = ts[state].faillink;
						if(ts[state].next!=maxnodenum)
						{
							enqueue(q3,ts[state].next);
							while(!empty(q3))
							{
								tempNode=dequeue(q3);
								lchild=ts[tempNode].lchild;
								rchild=ts[tempNode].rchild;
								if(lchild!=maxnodenum)
									enqueue(q3,lchild);
								if(rchild!=maxnodenum)
									enqueue(q3,rchild);
								if(ts[tempNode].data==ts[currentNode].data)
								{
									ts[currentNode].faillink=tempNode;
									//计算outlink 
									if(ts[tempNode].stateId!=0)
										ts[currentNode].outlink=tempNode;
									flag=0;
								}
							}
						}
						if(state==root&&flag==1)
						{
							ts[currentNode].faillink=root;
							break;
						}
					}//end while(flag) 
				
				}//end q2
			}
			
		}//end q1
	}
	return 1;
} 

//搜索之前对AC自动机初始化
void search_init(AC_STRUCT* node, long cNum, char* S)
{
	node->startPoint = node->startPoint + node->cNum;
	node->cNum = cNum;
	node->S = S;
	node->currentPoint = 0;
} 


//AC搜索
int ac_search(AC_STRUCT* node)
{
	long i;
	int flag;
	char* S = node->S;
	int currentState,child,outNode;
	if(node->startPoint == 0)
		node->currentState = node->root;
	currentState = node->currentState;
	for(i=0; i<node->cNum; i++)
	{
		child=ts[currentState].next;
		while(ts[currentState].flag!=-1 || child!=maxnodenum)
		{
			if(child==maxnodenum)
			{
				currentState = ts[currentState].faillink;
				child = ts[currentState].next;
				continue;
			}
			
			if(ts[child].data == S[i])
			{
				currentState = child;
				node->currentState = child;
				node->currentPoint = i+1; 
				if(ts[child].stateId!=0)
				{
					outNode=child;
					while(outNode!=maxnodenum)
					{
						fprintf(resultfp,"%s  %d\n",Patterns[ts[outNode].stateId]->P, node->startPoint+node->currentPoint-Patterns[ts[outNode].stateId]->length);
						outNode = ts[outNode].outlink;
					}
				}
				else if(ts[child].outlink!=maxnodenum)
				{
					outNode=ts[child].outlink;
					while(outNode!=maxnodenum)
					{
						fprintf(resultfp,"%s  %d\n",Patterns[ts[outNode].stateId]->P, node->startPoint+node->currentPoint-Patterns[ts[outNode].stateId]->length);
						outNode = ts[outNode].outlink;
					}
				} 
				break;
			}
			else if(S[i] > ts[child].data)
				child = ts[child].rchild;
			else
				child = ts[child].lchild;
				
		}
			 
	}
}

//快排，给模式串排序 
void quickSort(int left, int right)
{
	if(left>=right)
	     return;
	int i=left;
	int j=right; 
	int mid=(i+j)/2;
	char* key=sline[mid];
	sline[mid]=sline[left];
	
	while(i<j)
	{
		while(i<j && strcmp(key,sline[j])<=0)
			j--;
		sline[i]=sline[j];
		while(i<j && strcmp(key,sline[i])>=0)
			i++;
		sline[j]=sline[i];
	}
	sline[i]=key;
	quickSort(left,i-1);
	quickSort(i+1,right);
}

