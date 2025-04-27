int maxElement(int X[][100], int m, int n)
{
    int maxEle = INT_MIN;
    for(int i=0; i<n; i++)
        for(int j=0; j<m; j++)
            if(X[i][j] > maxEle)
                maxEle = X[i][j];
    return maxEle;
}


as a pseudocode
int maxElement(int X[][], int n, int m)
{
    currentMax = least integer
    for rows from 1 to n
        for col from 1 to m
            if(X[rows][col] > currentMax)
                update currentMax to X[rows][col]
    return currentMax
}


STACK  
DATA: [_, _, _, _, _]
Top = -1

Push(6)
DATA: [6*, _, _, _, _]
Top = 0

Push(8)
DATA: [6, 8*, _, _, _]
Top = 1

Pop
DATA: [6*, _, _, _, _]
Top = 0
return 8

Pop
DATA: [_, _, _, _, _]
Top = -1
return 6

Pop
Underflow
DATA: [_, _, _, _, _]
Top = -1

Push(12)
DATA: [12*, _, _, _, _]
Top = 0

Top?
DATA: [12*, _, _, _, _]
Top = 0
returns 12

Push(5)
DATA: [12, 5*, _, _, _]
Top = 1


Stack as an "Abstract Data Type" Data Structure
<eltype> Stack(<eltype>: s) --data part of the ADT

Operations
Push(s,a) (inserts an element in the stack)
Stack(s), <eltype> a
Precondition: not Full(s)?
Postcondition: s == a +(concatenation) s'

Pop (deletes an element from the stack)
Stack(s)
Precondition: not empty(s)
Postcondition:
Pop==First(s') and s == subseqence(s', 1, len(s)-1)

Top (returns the first element from the stack)
Stack(s)
Precondition: not empty(s)
Postcondition: Top==First(s)

Empty? (checks if stack is empty)
Stack(s)
Precondition: NIL
Postcondition: Empty==(len(s)==0)

Full? (checks if stack is full) 
-- In mathematics sequences can be infinitely long
-- full is not mathematically possible

Implementation of a stack ADT as an array

#define capacity 10000
struct stack
{
    int Data[capacity];
    int top;
}

array of 10 stacks
int Data[10][capacity];
int top[10]

3 stacks
int Data1[capacity], Data2[capacity], Data3[capacity]
int top1, top2, top3;




#define capacity 10000
struct stack
{
    int Data[capacity];
    int top;
}



int push(struct stack *S, int a)
{
    if(S->top >= capacity-1)
    {
        //printf("Stack Overflow Error");
        return INT_MAX;
    }
    S->Data[++S->top] = a;
    return a;
}
int pop(struct stack *S)
{
    if(S->top == -1)
    {
        //printf("underflow");
        return INT_MAX;
    }
    return S->Data[S->top--];
}
int top(struct stack *S)
{
    if(empty(S))
    {
        //printf("underflow");
        return INT_MAX;
    }
    return S->Data[S->top];
}
bool empty(struct stack *S)
{
    return (S->top==-1);
}
bool full(struct stack *S)
{
    return (S->top == capacity-1);
}
void initialize(struct stack *S)
{
    S->top = -1;
}
int main()
{
    struct stack S;
    initialize(&S);
    if(push(&S, 10)==INT_MAX)
    {
        printf("Error!!!");
    }
}

int count(struct stack *S)
{
    int count=0;
    for(int i=0; i<S->top; i++)
    {
        if(S->Data[i] == 3) //THIS IS ILLEGAL 
            count++;
    }
    return count;
}

int count(struct Stack *S)
{
    struct Stack temp;
    initialize(temp);
    int count=0;
    while(!empty(S))
    {
        int t=top(S);
        if(t==3) count++;
        pop(S);
        push(&temp,t);
    }
    while(!empty(temp))
    {
        int t=top(temp);
        pop(temp);
        push(S,t);
    }
    return count;
}




