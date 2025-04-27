nodeptr successor(nodeptr root)
{
    if(root==NULL) return NULL;
    if(root->right!=NULL)
    {
        return min_node(root->right);
    }
    else
    {
        nodeptr i=root;
        nodeptr j=root->parent;
        while(j!=NULL)
        {
            //j is the parent of i
            //check if i is the left child of j
            if(j->left==i) return j;
            i=j;
            j=j->parent;
        }
        return NULL;
    }
    /*else
    {
        nodeptr i=root;
        while(i->parent!=NULL) //travel upwards
        {
            //stop when i is the left 
            // child of its parent
            if(i->parent->left == i)
            {
                return i->parent;
            }
            i=i->parent;
        }
        return NULL;
    }*/
}
nodeptr min_node(nodeptr root)
{
    if(root==NULL) return NULL;
    if(root->left==NULL) 
        return root;
    return min_node(root->left);
}

nodeptr min_node(nodeptr root)
{
    if(root==NULL) return NULL;
    nodeptr y=root;
    while(y->left!=NULL)
    {
        y=y->left;
    }
    return y;
}



Deletion of a node from a linked list
Case A: node has no children, directly delete the node

Case B: node has just 1 child, delete the child 
like a linked list 

Case C: a node has 2 children
Step i: Replace the node with its inorder successor
(this does NOT change the inorder, 
    except the node value is deleted)

Step ii: The old successor node will have no left child
invoke Case B

nodeptr search(nodeptr t, int a)
{
    if(t==NULL) return NULL;
    else if(t->data==a) return root;
    else if(a < t->data) search(t->left,a);
    else search(t->right,a);
}

void BSTDelete(nodeptr t,nodeptr *root)
{
    if(t==NULL) return;
    //this case is covered in both below
    //if(t->left==NULL && t->right==NULL)
    //{
    //    transplant(t,NULL,root)
    //}
    //else 
    if(t->right==NULL) transplant(t,t->left,root);
    else if(t->left==NULL) transplant(t,t->right,root);
    else if(t->left!=NULL && t->right!=NULL)
    {
        nodetr u=t->right;
        while(u->left!=NULL) u=u->left;
        if(t->right!=u)
        {
            //replace u with its right child
            transplant(u,u->right,root); //1 and 2
            //make t's children as u's children
            u->right=t->right;//5
            u->right->parent=u;//7
            u->left=t->left; //6
            u->left->parent=u;//8
            //replace t with u as a node
            transplant(t,u,root);//3 and 4
        }
        else if(t->right==u)
        {
            u->left = t->left;
            u->left->parent = u;
            transplant(t,u,root);
        }
    }
}
void transplant(nodeptr t, nodeptr u, nodeptr *root)
{
    if(t==NULL) return NULL;
    if(t->parent!=NULL && t->parent->left==t) 
        t->parent->left=u; //1 left case
    else if(t->parent!=NULL) 
        t->parent->right=u;//1 right case
    if(u!=NULL) u->parent=t->parent; //2
    if(t->parent==NULL) //t is the root
        *root = u;
}

Max-Heap
Data[x]>=Data[left(x)], if left(x) exists
Data[x]>=Data[right(x)], if right(x) exists
OR
Data[x]<=Data[parent(x)], if parent(x) exists

Min-Heap
Data[x]<=Data[left(x)], if left(x) exists
Data[x]<=Data[right(x)], if right(x) exists
OR
Data[x]>=Data[parent(x)], if parent(x) exists

Construct Heap from an Array

Heapify(x): Assume left(x) is a valid heap and
right(x) is also a valid heap, make sub-tree rooted at
x as a valid heap


void Heapify(int x, int Data[], int heapSize)
{
    if(x>=heapSize) return;
    int largest=x;
    if(left(x)<heapSize && Data[left(x)]>Data[largest])
        largest=left(x);
    if(right(x)<heapSize && Data[right(x)]>Data[largest])
        largest=right(x);
    if(largest!=x)
    {
        swap(Data[largest],Data[x]);
        Heapify(largest,Data,heapSize);
    }
}

