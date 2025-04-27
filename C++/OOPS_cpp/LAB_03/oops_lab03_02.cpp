#include<iostream>
using namespace std;

void INPUT_arr(int n, int arr[]){
   for(int i=0; i<n; i++){
        cin>>arr[i];
   }
}
int DeleDupliEle(int n, int arr[]){
    int count = 0;
    for(int i=0; i<n; i++){
        for(int j=i+1; j<n; j++){
            if(arr[i]==arr[j]){
                count++;
               for(int k=j; k<n-1; k++){
                    arr[k] = arr[k+1];
               }
               n--;
               j--;
            }
        }
    }
    return n;
}
// int DeleDupliEle(int n, int arr[]){
//     int count = 0;
//     for(int i=0; i<n; i++){ 
//         for(int j=i+1; j<n; j++){
//             if(arr[i]==arr[j]){
//                 count++;
//                 cout<<count<<endl;
//                 arr[j] = arr[j+1];
//             }
//         }
//     }
//     int NLE = n-count;
//     return NLE;
// }
void NewArr(int NLE, int arr[]){
    for(int i=0; i<NLE; i++){
        cout<<arr[i]<<" ";
    }
}
int main(){
    int n;
    cin>>n;
    int arr[n];
    INPUT_arr(n,arr);
    DeleDupliEle(n, arr);
    int NLE = DeleDupliEle(n, arr);
    NewArr(NLE, arr);
    return 0;
}