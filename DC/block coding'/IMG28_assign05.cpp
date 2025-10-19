#include <bits/stdc++.h>
using namespace std;

class BlockCoding
{
private:
    map<string, string> encode;
    map<string, string> decode;

    bool isValidCodeword(const string &codeword)
    {
        if (codeword.find("0000") != string::npos)
            return false;

        int ones = count(codeword.begin(), codeword.end(), '1');
        if (ones < 2)
            return false;

        if (codeword.substr(0, 3) == "000" || codeword.substr(2, 3) == "000")
            return false;

        return true;
    }

public:
    void generateMappingTable()
    {
        vector<string> datawords;
        vector<string> codewords;

        for (int i = 0; i < 16; i++)
        {
            string dw = "";
            for (int j = 3; j >= 0; j--)
            {
                dw += ((i >> j) & 1) ? '1' : '0';
            }
            datawords.push_back(dw);
        }

        for (int i = 0; i < 32; i++)
        {
            string cw = "";
            for (int j = 4; j >= 0; j--)
            {
                cw += ((i >> j) & 1) ? '1' : '0';
            }
            if (isValidCodeword(cw))
            {
                codewords.push_back(cw);
            }
        }

        for (int i = 0; i < 16 && i < codewords.size(); i++)
        {
            encode[datawords[i]] = codewords[i];
            decode[codewords[i]] = datawords[i];
        }
    }

    void displayTable()
    {
        cout << "\n=== Generated 4B/5B Mapping Table ===" << endl;
        cout << "Dataword\tCodeword" << endl;
        cout << "--------\t--------" << endl;
        for (const auto &p : encode)
        {
            cout << p.first << "\t\t" << p.second << endl;
        }
        cout << endl;
    }

    string encodeDataword(const string &dataword)
    {
        if (encode.count(dataword))
        {
            return encode[dataword];
        }
        return "ERROR";
    }

    bool isValidReceivedCodeword(const string &codeword)
    {
        return decode.count(codeword) > 0;
    }

    string decodeCodeword(const string &codeword)
    {
        if (decode.count(codeword))
        {
            return decode[codeword];
        }
        return "INVALID";
    }

    void checkerAlgorithm(const string &originalDataword, const string &receivedCodeword)
    {
        cout << "\n=== ACTIVITY 3: Checker Algorithm ===" << endl;
        cout << "Input DataWord: " << originalDataword << endl;
        cout << "Received CodeWord: " << receivedCodeword << endl;

        if (isValidReceivedCodeword(receivedCodeword))
        {
            cout << "CodeWord Status: VALID" << endl;
            string extractedDW = decodeCodeword(receivedCodeword);
            cout << "Extracted DataWord: " << extractedDW << endl;

            if (extractedDW == originalDataword)
            {
                cout << "Verification: SUCCESS - Matches original!" << endl;
            }
            else
            {
                cout << "Verification: MISMATCH - Does not match original!" << endl;
            }
        }
        else
        {
            cout << "CodeWord Status: INVALID" << endl;
            cout << "Error detected - CodeWord not found in mapping table!" << endl;
        }
    }
};

int main()
{
    cout << "=== 4B/5B Block Coding - All Activities ===" << endl;

    BlockCoding bc;

    cout << "\n=== ACTIVITY 2: Generate Mapping Table ===" << endl;
    bc.generateMappingTable();
    bc.displayTable();

    cout << "=== ACTIVITY 1: Basic 4B/5B Implementation ===" << endl;
    string dataword;
    cout << "Enter 4-bit dataword: ";
    cin >> dataword;

    if (dataword.length() != 4)
    {
        cout << "Error: Enter exactly 4 bits!" << endl;
        return 1;
    }

    string codeword = bc.encodeDataword(dataword);
    if (codeword == "ERROR")
    {
        cout << "Error: Invalid dataword!" << endl;
        return 1;
    }

    cout << "DataWord to be encoded: " << dataword << endl;
    cout << "Redundant bits: 1 bit (embedded in codeword selection)" << endl;
    cout << "Generated CodeWord: " << codeword << endl;

    string received = codeword;
    char choice;
    cout << "Introduce transmission error? (y/n): ";
    cin >> choice;
    if (choice == 'y' || choice == 'Y')
    {
        received[0] = (received[0] == '0') ? '1' : '0';
        cout << "Simulated error: " << codeword << " → " << received << endl;
    }

    cout << "Received CodeWord: " << received << endl;

    if (bc.isValidReceivedCodeword(received))
    {
        string decoded = bc.decodeCodeword(received);
        cout << "Status: VALID" << endl;
        cout << "Decoded DataWord: " << decoded << endl;
        cout << "Match with original: " << (decoded == dataword ? "YES" : "NO") << endl;
    }
    else
    {
        cout << "Status: INVALID - Error detected!" << endl;
    }

    bc.checkerAlgorithm(dataword, received);

    return 0;
}