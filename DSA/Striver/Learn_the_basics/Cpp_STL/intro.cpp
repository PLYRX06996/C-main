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
Container Adapters [stack, queue, prority queue]*/

void explainpair() {
    pair<int, int> p = {1, 3};
    cout << p.first << " " << p.second;

    pair<int, pair<int, int>> q = {1, {2, 3}};
    cout << q.first << " " << q.second.first << " " << q.second.second << endl;

    pair<int, int> arr[] = {{1, 6}, {2, 3}, {4, 5 }};
}

int main() {
    vector<int> v;
    v.push_back(1);
    v.emplace_back(2);

    vector<pair<int, int>>V;
    V.push_back({1,2});
    V.emplace_back(1, 2);

    vector<int> v(5, 100);
    vector<int> v(5);

    vector<int> v1(5, 30);
    vector<int> v2(v1);

    vector<int> v = {10, 20, 30, 40, 50};
    vector<int>::iterator  it = v.begin(); // this points to the first memory location of the vecctor dtt
    cout << *(it) << endl;

    it = it + 2;
    cout << *(it) << endl;

    vector<int>::iterator it = v.end(); // it doesn't point to the last element of the vector, instead it points to the next memory address after the last element.
    vector<int>::iterator it = v.rend(); //this is a reverse iterator if we do it++ it will shift backwards
    //or this as if we apply r iterator then we should forst reverse the vector elements and then do pinter iterations, so this points to memory address location before 10
    vector<int>::iterator it = v.rbegin();// this point to memory address location of 50

    cout << v[0] << " " << v.at(0);
    cout << v.back() << endl;

    for(vector<int>::iterator it = v.begin(); it != v.end(); it++){
        cout << *(it) << " ";
    }

    return 0;
} 