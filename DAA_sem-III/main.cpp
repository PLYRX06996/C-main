 // BellmanFord_short.cpp
#include <bits/stdc++.h> // convenience header (includes most std headers)
using namespace std; // bring std symbols into global namespace for brevity
using ll = long long; // alias for 64-bit integers
const ll INF = (1LL<<60); // a large value used as "infinity" for distances

struct Edge { int u,v; int w; }; // simple edge record: endpoints u,v and weight w

int main(){
    ios::sync_with_stdio(false); // speed up i/o when not using C stdio
    cin.tie(nullptr); // untie cin from cout for faster input

    int n, e; // number of vertices and edges
    if(!(cin >> n >> e)) return 0; // read n and e, exit if no input
    vector<Edge> edges(e); // store all edges for Bellman-Ford
    for(int i=0;i<e;i++) cin >> edges[i].u >> edges[i].v >> edges[i].w; // read edges
    int src; cin >> src; // source vertex for shortest paths

    vector<ll> dist(n, INF); // distance vector initialized to INF
    dist[src] = 0; // distance to source is zero

    for(int iter=1; iter<=n-1; ++iter){ // relax edges up to n-1 times
        bool changed = false; // track if any update happened this iteration
        for(auto &ed : edges){ // iterate over each edge u->v with weight w
            if(dist[ed.u] != INF && dist[ed.u] + ed.w < dist[ed.v]){ // relax
                dist[ed.v] = dist[ed.u] + ed.w; // improve distance to v
                changed = true; // mark that we made progress
            }
        }
        if(!changed) break; // early exit if no changes (converged)
    }

    // negative cycle check (one pass)
    bool neg = false; // flag for negative cycle reachable from source
    for(auto &ed : edges){
        if(dist[ed.u] != INF && dist[ed.u] + ed.w < dist[ed.v]){ // if still relaxable
            neg = true; break; // negative cycle detected
        }
    }

    if(neg){
        // following simple output: print single line 'NEG'
        cout << "NEG\n"; // convention used in this snippet
    } else {
        for(int i=0;i<n;i++){
            if(dist[i]==INF) cout << i << " " << "INF\n"; // unreachable
            else cout << i << " " << dist[i] << "\n"; // print distance
        }
    }
    return 0; // end Bellman-Ford program
}
// Dijkstra_short.cpp
#include <bits/stdc++.h> // include needed headers for this section
using namespace std; // bring std into scope
using ll = long long; // alias for long long distances
const ll INF = (1LL<<60); // large sentinel value

int main(){
    ios::sync_with_stdio(false); // speed up i/o
    cin.tie(nullptr);

    int n, e; // vertices and edges
    cin >> n >> e; // read graph size

    vector<vector<pair<int,int>>> g(n); // adjacency list: g[u] = {(v,w), ...}
    for(int i=0;i<e;i++){
        int u,v,w; 
        cin >> u >> v >> w; // read an undirected edge
        g[u].push_back({v,w}); // add v to u's neighbors
        g[v].push_back({u,w}); // add u to v's neighbors
    }

    int src; 
    cin >> src; // source node for Dijkstra

    vector<ll> dist(n, INF); // store shortest known distances
    dist[src] = 0; // distance to source is zero

    priority_queue<pair<ll,int>, vector<pair<ll,int>>, greater<pair<ll,int>>> pq; // min-heap by distance
    pq.push({0, src}); // start from source

    while(!pq.empty()){
        auto du = pq.top(); pq.pop(); // extract smallest-distance candidate
        ll d = du.first; // tentative distance
        int u = du.second; // node id
        if(d != dist[u]) continue; // skip stale entries pushed earlier

        for(const auto &p : g[u]){
            int v = p.first; // neighbor
            int w = p.second; // weight of edge u-v
            if(dist[u] + w < dist[v]){ // relax edge u->v
                dist[v] = dist[u] + w; // update best distance
                pq.push({dist[v], v}); // push updated candidate into heap
            }
        }
    }

    for(int i=0;i<n;i++){
        if(dist[i] == INF) cout << i << " INF\n"; // unreachable
        else cout << i << " " << dist[i] << "\n"; // print distance
    }

    return 0; // end Dijkstra program
}
// FloydWarshall_short.cpp
#include <bits/stdc++.h> // include headers
using namespace std; // std namespace
using ll = long long; // 64-bit alias
const ll INF = (1LL<<60); // large sentinel value

