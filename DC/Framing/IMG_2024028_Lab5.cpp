#include<bits/stdc++.h>
using namespace std;

vector<string> byteSplitting(string data){
    vector<string> bytes;
    for(int i = 0; i < data.length(); i +=8){
        bytes.push_back(data.substr(i, 8));
    }
    return bytes;
}

string BitStuffing(string data){
    string StuffedData = "";
    int count = 0;
    for(char bit : data){
        if(bit == '1'){
            count++;
            StuffedData += bit;;
            if(count == 5){
                StuffedData += '0';
                count = 0;
            }
        }
        else {
            StuffedData += bit;
            count = 0;
        }   
    }
    string flag = "01111110";
    return flag + StuffedData + flag;
}

string bitDestuffing(string framed){
    string FLAG = "01111110";
    if(framed.substr(0, 8) == FLAG){
        framed = framed.substr(8);
    }
    
    if(framed.substr(framed.length() - 8) == FLAG){
        framed = framed.substr(0, framed.length() - 8);
    }

    int count = 0;
    string DestuffedData = "";
    for(int i = 0; i < framed.length(); i++){
        char bit = framed[i];
        DestuffedData += bit;

        if(bit == '1')
        count++;
        else count = 0;

        if(count == 5 && i + 1 < framed.length()) {
            i++;
            count = 0;
        }
    }
    return DestuffedData;
}

string byteStuffing(string data){
    string FLAG = "01111110";
    string ESC = "01111101";
    string StuffedData = "";

    vector<string> bytes = byteSplitting(data);

    for(string i : bytes){
        if(i ==FLAG || i == ESC){
            StuffedData += ESC;
        }
        StuffedData += i;
    }
    return FLAG + StuffedData + FLAG;
}

string byteDestuffing(string framed){
    string FLAG = "01111110";
    string ESC = "01111101";

    if(framed.substr(0, 8) == FLAG){
        framed = framed.substr(8);
    }

    if(framed.substr(framed.length() - 8) == FLAG){
        framed = framed.substr(0, framed.length() - 8);
    }

    vector<string> bytes = byteSplitting(framed);
    string DestuffedData = "";
    for (int i = 0; i < bytes.size(); i++)
    {
        if(bytes[i] == ESC){
            i++;
            if(i < bytes.size()){
                DestuffedData += bytes[i];
            }
        } else {
            DestuffedData += bytes[i];
    }
    return DestuffedData;
}
}

void printInChunks(string data) {
    vector<string> bytes = byteSplitting(data);
    for (string b : bytes) cout << b << " ";
    cout << endl;
}

int main() {
    string data;
    cout << "Enter the bitstream (binary): ";
    cin >> data;

    int choice;
    cout << "\n=== CHOOSE FRAMING METHOD ===\n";
    cout << "1. Bit Stuffing\n";
    cout << "2. Byte Stuffing (Character Stuffing)\n";
    cout << "Enter choice: ";
    cin >> choice;

    switch(choice) {
        case 1: {
            cout << "\n=== BIT STUFFING ===\n";
           string stuffed = BitStuffing(data);
            cout << "Transmitted Frame: ";
            printInChunks(stuffed);

            string destuffed = bitDestuffing(stuffed);
            cout << "Received (De-Stuffed) Data: ";
            printInChunks(destuffed);
            break;
        }

        case 2: {
            cout << "\n=== BYTE STUFFING ===\n";
            string stuffed = byteStuffing(data);
            cout << "Transmitted Frame: ";
            printInChunks(stuffed);

            string destuffed = byteDestuffing(stuffed);
            cout << "Received (De-Stuffed) Data: ";
            printInChunks(destuffed);
            break;
        }

        default:
            cout << "Invalid choice!" << endl;
    }

    cout << "\n=== END OF SIMULATION ===\n";
    return 0;
}