[3 + (2 * { 3 + 5 * [ 1+2 ] } + 4) * 3]
Stack: Empty
See [
Push [ 
Stack: [
See (
Push (
Stack: [ (    
Stack: [ ( {   
Stack: [ ( { [
Stack: [ ( { 
Stack: [ ( 
Stack: [ 
Stack: 

[2*}*}+5*(2+3)]
Stack: [
Stack: [, i see } , [ is not a valid opening for }-- error
(1+{(2*3+(5*6}+7))]
Stack (
Stack ( {
Stack ( { (
Stack ( { ( (
Stack ( { ( ( ,top (, which is not a valid opening for } -- error

bool checkBrackets(char expression[1000])
{
    stuct stack S; 
    initialize(S);
    for all characters c in expression
    {
        if(c is '[', '{' or '(')
            push(S, c); 
            //overflow will never happen if stack 
            //capacity is more than length of expression
        if(c is ']', '}' or ')')
        {
            if(empty(S)) return false; //underflow
            char b=top(S);pop(S);
            if(c is not a valid closing for b as opening)
                return     ;
        }
    }
    return true;
}



( 3 + 5) * (4 - (5 + 2))  + (3 + 2 )
"3 5 +" * "4 5 2 + -" + "3 2 +"
"3 5 + 4 5 2 + - *" + "3 2 +"
"3 5 + 4 5 2 + - * 3 2 + +"
 

 double evaluatePostfix(char postfix[])
 {
    stack S
    initialize(S)
    for all terms c in postfix
    {
        if(term is numeric) //operand
        {
            //assume capacity of stack is larger than expression length
            push(S, string2double(term))
        }
        else if(term is an operator)
        {
            //I assume all binary operators
            if(empty(S)) return DOUBLE_MIN
            double operand2= top(S)
            pop(S)
            if(empty(S)) return DOUBLE_MIN
            double operand1 = top(S)
            pop(S)
            double ans = apply operator on operand 1 and operand 2
            //assume capacity of stack is larger than expression length
            push(S, ans) 
        }
    }
    if size(S)>1, return DOUBLE_MIN
    return top(S)
 }

 struct Queue
 {
    int Data[capacity];
    int front;
    int rear;
 }
 void initialize(struct Queue Q)
 {
    Q->front = -1;
    Q->rear = -1;
 }
 int enqueue(struct Queue *Q, int a)
 {
    if(Q->rear >= capacity-1) return INT_MAX; //overflow error
    Q->Data[++Q->rear] = a;
    //insertion of the first element
    if(Q->front == -1) Q->front = 0; 
    return a;
 }
 int dequeue(struct Queue *Q)
{
    if(Q->front==-1 || Q->front > Q->rear) return INT_MAX; //underflow error
    Q->rear = ??
    return Q-Data[Q->front++];
}
bool empty(struct Queue *q)
{
    if(q->front == -1 || Q->front > Q->rear) return true;
}
bool full(struct *Queue q)
{
    if(Q->rear >= capacity-1) return true;
    return false;
}



Full condition (as well as Empty Condition):
Front = Rear + 1 || 
Front = 0 && Rear == capacity -1

OR

Front = (Rear + 1)%capacity


Problem 1: Empty and FUll have the same condition

Problem 2: Condition is little long to write (without circular arrays)
Front = Rear + 1 || Front = 0 && Rear == capacity -1

Solution to Problem 2 (Condition becomes simple)
Full: pFront == Rear
Empty: pFront == Rear

Solution to Problem 1 (Full and empty are same conditions)
I do not allow the queue to become full
When queue is (capacity - 1) or there is a scope for 1 more data
I call it as an overflow

struct Queue
{
    int Data[capacity];
    int pFront,rear;
}
void initialize()
{
    Q->pFront = capacity-1;
    Q->rear = capacity-1;
}
int enqueue(struct Queue *Q, int a)
{
    //temporarily add a
    int newRear = Q->rear + 1;
    if(newRear == capacity) newRear = 0;
    //if after adding queue is full, return error
    if(newRear == Q->pFront) return INT_MIN;
    Q->rear = newRear;
    Q->Data[Q->rear] = a;
    return a;
}
int dequeue(struct Queue *Q)
{
    if(Q->pFront == Q->rear) return INT_MIN;
    Q->pFront = Q->pFront + 1;
    if(Q->pFront == capacity) Q->front = 0;
    return Q->Data[Q->pFront];
}
int front(struct Queue *Q)
{
    if(Q->pfront == Q->rear) return INT_MIN;
    int index = Q->pFront + 1;
    if(index == capacity) index = 0;
    return Q->Data[index];
}

Objective: To calculate the total time that a program 
takes to execute without running the program 
Analysis of a program

int sumOfArray(int data[],int n)
{
    int sum = 0;
    for(int i=0; i<n; i++)
    {
        sum = sum + data[i];
    }
    return sum;
}

T(N) = aN^2 + b N + c is the computation time
which grows at the rate of a N^2 (approximately)
which is of the order of \theta(N^2)

-- it is of the order of N^2 --

int search(int Data[], int n, int a)
{
    for(int i=0;i<n;i++)
    {
        if(Data[i] == a)
            return i;
    }
    return -1;
}
-- I cannot calculate T(n) because it depends upon contents of 
array and a apart from n

Case I: Worst or largest running time
When a is not in the input array
T(n) [worst case] = aN+b, which is order of N
O(N) -- O stands for worst case

Case II: Best or smallest running time
When a is the first element
T(n) = a
\Omega(1) -- \Omega is best case

Case III: Average running time
When a is in the middle of the array [Assumptions made]
T(n) = aN + b
order of (N)


int Data[1GB] -- not work
int *Data=(int *) malloc()  --- work  

struct node //declaration
{
    int data; //part of definition
    struct node* next; //part of definition
}

1) insert
2) delete
3) search

typedef (struct node *) nodeptr;

int insertAfter(nodeptr p, int a)
{
    if(p == NULL) return INT_MAX; 
    //insertion of 1st element, which is not possible by this code

    //preparation of new node
    nodeptr q=getNode(); //1
    q->data = a; //2
    q->next = NULL; //3

    r = p->next; 
    //for valid p, r can be NULL (insertion of last element) 
    //or valid node

    //insertion
    p->next = q; //a
    q->next = r; //b
    return a;
}

int insertBefore(nodeptr p, int a)
{
    
}


void swap1(int a, int b)
{
    int c = a;
    a = b;
    b = c;

}

void swap(int *a, int *b)
{
    int c = *a;
    *a = *b;
    *b = c;

}

int main()
{
    int a=10, b=20;
    swap1(a,b);
    swap(&a,&b);
    printf("%d %d", a, b);
    return 0;
}

void swapPointerOld(int *c, int *d)
{
    int *e = c;
    c = d;
    d = e;
}

void swapPointerOld2(int *c, int *d)
{
    int *e = c;
    &c = d;
    &d = e;
}

void swapPointer(int **c, int **d)
{
    int *e = *c;
    *c = *d;
    *d = e;
}

int main()
{
    int a=10, b=20;
    int *c=&a;
    int *d=&b;
    swapPointer(&c,&d)
}

void insertFirst(struct node ** start, int a)
{
    struct node *p = getNode();//malloc.... (1)
    p->data = a; //2 (*p).data = a;
    p->next = *start; //3
    *start = p; //4
}

//difference between changing the value of where p points to (*)
// and changing what p points to (**)

main()
{
    struct node *start=NULL;
    insertFirst(start, 10);
    insertFirst(start, 20);
    insertFirst(start, 30);
    insertFirst(start, 40);
    insertFirst(start, 50);
    insertFirst(start, 60);
    insertFirst(start, 70);
}

struct node * insertFirst(struct node * start, int a)
{
    struct node *p = getNode();//malloc.... (1)
    p->data = a; //2 (*p).data = a;
    p->next = *start; //3
    start = p; //4
    return start;
}


main()
{
    struct node *start=NULL;
    insertFirst(&start,10);
}

main()
{
    struct node *start=NULL;
    start=insertFirst(start, 10);
    start=insertFirst(start, 20);
    start=insertFirst(start, 30);
    start=insertFirst(start, 40);
    start=insertFirst(start, 50);
    start=insertFirst(start, 60);
    start=insertFirst(start, 70);
}

main()
{
    nodeptr START=NULL;
    nodeptr LAST=NULL;
    int n;
    scanf("%d",&n);
    for(int i=0;i<n;i++)
    {
        int a;
        scanf("%d",&a);
        if(i==0)
        {
            START = insertFirst(START, a);
            LAST = START;  
        }
        else
            insertAfter(LAST, a);
            LAST = LAST->next;
        }
    }
}


int deleteAfter(nodeptr p)
{
    if(p==NULL || p->next==NULL) return INT_MAX;
    nodeptr q=p->next; 
    int a=q->data;
    p->next=q->next; 
    free(q);
    return a;
}

int deleteFirst(nodeptr *Start)
{
    if(Start==NULL) return INT_MAX;
    nodeptr p=*Start;
    int a=p->data;
    (*Start)=(*Start)->next;//new start could be NULL?
    free(p);
    return a;
}

nodeptr deleteFirst(nodeptr Start)
{
    if(Start==NULL) {printf("ERROR\n");return NULL;}
    nodeptr p=Start;
    Start=Start->next;//new start could be NULL?
    free(p);
    return start;
}

//search
nodeptr search(nodeptr start, int a)
{
    while(start!=NULL)
    {
        if(start->data == a) return start;
        start=start->next;    
    }
    return NULL;
}

void place(nodeptr start, int a)
{
    nodeptr j=NULL;
    for(nodeptr i=start;i!=NULL;i=i->next)
    {
        if(i->data > a) //or (i->data>=data)
        {
            if(j!=NULL) insertAfter(j,a);
            else start=insertFirst(start);
            return;
        }
        j=i;//j is always 1 step behind i
    }
    insertAfter(j,a);
}


void place(nodeptr start, nodeptr a)
{
    if(start->data > a->data) {start=insertFirst(start);return;}
    for(nodeptr j=start;j->next!=NULL;j=j->next)
    {
        if(j->next->data > a->data) //or (i->data>=data)
        {
            insertAfter(j,a);
            return;
        }
    }
    insertAfter(j,a);
}

nodeptr insertionSort(nodeptr start)
{
    nodeptr newLinkedListStart = NULL; //new sorted linked list
    while(start != NULL)
    {
        newLinkedListStart = place(newLinkedListStart, start)
        start = start->next;
    }
    start = newLinkedListStart;
    return start;
}

insertFirst(nodeptr *start, nodeptr nodeToInsert)
{

}
insertAfter(nodeptr p, nodeptr nodeToInsert)
{
    
}

nodeptr deleteData(nodeptr start, int a)
{
    nodeptr j=NULL;
    nodeptr i=start;
    while(i!=NULL)
    {
        if(i->data == a)
        {
            if(j == NULL) 
            {
                start = deleteFirst(start);
                i=start;j=NULL;
            }
            else 
            {
                deleteAfter(j);
                i=j->next;
            }
            //return start;
        }
        else
        {
            j = i; //j is one step behind i at all times
            i = i->next;
        }
    }
    return start;
}

nodeptr deleteData(nodeptr start, int a)
{
    while(start->data == a) {start=deleteFirst(start) return start;}
    for(nodeptr j=start; j->next!=NULL; j=j->next)
    {
        if(j->next->data == a)
        {
            deleteAfter(j);
        }
    }
    return start;
}

Circular Linked List

insertAfter(nodeptr p, int a)
{
    if(p==NULL) return INT_MAX;
    nodeptr n=getNode();
    n->data = a;
    q = p->next; //step 1
    p->next = n; //step 2
    n->next = q; //step 3
    return a;
}
int initialize(nodeptr * p, int a)
{
    nodeptr n=getNode();
    n->data = a;
    n->next = n;
    *p = n;
    return a;
}

nodeptr initialize(int a)
{
    nodeptr n=getNode();
    n->data = a;
    n->next = n;
    return n;
}

main()
{
    nodeptr p=NULL;
    for(int i=0;i<n;i++)
    {
        int a;
        scanf("%d", &a);
        if(i==0) p=initialize(a);
        else {insertAfter(p,10);p=p->next;} 
        //p always points to the last element 
    }
    nodeptr firstElement = p->next;
    while(true)
    {
        if(firstElement->next == firstElement) 
        //circular LL has only 1 element
        {
            //deletion of last element
            deleteAfter(firstElement);
            firstElement = NULL;
        }
        else deleteAfter(firstElement);
    }
}

int deleteAfter(nodeptr p)
{
    if(p==NULL) return INT_MAX; //invalid input
    nodeptr q=p->next; //step 1
    nodeptr r=q->next; //step 2
    p->next = r; //step 3
    int a=q->data;
    free(q);   
    return a;
}

int size(nodeptr p)
{
    if(p==NULL) return 0; 
    int count = 1;
    nodeptr i=p->next;
    while(i!=p)
    {
        i=i->next;
        count++;
    }
    return count;
}


nodeptr concatenate(nodeptr p, nodeptr r)
{
    if(p==NULL && q==NULL) return NULL;
    if(p==NULL) return r;
    if(r==NULL) return p;
    nodeptr q=p->next;
    nodeptr s=r->next;
    p->next=s;
    r->next=q;
    return p;  
}

void play(nodeptr p)
{
    q=p;
    while(q!=NULL)
    {
        print(q->next->word);
        scanf(answer);
        if(q->next->correctAns == answer)
        {
            if(q->next == q)
            {
                deleteAfter(q);
                q=NULL;
            }
            else 
            {
                deleteAfter(q);
            }
        }
        else q=q->next;
    }
}

struct stack
{
    nodeptr top;
}
void initialize(struct stack *S)
{
    S->top=NULL;
}
int push(struct stack *S)
{
    S->top = ... same code as insertion at the beginning---
}
int pop(struct stack *S)
{
    S->top = ... same code as deletion at the beginning---
}

struct Queue
{
    nodeptr front;
    nodeptr rear;
}
void initialize(struct Queue *q)
{
    q->front = NULL;
    q->rear = NULL;
}
int enqueue(struct Queue *q)
{
    q->rear = same as insertion at end;
    if(q->front == NULL) q->front = q->rear;
}

int dequeue(struct Queue *q)
{
    q->front = deletion at the beginning
    if(q->front == NULL) q->rear=NULL;
}


Doubly Linked List
struct node
{
    int data;
    struct node *next;
    struct node *prev;
}

nodeptr searchLastOccurenceOf(nodeptr last, int a)
{
    for(nodeptr i=last; i!=NULL; i=i->prev)
    {
        if(i->data == a)
            return i;
    }
    return NULL;
}

int deleteLinedList(nodeptr p)//, nodeptr *start, nodeptr *last)
{
    if(p==NULL) return INT_MAX;
    int a=p->data;
   nodeptr q=p->prev; //1, q could be NULL
   nodeptr r=p->next; //2, r could be NULL
   if(q!=NULL) q->next = r; //3, if deleting the start, 
                //ensure to update start at main before calling this function
   if(r!=NULL) r->prev = q; //4, if deleting the last,
                //ensure to update last at main before calling this function
   //if(p==*start) *start=r;
   //if(p==*last) *last=q;
   free(p);
   return a; 
}

nodeptr searchAndDelete(nodeptr start, int a)
{
    nodeptr i=start;
    while(start->data == a)
    {
        nodeptr p=start;
        start=start->next;
        deleteLinedList(p);
    }
    while(i->next!=NULL)
    {
        if(i->next->data==a)
        {
            deleteLinedList(i->next);
        }
        else i=i->next;
    }
    return start;
}


nodeptr insertBefore(nodeptr p, int a, nodeptr start)
{
    if(p==NULL) return NULL;
    nodeptr q=getNode(); //1
    q->data = a;
    nodeptr r=p->prev; //2, r could be NULL
    q->next = p; //3
    if(r!=NULL) r->next = q; //4, errror if r is NULL
    q->prev = r; //5
    p->prev = q; //6
    if(p==start) start=q;
    return start;
}

main()
{
    start = insertBefore(p,10,start);
}





main()
{
    nodeptr start=NULL, rear=NULL;
    for(int i=0;i<50000;i++)
    {
        scanf que, correctAns;
        insertAtRear(que,correctAns, &Start, &Rear); 
        //start and rear are updated in the function call
    }
}
practise(nodeptr start)
{
    nodeptr i=start;
    while(i!=NULL)
    {
        print i->que;
        scanf ans
        if(ans==i->correctAns)
        {
            print "yes"
            for(int j=1;j<=skip;j++)
            {
                if(i!=NULL) i=i->next;
                else break;
            }
        }
        else
        {
            print "no"
            //i want to ans all questions in a small neighbourhood
            //from delta questions before i to delta questions after i
            int j=1;
            int countBack=0;
            while(j<=delta)
            {
                if(i->prev!=NULL) {i=i->prev;countBack++;}
                else break;
                j++
            }
            for(j=1;j<=countBack+delta;j++)
            {
                if(j==countBack) continue;
                print i->que;
                scanf ans
                if(ans==i->correctAns) print "yes"
                else print "no"
                if(i!=NULL) i=i->next;
                else break;
            }
        }
    }
}



Recursion

int factorial(int n)
{
    //if(n<0) return INT_MAX;
    //  if(n==0) return 1;
    if(n==1) return 1;
    return n*factorial(n-1);
}

main()
{
    printf("%d",search(X,a,0,n));
}

int search(int X[], int a, int i, int sizeMainProb)
{
    int n = sizeMainProb - i; //size of this sub-problem
    if(i==sizeMainProb) return -1; //base condition
    if(X[i]==a) //Divide: sub-problem 1 (unit sub-problem)
    {
        return i;
    }
    return search(X,a,i+1,n); //Divide: sub-problem 2 (similar/smaller)
    //or of two solutions is naturally implemented
}

int binarySearch(int X[], int a, int l, int h)
{
    int n=h-l+1; //size of this problem
    if(l>h) return -1; 
    int m=(l+h)/2;
    if(a==X[m]) return m; //unit problem
    else if(a<X[m]) return binarySearch(X,a,l,m-1);
    else return binarySearch(X,a,m+1,h); //if a>X[m]
}


best case order of 1
worst case order of log_base_2 (n) = O(lg n)
main()
{
    binarySearch(X,a,0,n-1);
}



void quicksort(int X[], int l, int h)
{
    if(l<=h)
    {
        //divide step
        int pivotIndex = h; //pivot is X[pivotIndex]
        partitionIndex = partition(X,l,h,h); //use naive programming 
        //conquer
        quicksort(X,l,partitionIndex-1); //problem 1
        quicksort(X,partitionIndex+1,h); //problem 2
        //unit problem not to be solved is X[partitionIndex]
        //in combination nothing to do
    }
}



int partition(int X[], int l, int h)
{
    int n=h-l+1;
    int pivot=X[h];
    int p=l-1;
    for(int i=l;i<=h;i++)
    {
        //if(X[i]>pivot) nothing to do;
        if(X[i]<=pivot)
        {
            swap(&X[p+1],&X[i]);
            p=p+1;
        }
    }
    swap(&X[p+1],&X[h]);
    return p+1;
}

void quicksort(int X[],int l, int h)
{
    int n=h-l+1;
    if(l>=h) return;
    int p=partition(X,l,h);
    quicksort(X,l,p-1);
    quicksort(X,p+1,h);
}


Best case: order of n lg n 
Worst case: order of n^2



void MergeSort(int X[], int l, int h)
{
    if(l>=h) return;
    //divide into 2 parts
    int m=(l+h)/2;
    //conquer
    MergeSort(X,l,m); //1st part l...m
    MergeSort(X,m+1,h); //m+1 .... h
    Merge(X,l,m,h);//combine
}

//complexity is order of n lg n (in all cases)
//but it takes order of n, O(n), extra memory

//what if i divide into 3, 4, 5, ... n sub-problems?

void Merge(int X[], int l, int h, int m)
{
    int sizeLeft=m-l+1;
    int sizeRight=h-(m+1)+1;
    int L[sizeLeft], R[sideRight];
    for(int i=0;i<sizeLeft;i++) L[i]=X[l+i];
    for(int i=0;i<sizeRight;i++) R[i]=X[m+1+i];
    int c1=0, c2=0, c=l;
    while(c<=h)
    {
        //case 1: both c1 and c2 are valid
        if(c1<sizeLeft && c2<sizeRight && L[c1]<R[c2]) 
            X[c++]=L[c1++];
        //c1 is valid but c2 is not valid
        else if(c1<sizeLeft) X[c++]=L[c1++];
        //c2 is valid but c1 is not valid
        else //if(c2<sizeRight) 
            X[c++]=R[c2++];
    }
    
}

//complexity of merge is order of n
void Merge(int X[], int l, int h, int m)
{
    int sizeLeft=m-l+1;
    int sizeRight=h-(m+1)+1;
    int L[sizeLeft+1], R[sideRight+1];
    for(int i=0;i<sizeLeft;i++) L[i]=X[l+i];
    L[sizeLeft]=INT_MAX;
    for(int i=0;i<sizeRight;i++) R[i]=X[m+1+i];
    R[sizeRight]=INT_MAX;
    int c1=0, c2=0, c=l;
    while(c<=h)
    {
        if(L[c1]<R[c2]) X[c++]=L[c1++];
        else X[c++]=R[c2++];
    }
    
}

Sorting

void selectionSort(int X[], int n)
{
    for(int i=0; i<n; i++)
    {
        //sorted array is 0...(i-1)
        //unsorted array is i... (n-1)
        //find index of smallest element in unsorted
        int smallestIndex=i;
        for(int j=i; j<n; j++)
        {
            if(X[j] < X[smallestIndex])
            {
                smallestIndex = j;
            }
            swap(X[smallestIndex],X[i]);
        }
    }
}

void insertionSort(int X[], int n)
{
    for(int i=0; i<n; i++)
    {
        //sorted part 0...(i-1)
        //unsorted part i... (n-1)
        //Task: take X[i] and insert it
        //in the already sorted seq 0...(i-1)
        int key=X[i];
        int j=i-1; //loop on integers to move
        while(j>=0 && X[j]>key)
        {
            X[j+1]=X[j];
            j--;
        }
    }
}