int main(){
    ios::sync_with_stdio(false); // fast i/o
    cin.tie(nullptr);

    int n, e; // vertices and edges
    cin >> n >> e; // read sizes

    vector<vector<ll>> d(n, vector<ll>(n, INF)); // distance matrix
    for(int i=0;i<n;i++) d[i][i] = 0; // zero diagonal

    for(int i=0;i<e;i++){
        int u,v,w;
        cin >> u >> v >> w; // read directed edge u->v weight w
        d[u][v] = min(d[u][v], (ll)w); // keep smallest weight if multiple edges
    }

    for(int k=0;k<n;k++) // intermediate vertex
        for(int i=0;i<n;i++)
            if(d[i][k] != INF)
                for(int j=0;j<n;j++)
                    if(d[k][j] != INF)
                        d[i][j] = min(d[i][j], d[i][k] + d[k][j]); // relax via k

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(d[i][j] == INF) cout << "INF "; // unreachable entry
            else cout << d[i][j] << " "; // print distance
        }
        cout << "\n"; // new line per row
    }

    return 0; // end Floyd-Warshall program
}
// Prim_short.cpp
#include <bits/stdc++.h> // include headers
using namespace std; // std namespace

int main(){
    ios::sync_with_stdio(false); // fast i/o
    cin.tie(nullptr);

    int n,e; // vertices and edges
    cin >> n >> e; // read graph

    vector<vector<pair<int,int>>> g(n); // adjacency list
    for(int i=0;i<e;i++){
        int u,v,w; 
        cin >> u >> v >> w; // read undirected weighted edge
        g[u].push_back({v,w}); // add neighbor
        g[v].push_back({u,w}); // add reverse neighbor
    }

    vector<int> key(n, INT_MAX), parent(n, -1); // key: min edge weight to tree; parent: MST parent
    vector<bool> used(n,false); // used[v] = true when v added to MST

    key[0] = 0; // start from node 0

    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq; // min-heap (key,node)
    pq.push({0,0}); // push start node with key 0

    while(!pq.empty()){
        auto du = pq.top(); pq.pop(); // candidate with smallest key
        ll d = du.first; // current key value
        int u = du.second; // node id
        if(used[u]) continue; // skip if already in MST
        used[u] = true; // mark added to MST

        for(const auto &p : g[u]){
            int v = p.first; // neighbor
            int w = p.second; // weight
            if(!used[v] && w < key[v]){ // if edge improves connection
                key[v] = w; // update best connecting edge weight
                parent[v] = u; // set parent in MST
                pq.push({key[v], v}); // push new candidate
            }
        }
    }

    for(int i=1;i<n;i++){
        cout << parent[i] << " " << i << " " << key[i] << "\n"; // print MST edges
    }

    return 0; // end Prim program
}
// Kruskal_short.cpp
#include <bits/stdc++.h> // include headers
using namespace std; // std namespace

struct Edge { int u,v,w; }; // edge container for Kruskal (local to this section)

struct DSU { // disjoint set union for cycle detection
    vector<int> p, r; // parent and rank arrays
    DSU(int n): p(n), r(n,0) { iota(p.begin(), p.end(), 0); } // initialize each node as its own parent
    int find(int a){ return p[a] == a ? a : p[a] = find(p[a]); } // path-compressed find
    bool unite(int a, int b){ // union by rank
        a = find(a); b = find(b);
        if(a == b) return false; // already same set -> would form cycle
        if(r[a] < r[b]) swap(a,b); // ensure a has larger rank
        p[b] = a; // attach b under a
        if(r[a] == r[b]) r[a]++; // increment rank if equal
        return true; // union successful
    }
};

