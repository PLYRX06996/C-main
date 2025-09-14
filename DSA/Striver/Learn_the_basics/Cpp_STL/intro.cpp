// #include <iostream>
// #include <vector>
// #include <utility>
// #include <string>
// #include<list>
// #include<set>
// #include<queue>
// #include<st>
// #include<unordered_map>
// #include<unordered_set>
// #include<map>
// #include<algorithm>
#include<bits/stdc++.h>
using namespace std;

/*
STL has mainly four parts:
Algorithms
Containers
Functions
Iterators.

C++ STL containers:
Sequence containers [Arrays, Vectors, Lists]
Associative containers[sets, maps]
Unordered containers[unordered set/multi set, unordered maps]
Container Adapters [st, queue, prority queue]*/


void explainpair()
{
    pair<int, int> p = {1, 3};
    cout << p.first << " " << p.second;

    pair<int, pair<int, int>> q = {1, {2, 3}};
    cout << q.first << " " << q.second.first << " " << q.second.second << endl;

    pair<int, int> arr[] = {{1, 6}, {2, 3}, {4, 5}};
}

int main()
{

    vector<int> v;
    v.push_back(1);
    v.emplace_back(2);

    vector<pair<int, int>> V;
    V.push_back({1, 2});
    V.emplace_back(1, 2);

    vector<int> v(5, 100);
    vector<int> v(5);

    vector<int> v1(5, 30);
    vector<int> v2(v1);

     v = {10, 20, 30, 40, 50};
    vector<int>::iterator it = v.begin(); // this points to the first memory location of the vecctor dtt
    cout << *(it) << endl;

    it = it + 2;
    cout << *(it) << endl;

    vector<int>::iterator it = v.end();  // it doesn't point to the last element of the vector, instead it points to the next memory address after the last element.
    vector<int>::iterator it = v.rend(); // this is a reverse iterator if we do it++ it will shift backwards
    // or this as if we apply r iterator then we should forst reverse the vector elements and then do pinter iterations, so this points to memory address location before 10
    vector<int>::iterator it = v.rbegin(); // this point to memory address location of 50

    cout << v[0] << " " << v.at(0);
    cout << v.back() << endl; //  last element of the vector

    for (vector<int>::iterator it = v.begin(); it != v.end(); it++)
    {
        cout << *(it) << " ";
    }

    for (auto it = v.begin(); it != v.end(); it++)
    {
        cout << *(it) << " ";
    }

    for (auto it : v)
    {
        cout << (it) << " ";
    }
    // {10, 20, 12, 23, 30}
    v.erase(v.begin() + 1);                // 20 will be ersed
    v.erase(v.begin() + 2, v.begin() + 4); // 12, 23 will be erased new = {10, 20, 30}

    // Insert function
    vector<int> v(2, 100);          //{100, 100}
    v.insert(v.begin(), 300);       // {300, 100, 200}
    v.insert(v.begin() + 1, 2, 10); //{300, 10, 10, 100, 200}

    vector<int> copy(2, 50);                       // {50, 50}
    v.insert(v.begin(), copy.begin(), copy.end()); // {50, 50, 300, 10, 100, 200}

    cout << v.size(); // gives the size of the vector

    v.pop_back(); // pops out the last elemt from the vector

    // if v1 and v2 have same size eg. v1 -> {10, 20} & v2 -> {30, 40}
    v1.swap(v2); // v1 -> {30, 40} & v2 -> {10, 20}

    v.clear(); // erases the entire vector

    cout << v.empty(); // true or false based on v

    //--------------------------------------------------------------
    list<int> ls;
    ls.push_back(2);    //{2}
    ls.emplace_back(4); // {2, 4}
    ls.push_front(5);   // {5, 2, 4}
    ls.emplace_front();
    // {2, 4};

    deque<int> dq; // same functions same as vector and deque

    stack<int> st;    // LIFO
    st.push(2);       //{2}
    st.push(3);       //{3, 2}
    st.emplace(5);    //{5, 3, 2}
    cout << st.top(); // 5
    st.push(7);       //{7, 5, 3, 2}
    st.pop();         // 7 {5, 3, 2}
    st.size();        // 3
    st.empty();       // false

    queue<int> q; // FIFO
    q.push(1);    //{1}
    q.push(5);    //{1, 5}
    q.push(3);    //{1, 5, 3}
    q.push(8);    //{1, 5, 3, 8}
    q.back();     // 4
    q.front();    // 1
    q.pop();      // 1 {5, 3, 8}

    // max heap
    priority_queue<int> pq; // max element will stay at the top, lexicographically largest string
    // it is not a linear DS, in it a tree DS is maintained
    // main functions push, pop, top, empty, swap, size
    pq.push(5); //{5}
    pq.push(2); //{5, 2}
    pq.push(8); //{8, 5, 2}
    pq.push(6); //{8, 6, 5, 2}

    pq.top(); // 8 -> always the maximun element
    pq.pop(); // deletes the max element

    // minimum heap
    priority_queue<int, vector<int>, greater<int>> pq;
    pq.push(5); //{5}
    pq.push(2); //{2, 5}
    pq.push(8); //{2, 5, 8}
    pq.push(6); //{2, 5, 6, 8}

    pq.top(); // 2 -> always the minimum element
    pq.pop(); // deletes the min element

    // //********important*******
    // /*push and pop operations always take O(log(n)) and top O(1) */

    set<int> s; // it always stores the element in the sorted order and all the elements are unique
    // it is also maintained in the tree manner
    s.insert(1);  //{1}
    s.emplace(2); //{1, 2}
    s.insert(2);  //{1, 2}
    s.insert(4);  //{1, 2, 4}
    s.insert(3);  // {1, 2, 3, 4}

    auto it = s.find(3); // it return an pointer/iteratior which points at 3

    auto it = s.find(6); // if an element is not there in the set then it will return s.end();
    // if we erase any element from the set it will erase it and will maintain the sorted order... ex. s.erase(3); // 3 {1, 2, 4}

    // another example {1, 2, 3, 4, 5}
    auto it1 = s.find(2);
    auto it2 = s.find(4);
    s.erase(it1, it2); // {1, 4, 5} after erase == s -{ [first, last) }
    // rest all the operations are similar to vector
    s.count(1); // counts the occurences of a particlular element
    // in set every operations happen in log(n)

    multiset<int> ms;
    ms.insert(1);         // {1}
    ms.insert(1);         // {1, 1}
    ms.insert(1);         // {1, 1, 1}
    ms.insert(2);         // {1, 1, 1, 2}
    ms.erase(1);          // all the occurences of 1 will be deleted
    ms.erase(ms.find(1)); // find the 1st occurence of 1 and point the iterator and erase it
    ms.erase(ms.find(1), ms.find(1)++);
    // rest all the operations are similar to set

    unordered_set<int> us; // there is noo rule for sorting, nut all the elemnts will be unique
    // the time complexity for all the operations are O(1)
    //  worst case: once in a blue moon -> linear-> O(n)

    // //******************************************************** */
    map<int, int> mpp; // key(unique) and value
    // map<int, pair<int, int>> mpp;
    // map< pair<int, int>, int> mpp;

    mpp[1] = 2; // {1, 2}: key: 1 and value: 2
    // map stores unique keys in sorted order

    for (auto it : mpp)
    {
        cout << it.first << " " << it.second << endl;
    }

    // cout << mpp[key] = value;
    auto iterator = mpp.find(3); // it return the iterator to the element at key=3
    // cout << *(iterator).second;

    auto it1 = mpp.find(5); // if there is no element then it will point to after the map ---> mpp.end(); <---

    multimap<int, int> mmpp; // everything is same
    /*additional is we can store duplicate keys as well, and rest the keys will be in sorted order*/
    unordered_map<int, int> umpp;
    // the keys will be unique
    //  but the order will be randomized

    // important algorithms using in built C++ functions
    vector<int> v = {15, 46, 24, 98, 71, 59};
    auto a = v.begin();
    sort(a, a + 6); // a + 6 -> v.end();
    sort(v.begin(), v.end());
    sort(v.begin(), v.end(), greater<int>()); // it' s an in built comparator and sorts the vector into decending order

    pair<int, int> a[] = {{1, 2}, {2, 1}, {4, 1}};
    //sorting in my way
    // sort it according to second element in ascending order
    // if the second element is same, then sort it according to second element in descending order
    // so the answer for my-way sort for the pair
    // {{4, 1}, {2, 1}, {1, 2}}
    bool comp;
    
    sort(a, a + 3, comp); // comp -> a self written comparator, a boolean function

    // int num = 7;
    // int cnt = __builtin_popcount(7); // no of set bits

    // int long num = 16631874634;
    // cnt = __builtin_popcountll(num);

    string str = "123";
    // I want all the permutations of a given string
    sort(s.begin(), s.end());
    do{
        cout << str << endl;
    } while (next_permutation(s.begin(), s.end()));
    
    
    return 0;
}
bool comp(pair<int, int> p1, pair<int, int> p2) {
        if(p1.second < p2.second) return true;
        if(p1.second > p2.second) return false;

        if(p1.first > p2.first) return true;
        return false;
    }