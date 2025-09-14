#include <iostream>
#include <string>
#include <map>
#include <algorithm>
using namespace std;

class BlockCoding4B5B
{
private:
    // 4B/5B encoding table following the rules from assignment
    map<string, string> encodingTable;
    map<string, string> decodingTable;

public:
    BlockCoding4B5B()
    {
        // Initialize the 4B/5B mapping table
        // Following rules: max 3 consecutive zeros, at least 2 ones, limited leading/trailing zeros
        encodingTable["0000"] = "11110"; // At least 2 ones, no long zero runs
        encodingTable["0001"] = "01001"; // Valid pattern
        encodingTable["0010"] = "10100"; // Valid pattern
        encodingTable["0011"] = "10101"; // Good transition pattern
        encodingTable["0100"] = "01010"; // Good transition pattern
        encodingTable["0101"] = "01011"; // Good transition pattern
        encodingTable["0110"] = "01110"; // At least 2 ones
        encodingTable["0111"] = "01111"; // Many ones, good for clock recovery
        encodingTable["1000"] = "10010"; // Valid pattern
        encodingTable["1001"] = "10011"; // Valid pattern
        encodingTable["1010"] = "10110"; // Valid pattern
        encodingTable["1011"] = "10111"; // Valid pattern
        encodingTable["1100"] = "11010"; // Valid pattern
        encodingTable["1101"] = "11011"; // Valid pattern
        encodingTable["1110"] = "11100"; // Valid pattern
        encodingTable["1111"] = "11101"; // Valid pattern

        // Create reverse mapping for decoding
        for (auto &pair : encodingTable)
        {
            decodingTable[pair.second] = pair.first;
        }
    }

    string encode(string dataword)
    {
        if (encodingTable.find(dataword) != encodingTable.end())
        {
            return encodingTable[dataword];
        }
        return "ERROR";
    }

    string decode(string codeword)
    {
        if (decodingTable.find(codeword) != decodingTable.end())
        {
            return decodingTable[codeword];
        }
        return "INVALID";
    }

    bool isValidCodeword(string codeword)
    {
        return decodingTable.find(codeword) != decodingTable.end();
    }

    void displayMappingTable()
    {
        cout << "\n=== 4B/5B Encoding Table ===" << endl;
        cout << "Dataword\tCodeword" << endl;
        cout << "--------\t--------" << endl;
        for (auto &pair : encodingTable)
        {
            cout << pair.first << "\t\t" << pair.second << endl;
        }
        cout << endl;
    }

    string getRedundantBits(string dataword, string codeword)
    {
        // For 4B/5B, we're adding 1 redundant bit
        // The redundant information is embedded in the choice of codeword
        return "1 bit (embedded in codeword selection)";
    }

    string simulateTransmission(string codeword, bool introduceError = false)
    {
        if (!introduceError)
        {
            return codeword;
        }

        // Simulate a single bit error for demonstration
        string transmitted = codeword;
        if (transmitted.length() > 0)
        {
            // Flip the first bit as an example error
            transmitted[0] = (transmitted[0] == '0') ? '1' : '0';
        }
        return transmitted;
    }
};

int main()
{
    BlockCoding4B5B blockCoder;

    cout << "=== 4B/5B Block Coding Implementation ===" << endl;
    cout << "Week 05 - Assignment 05: Error Detection and Correction" << endl;

    // Display the encoding table
    blockCoder.displayMappingTable();

    // Get user input
    string dataword;
    char errorChoice;

    cout << "Enter a 4-bit dataword (e.g., 0101): ";
    cin >> dataword;

    // Validate input
    if (dataword.length() != 4)
    {
        cout << "Error: Please enter exactly 4 bits!" << endl;
        return 1;
    }

    for (char c : dataword)
    {
        if (c != '0' && c != '1')
        {
            cout << "Error: Please enter only 0s and 1s!" << endl;
            return 1;
        }
    }

    cout << "\nDo you want to simulate a transmission error? (y/n): ";
    cin >> errorChoice;

    cout << "\n=== SENDER SIDE ===" << endl;
    cout << "DataWord to be encoded: " << dataword << endl;

    // Encoding process
    string codeword = blockCoder.encode(dataword);
    if (codeword == "ERROR")
    {
        cout << "Error: Invalid dataword!" << endl;
        return 1;
    }

    cout << "Redundant bits generated: " << blockCoder.getRedundantBits(dataword, codeword) << endl;
    cout << "CodeWord generated at sender: " << codeword << endl;

    cout << "\n=== TRANSMISSION ===" << endl;
    bool introduceError = (errorChoice == 'y' || errorChoice == 'Y');
    string receivedCodeword = blockCoder.simulateTransmission(codeword, introduceError);

    if (introduceError)
    {
        cout << "Original CodeWord: " << codeword << endl;
        cout << "Received CodeWord (with error): " << receivedCodeword << endl;
    }
    else
    {
        cout << "Received CodeWord (no errors): " << receivedCodeword << endl;
    }

    cout << "\n=== RECEIVER SIDE ===" << endl;
    cout << "Received CodeWord: " << receivedCodeword << endl;

    // Validation and decoding
    if (blockCoder.isValidCodeword(receivedCodeword))
    {
        cout << "CodeWord Status: VALID" << endl;
        string decodedDataword = blockCoder.decode(receivedCodeword);
        cout << "Decoded DataWord: " << decodedDataword << endl;

        // Verify if decoded matches original
        if (decodedDataword == dataword)
        {
            cout << "Verification: SUCCESS - Decoded dataword matches original!" << endl;
        }
        else
        {
            cout << "Verification: WARNING - Decoded dataword differs from original!" << endl;
        }
    }
    else
    {
        cout << "CodeWord Status: INVALID" << endl;
        cout << "Error detected during transmission!" << endl;
        cout << "Cannot decode - requesting retransmission..." << endl;
    }

    cout << "\n=== SUMMARY ===" << endl;
    cout << "Input Parameters:" << endl;
    cout << "  N (bits in dataword): 4" << endl;
    cout << "  M (bits in codeword): 5" << endl;
    cout << "  Original DataWord: " << dataword << endl;
    cout << "  Generated CodeWord: " << codeword << endl;
    cout << "  Received CodeWord: " << receivedCodeword << endl;

    return 0;
}