int main(){
    ios::sync_with_stdio(false); // fast i/o
    cin.tie(nullptr);

    int n,e; // vertices and edges
    cin >> n >> e; // read sizes

    vector<Edge> edges(e); // list of edges to sort
    for(int i=0;i<e;i++) cin >> edges[i].u >> edges[i].v >> edges[i].w; // read edges

    sort(edges.begin(), edges.end(), [](auto &a, auto &b){ return a.w < b.w; }); // sort by weight ascending

    DSU dsu(n); // initialize union-find
    vector<Edge> mst; // collect chosen MST edges

    for(auto &ed : edges){
        if(dsu.unite(ed.u, ed.v)){ // if endpoints in different components
            mst.push_back(ed); // include edge in MST
            if((int)mst.size() == n-1) break; // MST complete
        }
    }

    for(auto &ed : mst)
        cout << ed.u << " " << ed.v << " " << ed.w << "\n"; // print MST

    return 0; // end Kruskal program
}
// MergeSort_short.cpp
#include <bits/stdc++.h> // include headers
using namespace std; // std namespace

void merge(vector<int>& a, int l, int m, int r){
    int n1 = m - l + 1, n2 = r - m; // sizes of left and right halves
    vector<int> L(n1), R(n2); // temporary arrays
    for(int i=0;i<n1;i++) L[i] = a[l+i]; // copy left
    for(int i=0;i<n2;i++) R[i] = a[m+1+i]; // copy right
    int i=0, j=0, k=l; // indices
    while(i<n1 && j<n2) a[k++] = (L[i] <= R[j] ? L[i++] : R[j++]); // merge
    while(i<n1) a[k++] = L[i++]; // copy remaining left
    while(j<n2) a[k++] = R[j++]; // copy remaining right
}

void mergeSort(vector<int>& a, int l, int r){
    if(l >= r) return; // base case
    int m = (l+r)/2; // middle
    mergeSort(a, l, m); // sort left
    mergeSort(a, m+1, r); // sort right
    merge(a, l, m, r); // merge sorted halves
}

int main(){
    int n; 
    cin >> n; // number of elements
    vector<int> a(n); // array to sort
    for(int i=0;i<n;i++) cin >> a[i]; // read elements
    mergeSort(a, 0, n-1); // sort
    for(int x : a) cout << x << " "; // print sorted
    return 0; // end MergeSort program
}
// QuickSort_short.cpp
#include <bits/stdc++.h>
using namespace std;

int partIndex(vector<int>& a, int l, int r){
    int pivot = a[r]; // choose last element as pivot
    int i = l - 1; // index of smaller element
    for(int j=l;j<r;j++){
        if(a[j] <= pivot)
            swap(a[++i], a[j]); // place smaller element to the left
    }
    swap(a[i+1], a[r]); // place pivot in correct position
    return i+1; // return pivot index
}

void quickSort(vector<int>& a, int l, int r){
    if(l < r){
        int p = partIndex(a, l, r); // partition
        quickSort(a, l, p-1); // sort left
        quickSort(a, p+1, r); // sort right
    }
}

int main(){
    int n; cin >> n; // number of elements
    vector<int> a(n);
    for(int i=0;i<n;i++) cin >> a[i]; // read array
    quickSort(a, 0, n-1); // sort
    for(int x : a) cout << x << " "; // print sorted
    return 0; // end QuickSort program
}
// HeapSort_short.cpp
#include <bits/stdc++.h>
using namespace std;

void heapify(vector<int>& a, int n, int i){
    int largest = i; // assume root is largest
    int L = 2*i + 1, R = 2*i + 2; // children indices
    if(L < n && a[L] > a[largest]) largest = L; // left child larger
    if(R < n && a[R] > a[largest]) largest = R; // right child larger
    if(largest != i){
        swap(a[i], a[largest]); // swap and continue heapifying
        heapify(a, n, largest);
    }
}

void heapSort(vector<int>& a){
    int n = a.size();
    for(int i=n/2-1; i>=0; i--) heapify(a,n,i); // build max-heap
    for(int i=n-1; i>0; i--){
        swap(a[0], a[i]); // extract max to end
        heapify(a, i, 0); // heapify reduced heap
    }
}