void HeapifyIterative(int x, int Data[], int heapSize)
{
    if(x>=heapSize) return;
    while(true)
    {
        int largest=x;
        if(left(x)<heapSize && Data[left(x)]>Data[largest])
            largest=left(x);
        if(right(x)<heapSize && Data[right(x)]>Data[largest])
            largest=right(x);
        if(largest!=x)
        {
            swap(Data[largest],Data[x]);
            x=largest;
        }
        else break;
    }
}

void MakeHeap(int Data[], int heapSize)
{
    for(int x=(heapSize-1)/2;x>=0;x--)
    {
        MaxHeapify(x,Data,heapSize);
    }
}



//BST Repeat
If every node strictly has 2 or 0 children
n leaves, n-1 non-leaves, total 2n-1 nodes
if there are n nodes, (n+1)/2 are leaves and
(n-1)/2 are non-leaves

If one node has 1 child, all remaining have 2 or 0 
children
n leaves, n non-leaves, 2n nodes
if there are n nodes, n/2 leaves and n/2 non-leaves



void heapSort(int X[], int n)
{
    MakeHeap(X,n); //O(n)
    int heapSize=n; //O(1)
    while(heapSize>=1) //runs n times
    {
        swap(X[0],X[heapSize-1]); //O(1) * n times, O(n)
        heapSize = heapSize - 1; //O(1) * n times, O(n)
        Heapify(X, heapSize); 
        //O(height of current heap) * n times
        // <= O(log n) * n = O(n log n)
    }
}

memory complexity O(1)
time complexity O(n log n)


int function(struct student X[], int n)
{
    if(X[5].cgpa > X[10].cgpa)
    {
        swap(X[5], X[10]);
    }
}

best case: c1: O(1)
worst case: c1+c2+c3+c4, O(1)
where c1< c2, c3, c4 

Priority Queue
Store max priority queue as a max heap

Front = X[0] - O(1)


void enqueue(struct Queue Q, int priotity)
{
    Q->Data[Q->heapSize] = INT_MIN;
    Q->heapSize++;
    //this heap will be invalid
    increasePriotity(Q, Q->heapSize-1, priority);
}
//complexity O(log n) when invalidNode travels 
//bottom of tree till the top (root)

int Dequeue(struct Queue Q)
{
    if(Q->heapSize<=0) return INT_MIN;
    int a=Q->Data[0];
    swap(Q->Data[0],Q->Data[Q->heapSize-1]);
    Q->heapSize = Q->heapSize-1;
    MaxHeapify(Q->Data,0);
    return a;
}
//O(log n)

void increasePriotity(struct Queue Q, int index, int neePriority)
{
    Q->Data[index]=newPriority;
    //assumption new priority>old priority
    int invalidNode=Q->heapSize-1;
    while(true)
    {
        if(invalidNode>0 && 
            Q->Data[invalidNode]>
                Q->Data[parent(invalidNode)])
        {
            swap(Q->Data[invalidNode], Q->Data[parent(invalidNode)]);
            invalidNode=parent(invalidNode);
        }
        else break;
    }
}



Dictionary
Keys - unique - no 2 data can have the same keys
Values

int data[100];
struct student data[100];

struct student
{
    char rollNo[10]; //key
    char address[1000]; //value1
    char name[100]; //value 2
}


struct student
{
    char rollNo[10]; //key
    char address[1000]; //value1
    char name[100]; //value 2
    ...
}

struct studentNode
{
    struct student s;
    struct studentNode *next;
}

struct studentTreeNode
{
    struct student s;
    struct studentNode *left, *right, *parent;
}

Hash Table
Ambition:
Search - O(1)
Insertions - O(1)
Deletions - O(1)

Direct Addressing
Assume maximum of n keys are possible
(E.g. IMT2024001-IMT2024120 and IMG2024001-IMG2024060)
I reserve a separate memory for every possible key
struct student X[120+60];
I can calculate a unique address for every student 
based on his/her key

Say, first 120 seats (0...119) are for IMT2024
and 121-180 seats (120...179) are for IMG2024

index of key (roll) k
if(k[2]=='T')
  h(k)=(k[7]-'0')*100 + 
    (k[8]-'0')*10 + (k[9]-'0')*1 - 1;
else 
 h(k)=120+
    (k[7]-'0')*100 + (k[8]-'0')*10 + (k[9]-'0')*1 - 1


Say, I am doing first time admissions
key: IIT JEE Application ID
1.5*10^6

