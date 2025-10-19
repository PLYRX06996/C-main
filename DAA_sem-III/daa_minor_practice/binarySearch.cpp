    #include<bits/stdc++.h>
    using namespace std;

    void inputArray(vector<int>& arr){
        int size;
        cout << "Enter the number of elements: ";
        cin >> size;
        arr.resize(size);
        cout << "Enter the elements for the array: ";
        for(int i = 0; i < arr.size(); i++){
            cin >> arr[i];
        }
    }
    void printArray(const vector<int>& arr){
        cout << "Elements of the array in sorted order: ";
        for(auto i : arr){
            cout << i << " ";
        }
        cout <<  endl;
    }

    int BinarySearch(vector<int>& arr, int l, int h, int tgt){
        if(l > h){
            return -1;
        }
            int mid = (l + h)/2;
            while (l <= h)   
            {
                if(tgt == arr[mid]){
                    return mid;
                } else if(arr[mid] <= tgt){
                    l = mid + 1;
                    return BinarySearch(arr, l, h, tgt);
                } else{
                    h = mid - 1;
                    return BinarySearch(arr, l, h, tgt);
                }
            }
    }

    int main() {
        vector<int> arr;
        inputArray(arr);
        printArray(arr);
        cout << "Enter the element to find it's position: ";
        int target;
        cin >> target;
        cout << arr.size() << endl;
        cout << "The position(index) of the target element is: " << BinarySearch(arr, 0, arr.size() - 1, target) <<  endl;
        return 0;
    }