int main(){
    int n; cin >> n; // number of elements
    vector<int> a(n);
    for(int i=0;i<n;i++) cin >> a[i]; // read array
    heapSort(a); // sort using heap
    for(int x:a) cout << x << " "; // print sorted
    return 0; // end HeapSort program
}
// InsertionSort_short.cpp
#include <bits/stdc++.h>
using namespace std;

void insertionSort(vector<int>& a){
    int n = a.size();
    for(int i=1;i<n;i++){
        int key = a[i], j = i-1; // take element and insert into sorted prefix
        while(j >= 0 && a[j] > key){
            a[j+1] = a[j]; // shift elements right
            j--;
        }
        a[j+1] = key; // insert key
    }
}

int main(){
    int n; cin >> n; // number of elements
    vector<int> a(n);
    for(int i=0;i<n;i++) cin >> a[i]; // read array
    insertionSort(a); // sort
    for(int x:a) cout << x << " "; // print sorted
    return 0; // end InsertionSort program
}
// SelectionSort_short.cpp
#include <bits/stdc++.h>
using namespace std;

void selectionSort(vector<int>& a){
    int n = a.size();
    for(int i=0;i<n-1;i++){
        int minIdx = i; // index of minimum in unsorted portion
        for(int j=i+1;j<n;j++)
            if(a[j] < a[minIdx])
                minIdx = j; // update min index
        swap(a[i], a[minIdx]); // place min at current position
    }
}

int main(){
    int n; cin >> n; // number of elements
    vector<int> a(n);
    for(int i=0;i<n;i++) cin >> a[i]; // read array
    selectionSort(a); // sort
    for(int x:a) cout << x << " "; // print sorted
    return 0; // end SelectionSort program
}
//ternary partition sorting
#include <iostream>
#include <vector>
using namespace std;

pair<int,int> partition(vector<int>& arr, int low, int high) {
    if (arr[low] > arr[low + 1]) { // ensure first two pivots in order
        swap(arr[low], arr[low + 1]);
    }
    int p1 = arr[low], p2 = arr[low + 1]; // two pivot values
    int lt = low + 2; // area for elements < p1
    int i  = low + 2; // current index
    int gt = high; // area for elements > p2

    while (i <= gt) {
        if (arr[i] < p1) {
            swap(arr[i], arr[lt]); // move to left partition
            lt++;
            i++;
        } else if (arr[i] > p2) {
            swap(arr[i], arr[gt]); // move to right partition
            gt--;
        } else {
            i++; // element in middle partition
        }
    }

    lt--;
    gt++;
    swap(arr[low + 1], arr[lt]); // place pivots into final positions
    swap(arr[low], arr[lt - 1]);

    return {lt - 1, lt}; // return indices of pivots
}

void ternaryPartitionSort(vector<int>& arr, int low, int high) {
    if (high - low <= 1) { // base case: 0 or 1 element, or two elements
        if (low < high && arr[low] > arr[high]) {
            swap(arr[low], arr[high]); // sort two elements
        }
        return;
    }

    pair<int,int> pivots = partition(arr, low, high); // partition around two pivots
    int p1Index = pivots.first;
    int p2Index = pivots.second;

    ternaryPartitionSort(arr, low, p1Index - 1); // sort left part
    ternaryPartitionSort(arr, p1Index + 1, p2Index - 1); // sort middle
    ternaryPartitionSort(arr, p2Index + 1, high); // sort right
}

void ternaryPartitionSort(vector<int>& arr) {
    if (!arr.empty()) {
        ternaryPartitionSort(arr, 0, arr.size() - 1); // helper entry
    }
}

int main() {
    int n; // number of elements
    cin >> n; // read n
    vector<int> arr(n); // array to sort
    for (int i = 0; i < n; i++) {
        cin >> arr[i]; // read elements
    }

    ternaryPartitionSort(arr); // sort array
    cout<<"Sorted Array: "; // output header
    for (int x : arr) {
        cout << x << " "; // print sorted elements
    }
    cout << endl; // newline
    return 0; // end program
}