AAdhar No: 10^12

In direct addressing, spare reserved is extremely largest
to the actual size of the data, may not fit main memory

Hash Function: Converts keys (a key out of several 
    possible keys) into a range between 
    0 and memory size -1 

Hash Collision: When 2 data with the same hash value
exist in the system, so 2 data compete for 1 reserved 
memory (if m>>n, chances are extremely rare)

Typically, when I design a hash function I do
not have a lot of details of the applications

A good hash function should change with a change 
of every single bit in the key


h(k) = k % m

h(k) = k % 100

5463784594345 % 100 = 45

h(1 0 1 1 1 0 0 1 1 1 0 0 1 0 0 1 1 1) 
=1 0 1 1 1 0 0 1 1 1 0 0 1 0 0 1 1 1  % 1 0 0 0
= 7

h(k) = k % 8

if m is a power of 2, say 2^p, we are only considering
the last p bits and not the entire sequence
which is a bad hash function
m should preferably be large primes


struct node
{
    int key;
    struct node *next;
}
typedef struct node * nodeptr

int h(int k, int m)
{
    return k%m;
}

int h(int k)
{
    return (5*k+k^2+10)%m;
}

h(3)=34%12=10

void insert(nodeptr HashTable[], int k, int m)
{
    int hash=h(k,m);
    nodeptr ll=HashTable[hash];
    if(search(ll,k,m)!=NULL) 
    //avoids insertion of duplicate keys
        HashTable[hash]=insertBeg(ll,k,m);
}

nodeptr search(nodeptr HashTable[], int k, int m)
{
    int hash=h(k,m);
    return LLSearch(HashTable[hash], k)
}

void deleteKey(nodeptr HashTable[], int k, int m)
{
    int hash=h(k,m);
    HashTable[hash]=LLDelete(HashTable[hash], k)
}

nodeptr LLSearch(nodeptr start, int k)
{
    for(nodeptr i=start;i!=NULL;i=i->next)
        if(i->key==k) return i;
    return NULL;
}

void initialize(nodeptr HashTable[], int m)
{
    for(int i=0;i<m;i++)
        HashTable[i]=NULL;
}

int main()
{
    nodeptr HashTable[m];
}


search is O(n)
insertion disregarding search is O(1)
insertion with search is O(n)
deletion disregarding search is O(1)
deletion with search is O(n)

data table size: m
number of elements is: n
load factor: n/m
by probability (with uniform distribution assumption)
complexity of search is O(1+ load factor)
practical complexity is constant


Open Addressing
h(k,i) = (h(k) + i)%m
k: key
i: is the probe attempt (i = 0, 1, 2, , 3)


i=0, h(46,0) = 10 -- collides
i=1, h(46,1) = 11 -- collides
i=2, h(46,2) = 0 -- collides
i=3, h(46,3) = 1 -- collides
i=4, h(46,4) = 2 -- free
HashTable[4]=46

what is the probability that 
x%12 takes a value 0
if x is taken from a uniform distribution 
of positive natural numbers

Ans: 1/12


Primary Clustering: Data clusters into some segments
clusters with large data have more probability 
of getting newer data, and the bigger clusters grow

Quadratic Probing
h(k,i) = (h(k) + c1*i + c2*i^2)%m

Secondary Clustering

void insert(int HashTable[], int k, int m)
{

}

int h(int k, int m) {return k%m;}

int h2(int k, int i, int m)
{
    return (h(k)+i)%m; //linear probing
    //or return (h(h)+c1*i+c2*i*i); quadratic probing 
}

int insert(int HashTable[], int k, int m)
{
    i=0; //probe attempts
    while(i<m)
    {
        int hash=h2(k,i,m);
        if(HashTable[hash]==EMPTY || HashTable[hash]==DELETED)
        {
            HashTable[hash]=k;
            return;
        }
        i=i+1;
    }
    return INT_MIN; //no insetion possible
}

int search(int HashTable[], int k, int m)
{
    i=0; //probe attempts
    while(i<m)
    {
        int hash=h2(k,i,m);
        if(HashTable[hash]==k)
        {
            return HashTable[hash];
        }
        else if(HashTable[hash]==EMPTY) return INT_MIN;
        i=i+1;
    }
    return INT_MIN; //no insetion possible
}

