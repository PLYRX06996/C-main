#include <bits/stdc++.h>
using namespace std;

class BlockCoding4B5B
{
    map<string, string> encodingTable;
    map<string, string> decodingTable;

public:
    BlockCoding4B5B()
    {
        encodingTable = {
            {"0000", "11110"}, {"0001", "01001"}, {"0010", "10100"}, {"0011", "10101"}, {"0100", "01010"}, {"0101", "01011"}, {"0110", "01110"}, {"0111", "01111"}, {"1000", "10010"}, {"1001", "10011"}, {"1010", "10110"}, {"1011", "10111"}, {"1100", "11010"}, {"1101", "11011"}, {"1110", "11100"}, {"1111", "11101"}};

        for (auto &p : encodingTable)
        {
            decodingTable[p.second] = p.first;
        }
    }

    string encode(string dataword)
    {
        if (encodingTable.find(dataword) != encodingTable.end()){
            return encodingTable[dataword];
        }
        return "ERROR";
    }

    bool isValidCodeWord(string codeword){
        return decodingTable.find(codeword) != decodingTable.end();
    }

    string decode(string codeword)
    {
        if (isValidCodeWord(codeword)){
            return decodingTable[codeword];
        }
        return "INVALID";
    }

    void diplayMappingtable(){
        cout << "Dataword\t|\tCodeword" << endl;
        cout << "-------------------------" << endl;
        for(auto& i : encodingTable){
            cout << i.first << "\t|\t" << i.second << endl;
        }
        cout <<  endl;
    }

    string simulatetransmissionError(string codeword, bool introduceError = false){
        if(introduceError == false){
            return codeword;
        }

        string *transmitted = &codeword;
    }
};

void validateInput(string input){
    if(input.length() != 4){
        cout << "Error: Please enter exactly bits!" << endl;
        return;
    }
    for(auto c: input){
        if('c' != 1 && 'c' != 0){
            cout << "Invalid Inputs, please enter only '0' and '1'."<< endl;
            return;
        }
    }
}

int main()
{
    string dataword;
    cout << "Enter 4-bit data: ";
    cin >> dataword;
    return 0;
}