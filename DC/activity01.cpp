#include <iostream>
#include <map>
#include <string>
using namespace std;

int main() {
    // 4B/5B mapping table
    map<string, string> encode = {
        {"0000", "11110"}, {"0001", "01001"}, {"0010", "10100"}, {"0011", "10101"},
        {"0100", "01010"}, {"0101", "01011"}, {"0110", "01110"}, {"0111", "01111"},
        {"1000", "10010"}, {"1001", "10011"}, {"1010", "10110"}, {"1011", "10111"},
        {"1100", "11010"}, {"1101", "11011"}, {"1110", "11100"}, {"1111", "11101"}
    };
    
    // Reverse mapping for decoding
    map<string, string> decode;
    for (auto& p : encode) {
        decode[p.second] = p.first;
    }
    
    string dataword;
    cout << "Enter 4-bit dataword: ";
    cin >> dataword;
    
    // Encoding
    string codeword = encode[dataword];
    cout << "DataWord: " << dataword << endl;
    cout << "Redundant bits: 1 bit" << endl;
    cout << "CodeWord: " << codeword << endl;
    
    // Simulate transmission (with optional error)
    string received = codeword;
    char choice;
    cout << "Introduce error? (y/n): ";
    cin >> choice;
    if (choice == 'y') {
        received[0] = (received[0] == '0') ? '1' : '0';  // flip first bit
    }
    
    cout << "Received CodeWord: " << received << endl;
    
    // Validation and decoding
    if (decode.count(received)) {
        string original = decode[received];
        cout << "Status: VALID" << endl;
        cout << "Decoded DataWord: " << original << endl;
        cout << "Match: " << (original == dataword ? "YES" : "NO") << endl;
    } else {
        cout << "Status: INVALID - Error detected!" << endl;
    }
    
    return 0;
}