void initialize(int HashTable[], int m)
{
    for(int i=0;i<m;i++) HashTable[i]=EMPTY;
}

int deleteHashTable(int HashTable[], int k, int m)
{
    i=0; //probe attempts
    while(i<m)
    {
        int hash=h2(k,i,m);
        if(HashTable[hash]==k)
        {
            HashTable[hash]=DELETED;
            return;
        }
        else if(HashTable[hash]==EMPTY) return INT_MIN;
        i=i+1;
    }
    return INT_MIN; //no insetion possible
}

# define EMPTY INT_MIN
# define DELETED INT_MIN+1


Double Hashing

Linear Probing: Primary Clustering
Quadratic Probing: Secondary Clustering

Problem in all cases is probe sequence offset 
(which is +i in linear probing and 
+c1*i+c2*i*i in quadratic probing)

is indepedent of the key

There are m! probe sequences possible
for every data, I chose a different probe sequence
depending upon its key

Hash Function
h1(k)=k%m; //primary hash function

//if primary hash function collides, I probe by 
//a secondary hash function
h2(k)=1+k%m2

h(k,i) = h1(k) + i*h2(k) -- hash function
Double Hashing




Graphs G<V,E>:
A collection of vertices (V) and edges (E) 

Say V={v1, v2, v3, ...}
E={<v1,v2>, <v2,v3>}
V={aditya, shivansh, vedashree, ... }
E={<aditya, shivansh>, <vedashree, vijayshni>, 
    <aditya, vedashree> .... }

    v1, v2, v3
    edge e from v1 to v3: e<v1, v3>

    
    Undirected edge: if an edge exists from u to v,
    denoted as e<u,v>, then an edge exists from 
    v to u, denoted as e'<v,u>.

    Undirected Graph - all edges are undirected 

    Directed edge: if an edge exists from u to v,
    denoted as e<u,v>, then an edge may or may not exist
    from v to u, denoted as e'<v,u>.

    Directed Graph - all edges are directed
    
    Weighted graph: every edge e<u,v> has a weight 
    given by w(u,v).

Directed Weighted Graph (e.g. road network graph)
Undirected Weighted Graph (e.g. road network graph of IIITM)
Directed Unweighted Graph (X loves Y)
Undirected Unweighted Graph (walking, timetabling)

If I store all edges as a LinkedList/Array
I will need O(E) time to search whether an edge
exists between two vetices and O(E) lime to 
get all edges from a vertex

In-degree of a vertex: The number of edges that get in
Out-degree of a vertex: The number of edges that get out 

Sink: Vertex that only has incoming edges and no
outgoing edges


Graph G<V,E>
V: list of vertices
E: list of edges

For this course, assume all vertices are integers
numbered 0 to V-1

Adjacency Matrix A:
A[u][v]=1 iff there is an edge from u to v
A[u][v]=0 iff there is no edge from u to v

Get list of all outgoing edges - complexity = O(V)
Check if an edge exists from u to v - complexity = O(1)
Space complexity: O(V^2)

small v is a vertex
capital V is the number or list of vertices

Check(int u, int v)
{
    return A[u][v];
}

void Outgoing(int u, int Out[], int *count)
{
    for(int i=0;i<V;i++)
    {
        if(A[u][i]==1) Out[count++]=i;
    }
    return Out;
}

In an undirected graph
If (u,v) is an edge (A[u][v]=1), then (v,u) is an edge A[v][u]=1
If (u,v) is not an edge (A[u][v]=0), then (v,u) is not edge A[v][u]=0

For an undirected graph, A=A^Transpose, or A is symetric

W[u][v]=weight of edge (u,v) if (u,v) is an edge
INT_MAX otherwise

struct node
{
    int v;
    int w;
    struct node *next;
}

struct node OutgoingEdges[V];

CheckEdge(int u, int v)
{
    nodeptr p=A[u];
    while(p!=NULL)
    {
        if(p->data==v) return 1;
        p=p->next;
    }
    return 0;
}
Complexity O(maximum outdegree)

nodeptr getListOfOutEdges(int u)
{
    return A[u];
}

Space Complexity: O(V+E)

nodeptr A[V]; -- array of linked list O(V) Space
total number of linkedlist nodes - O(E)
O(V+E) 

