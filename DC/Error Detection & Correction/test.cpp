#include<bits/stdc++.h>
using namespace std;

bool isEvenParity(const string& data) {
    int count = 0;
    for (char bit : data) {
        if (bit == '1') {
            count++;
        }
    }
    return (count % 2 == 0);
}

bool isOddParity(const string& data) {
    int count = 0;
    for (char bit : data) {
        if (bit == '1') {
            count++;
        }
    }
    return (count % 2 != 0);
}

string flipBit(string data, double probability) {
    if((double)rand() /RAND_MAX < probability) {
        int pos = rand() % data.size();
        data[pos] = (data[pos] == '0') ? '1' : '0';
        cout << "Bit flipped at position: " << pos << endl;
    } else {
        cout << "No Bit Error Occurred" << endl;
    }
    return data;
}

// Checksum Functions

string addBinary(string a , string b ){
    string result = "";
    int carry = 0;
    int i = a.size() - 1, j = b.size() - 1;
    while (i >= 0 || j >= 0 || carry) {
        int sum = carry;//consider carry from previous addition
        if(i >= 0) sum+= a[i--] - '0'; // - '0' to convert char to int, using ASCII values
        if(j >= 0) sum += b[j--] - '0';
        result = char((sum % 2) + '0') + result; // + '0' to convert int to char, using ASCII values
        carry = sum / 2;
    }
    
    //if overflow occurs, wrap around
    if(result.size() > a.size()) {
        string overflow = result.substr(0, result.size() - a.size()); //overflow bits are essentially carry
        result = result.substr(result.size() - a.size());
        result = addBinary(result, overflow);
    }
    return result;
}

string onesComplement(string data) {
    for (char &b : data) {// & to modify original string
        b = (b == '0') ? '1' : '0';
        return data;
    }
}

string calcChecksum(string data, int wordSize) {
    //pad data (0s) to make its size multiple of wordSize
    while(data.size() % wordSize != 0) data = '0' + data;

    string sum = data.substr(0, wordSize); //initialize sum with first word
    for(int i = wordSize; i < data.size(); i += wordSize) {
        string nextWord = data.substr(i, wordSize);
        sum = addBinary(sum, nextWord);//add next word to sum
    }
    return onesComplement(sum);//return one's complement of sum as checksum
}

bool verifyChecksum(string recievedData, int wordSize) {
    while(recievedData.size() % wordSize != 0) recievedData = '0' + recievedData;

    string sum = recievedData.substr(0, wordSize); //initialize sum with first word
    for(int i = wordSize; i < recievedData.size(); i += wordSize) {
        string nextWord = recievedData.substr(i, wordSize);
        sum = addBinary(sum, nextWord);//add next word to sum
    }
    string checksum = onesComplement(sum);
    //if checksum is all 0s, no error
    for(char bit : checksum) {
        if(bit == '1') return false; //error detected
    }
    return true; //no error
}

int main() {
    
    return 0;
}