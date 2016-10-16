#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <conio.h>

#define INPUT "DIJKSTRA.INP"
#define OUTPUT "DIJKSTRA.OUT"
#define TMAX 100000 //Max distance

//Neighbor Node
typedef struct tagNbNode
{	
	int v; //label of neighbor vertex
	long w; //weight of edges
	struct tagNbNode *Next; // pointer to Next node	
} N_NODE; 

typedef struct
{
	N_NODE* Head;
}N_LIST; //Neighbor Link List

typedef struct tagVertexNode
{
	int v; //vertex's label
	long d; //distance of path	
	int parent;
	N_LIST NbList; 
	struct tagVertexNode* Next;
	struct tagVertexNode* Pre;
	//tagVertexNode* parent;
} V_NODE; //Vertex Node 

typedef struct V_NODE
{
	V_NODE* Head;	
}V_LIST; //Link List of Vertex 

//Create Neighbor Node 
N_NODE* CreateN_Node(int v,long w)
{
	N_NODE *p;
	p=(N_NODE*) malloc(sizeof(N_NODE));
	p->Next=NULL;
	p->v=v;
	p->w=w;	
	return p;
}	
//Create Vertex Node
V_NODE* CreateV_Node(int v)
{
	V_NODE *p;
	p=(V_NODE*) malloc(sizeof(V_NODE));
	p->Next=NULL;
	p->NbList.Head=NULL;
	p->v=v;
	p->Pre=NULL;
	p->d=TMAX;
	p->parent=-1;
	return p;
}	

//Add a neigbor node into a N_LIST
void AddFirstN_LIST(N_LIST *mylist,N_NODE *q)
{
	q->Next=mylist->Head;
	mylist->Head=q;
}
//Add a vertex into a V_LIST
void AddFirstV_LIST(V_LIST *myList,V_NODE* p)
{
	if(myList->Head==NULL)
	{
		p->Pre=NULL;
		p->Next=NULL;
		//myList.Head=myList.Tail = p;
		myList->Head= p;
		return;
	}
	p->Next=myList->Head;
	p->Pre = myList->Head->Pre;
	myList->Head->Pre = p;
	myList->Head=p;
}

//Check if list is Empty
int IsEmpty(V_LIST q)
{
	return (q.Head==NULL?1:0);
}

//insert a vertex into a sorted list, sorted by distance desc
void Push(V_LIST *q,V_NODE *p)
{
	//quere is empty or p's distance is less than head element
	if(q->Head==NULL || q->Head->d > p->d)
	{
		AddFirstV_LIST(q,p);
		return;
	}
	V_NODE * cur= q->Head;	
	V_NODE * next = cur->Next;
	//move to the right position
	while(next!=NULL && next->d < p->d)
	{
		cur=cur->Next;
		next=next->Next;
	}
	//insert after current position
	p->Pre = cur;
	p->Next = next;
	cur->Next = p;
	if(next!=NULL)
		next->Pre = p;
}
//Remove a Node from a Link list
void RemoveNode(V_LIST *q,V_NODE *p)
{
	if(p->Pre!=NULL)
		p->Pre->Next = p->Next;
	if(p->Next !=NULL)
		p->Next->Pre=p->Pre;
	if(p == q->Head)
		q->Head = p->Next;
	p->Next=p->Pre=NULL;
}
//get the highest priority elements of list. The list is sorted by disctance desc, so that it is first element.
V_NODE* Pop(V_LIST *q)
{	
	if(q->Head==NULL)
		return NULL;
	V_NODE* node = q->Head;	
	RemoveNode(q,node);
	return node;
}

//Add a Neighbor for a Vertex. v is label of neighbor vertex and w i edges's weight.
void AddNeighbor(V_NODE *node,int v,long w)
{
	N_NODE *n = CreateN_Node(v,w);
	AddFirstN_LIST(&node->NbList,n);
}
// Search a Vertex node have label v. return NULL if not found
V_NODE* SearchV(V_LIST g,int v)
{
	V_NODE* cur=g.Head;
	while(cur!=NULL && cur->v!=v)
		cur=cur->Next;
	return cur;
}

//Add an edges (v1,v2) with weight w into graph
void AddOrInsert(V_LIST *g,int v1,int v2,long w)
{
	V_NODE* n1 = SearchV(*g,v1); 
	if(n1==NULL)	 // if graph don't have node v1 yet
	{
		n1= CreateV_Node(v1);		//create new vertex node with label v1, and add to graph
		AddFirstV_LIST(g,n1);
	}
	AddNeighbor(n1,v2,w);

	V_NODE* n2 = SearchV(*g,v2); // similar with v1 above
	if(n2==NULL)	
	{
		n2= CreateV_Node(v2);		
		AddFirstV_LIST(g,n2);
	}
	AddNeighbor(n2,v1,w);	
}