For Dense Graph, E~O(V^2), adjacency matrix is better
For Sparse Graph, E~O(V), adjacency list is better

Graph-Traversal

d[v]=distance of v from source
parent[v]=penultimate node in path from source to v 

colour[v]= WHITE, if v is unseen (initial default)
            GREY, if v is seen but unexplored
            BLACK, if v is seen and explored

Frontier is a collection of seen but unexplored nodes

Breadth First Search
(Frontier is a queue)








Bredth First Search(Source, AdjList)
{
    //Initialize
    for all vertices v //Complexity O(V)
    {d[v]=Infinity,Parent[v]=NIL,Colour[v]=WHITE}
    d[Source]=0, Parent[Source]=NIL, Colour[Source]=GREY
    Q = empty queue, Enqueue(Q,Source)
    while(!Empty(Q)) //this runs maximum of V times
    {
        u=Front(Q), Dequeue(Q)
        //next loop runs <number of outgoing edges(u)>
        for(all v such that <u,v> is an edge)
        {
            if(Colour[v]==WHITE)
            {
                d[v]=d[u]+1, Parent[v]=u, Colour[v]=GREY
                Enqueue(Q,v)
            }
        }
        Colour[u]=BLACK
    }
}

Complexity O(V) + 
O(sum over u <number of outgoing edges(u))

O(V+E)

PrintPath(int Source, int u, int Parent[], int V)
{
    if(u==Source) print(u)
    else if(Parent[u]=NIL) 
        {print("no path exists"); return;}
    else 
    {
        PrintPath(Source, Parent[u]);
        print(u)
    }
}

Breadth First Search gives the shortest distance 
from Source to u, and the shortest path from
Source to u

//Complexity Outgoing edges of u
DFS-Visit(u)
{
    time = time + 1
    d[u]=time
    Colour[u]=GREY
    for all outgoing edges (u,v)
    {
        if(Colour[v]==WHITE)
        {Parent[v]=u, DFS-Visit(v)}
    }
    time = time+1
    f[u] = time
    Colour[u]=BLACK
}

DFS()
{
    for all vertices V
    {   
        Colour[v]=WHITE, d[v]=Infinity, 
        f[v]=Infinity,Parent[v]=NIL
    }
    for all vertices u
    {
        If(Colour[u]==WHITE)
            DFS-Visit(u)
    }
}

O(V+E)

AVL Tree (Self-balancing) //1->5->6->(10)->15->20->24->30->35             //      10                                               15
Imbalance factor = (depth of right sub-tree) - (depth of left sub-tree)   //     /  \                                             /   \
// Rotation                                                               //    5    15           LEFT - ROTATION                10     20
Trees are Rotation Invariant                                              //        /  \          --------------->              /  \    /  \ 
-1, +1 are not problems                                                   //      12     20                                    5    12  18   25
Left heavy -> child also left heavy = LL case -> we will do right roration/*            /  \ */
Left heavy -> child also left heavy = RR case -> we will do left roration //           18   25
|Anything with imbalance factor| > 2 is a problem -> That node we have to balance-out 
Balancing is doe bottom-up: first balancing the leave -> parent of leaves -> grnadparent of leave

I assume that the tree was balanced at time templateAt time t,
the user asks me to inset/delete a min_nodeI do it, 
but this can cause an imbalance
Say, imbalance happens at node p 
balance(p) = depth(r->right) - depth(r->left)

I nedd to balance p:
Case 1: p is right heavy, and p->right is also right heavy
RR case -> LeftRotate(p)

Case 2: p is left heavy, and p->left is also left heavy
LL case -> RightRotate(p)

Case 3: p is left heavy and p->left is right heavy,
LR case -> LeftRotate(p->left), RightRotate(p)

Case 4: p is right heavy and p->right is left heavy,
RL case -> RightRotate(p->right), LeftRotate(p)

nodeptr BSTInsert(nodeptr root, int a) {
    if(root == NULL) {
        nodeptr r = getNode(); r->data = a;
        r->balance = 0;
    }
    if(a < root->data) root->left- = BSTInsert(root->left, a);
    else if(a < root->data) root->right = BSTInsert(root->right,a);
    else return root; //duplicate key insertion, error

    root->balance = getDepth(root->right) = getDepth(root->left);
    //pseudo-code is remaining
}