//free memory
void DeleteN_NODE(N_NODE *p)
{
	if(p==NULL)return;
	N_NODE* cur = p;
	N_NODE* next = p->Next;
	while(cur!=NULL)
	{
		free(cur);
		cur=next;
		if(next!=NULL)
			next=next->Next;
	}	
}
void DeleteN_LIST(N_LIST *list)
{
	if(list->Head!=NULL)
		DeleteN_NODE(list->Head);
	list->Head=NULL;
}
void DeleteV_LIST(V_LIST *g)
{
	if(g->Head==NULL) return;
	V_NODE* cur = g->Head;
	V_NODE* next = cur->Next;
	while(cur!=NULL)
	{
		DeleteN_LIST(&cur->NbList);
		free(cur);
		cur=next;
		if(next!=NULL)
			next=next->Next;
	}
	g->Head=NULL;
}


int ReadInput();

//void WriteTest()
//{
//	V_NODE *cur = Graph.Head;
//	while (cur!=NULL)
//	{
//		fprintf(fo,"%d: ",cur->v);
//		N_NODE* p = cur->NbList.Head;
//		while(p!=NULL)
//		{
//			fprintf(fo,"(%d,%d),",p->v,p->w);
//			p=p->Next;
//		}
//		fprintf(fo,"\n");
//		cur=cur->Next;
//	}
//}

void Dijkstra();
void WriteOutput();
void ClearData();

V_LIST Graph; //the init Graph, contain all node with distance = TMAX (disconnect vertexes).
V_LIST uQ; //unvisited priority quere. Contain nodes have distance < TMAX and unvisited, sorted by distance desc
V_LIST vQ; // visted Vertex Node
int start; // start node
int E; //number of Edges

FILE* fi;
FILE* fo;
int main()
{	
	//freopen(INPUT,"r",stdin);
	//freopen(OUTPUT,"w",stdout);					
	fi=fopen(INPUT,"rt");
	fo=fopen(OUTPUT,"wt");
	Graph.Head=NULL;
	while (ReadInput())
	{
		Dijkstra();
		WriteOutput();
		ClearData();
	}
	fclose(fi);
	fclose(fo);

}
//Read input and build graph
int ReadInput()
{		
	fscanf(fi,"%d",&E);
	if(E==0)
		return 0;  
	int i=0;
	for(;i<E;i++)	
	{
		int v1,v2,w;
		fscanf(fi,"%d%d%d",&v1,&v2,&w);
		//Add Edges (v1,v2) weight w into Graph
		AddOrInsert(&Graph,v1,v2,w);
	}	
	//read start node
	fscanf(fi,"%d",&start);
	return 1;
}

void Dijkstra()
{
	//begin with start node
	V_NODE* sNode = SearchV(Graph,start);
	sNode->d=0;
	//remove start node from Un Reach nodes List
	RemoveNode(&Graph,sNode);
	//init Reachable List by startNode
	Push(&uQ,sNode);
	//
	while( !IsEmpty(uQ))
	{
		V_NODE * u = Pop(&uQ);		
		AddFirstV_LIST(&vQ,u); // Add u to Visted List
		
		//for all Neighbor of u.
		N_NODE* nb = u->NbList.Head;
		while (nb!=NULL)
		{
			//seach node V in UnVisted Quere
			V_NODE *vN = SearchV(uQ,nb->v);						
			if(vN!=NULL && (vN->d > u->d+nb->w) )//if found and have a new path with smaller distance than current path
			{
				vN->d = u->d + nb->w; //update distance
				vN->parent = u->v; //update parent: previos node in path.
				//update priority quere uQ
				RemoveNode(&uQ,vN);
				Push(&uQ,vN);
			}else
			{
				vN = SearchV(Graph,nb->v);		//search in unreached nodes	
				if(vN!=NULL && (vN->d > u->d+nb->w) )//if update distance
				{
					vN->d = u->d + nb->w;
					vN->parent = u->v;
					//Remove from TMax distance Node
					RemoveNode(&Graph,vN);
					//push in to priority unvisited quere
					Push(&uQ,vN);
				}
			}
			//move to next Neighbor
			nb=nb->Next;
		}		
	}
}

void WriteOutput()
{
	fprintf(fo,"Result from node %d\n",start);
	V_NODE* cur = vQ.Head; //output All visted node have distance < TMAX
	while (cur!=NULL)
	{
		fprintf(fo,"Node %d: dist= %d, previous= %d\n",cur->v,cur->d,cur->parent);
		cur=cur->Next;
	}
	cur=Graph.Head; // the list Graph contain all disconnect nodes of startNode. distance of them are -1
	while (cur!=NULL)
	{
		fprintf(fo,"Node %d: dist= %d, previous= -1\n",cur->v,cur->d);
		cur=cur->Next;
	}

}
void ClearData()
{
	DeleteV_LIST(&Graph);
	DeleteV_LIST(&uQ);
	DeleteV_LIST(&vQ);
}
