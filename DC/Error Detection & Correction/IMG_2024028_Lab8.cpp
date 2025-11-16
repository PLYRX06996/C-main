#include <bits/stdc++.h>
using namespace std;

string addBinary(string a, string b)
{
    string res = "";
    int carry = 0;
    int n = max(a.size(), b.size());
    while (a.size() < n)
        a = '0' + a;
    while (b.size() < n)
        b = '0' + b;

    for (int i = n - 1; i >= 0; i--)
    {
        int sum = (a[i] - '0') + (b[i] - '0') + carry;
        res = char((sum % 2) + '0') + res;
        carry = sum / 2;
    }
    if (carry)
        res = '1' + res;
    if (res.size() > n)
        res = addBinary(res.substr(1), "1");
    return res.substr(res.size() - n);
}

string onesComplement(string s)
{
    for (auto &c : s)
        c = (c == '0' ? '1' : '0');
    return s;
}

string calcChecksum(string data, int wordSize)
{
    while (data.size() % wordSize != 0)
        data = '0' + data;
    string sum = data.substr(0, wordSize);
    for (int i = wordSize; i < data.size(); i += wordSize)
    {
        string word = data.substr(i, wordSize);
        sum = addBinary(sum, word);
    }
    return onesComplement(sum);
}

bool verifyChecksum(string data, int wordSize)
{
    string sum = data.substr(0, wordSize);
    for (int i = wordSize; i < data.size(); i += wordSize)
    {
        string word = data.substr(i, wordSize);
        sum = addBinary(sum, word);
    }
    sum = onesComplement(sum);
    return sum.find('1') == string::npos;
}

string divideCRC(string data, string poly)
{
    int n = poly.size();
    string temp = data.substr(0, n);
    for (int i = n; i <= data.size(); i++)
    {
        if (temp[0] == '1')
        {
            for (int j = 0; j < n; j++)
                temp[j] = (temp[j] == poly[j]) ? '0' : '1';
        }
        if (i < data.size())
            temp = temp.substr(1) + data[i];
    }
    return temp.substr(1);
}

string calcFCS(string data, string poly)
{
    int n = poly.size() - 1;
    string appended = data + string(n, '0');
    return divideCRC(appended, poly);
}

bool checkCRC(string frame, string poly)
{
    string rem = divideCRC(frame, poly);
    for (char c : rem)
        if (c == '1')
            return false;
    return true;
}

int calcParityBits(int dataBits)
{
    int r = 0;
    while (pow(2, r) < (dataBits + r + 1))
        r++;
    return r;
}

string generateHamming(string data)
{
    int m = data.size();
    int r = calcParityBits(m);
    int total = m + r;
    string code(total + 1, '0');
    int j = 0;
    for (int i = 1; i <= total; i++)
        if ((i & (i - 1)) != 0)
            code[i] = data[j++];
    for (int i = 0; i < r; i++)
    {
        int pos = pow(2, i), cnt = 0;
        for (int bit = pos; bit <= total; bit++)
            if (bit & pos)
                if (code[bit] == '1')
                    cnt++;
        code[pos] = (cnt % 2 == 0) ? '0' : '1';
    }
    string res = "";
    for (int i = 1; i <= total; i++)
        res += code[i];
    return res;
}

string detectAndFix(string recv)
{
    int n = recv.size(), r = 0;
    while (pow(2, r) <= n)
        r++;
    int err = 0;
    for (int i = 0; i < r; i++)
    {
        int pos = pow(2, i), cnt = 0;
        for (int bit = pos; bit <= n; bit++)
            if (bit & pos)
                if (recv[bit - 1] == '1')
                    cnt++;
        if (cnt % 2 != 0)
            err += pos;
    }
    if (err == 0)
    {
        cout << "No error detected.\n";
    }
    else
    {
        cout << "Error detected at bit position " << err << ". Correcting...\n";
        recv[err - 1] = (recv[err - 1] == '0') ? '1' : '0';
    }
    return recv;
}

int main()
{
    srand(time(0));
    while (true)
    {
        cout << "1. Checksum Method\n";
        cout << "2. CRC Method\n";
        cout << "3. Hamming Code\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        int ch;
        cin >> ch;
        if (ch == 4)
            break;

        if (ch == 1)
        {
            string data;
            int wordSize;
            cout << "\nEnter binary data: ";
            cin >> data;
            cout << "Enter word size: ";
            cin >> wordSize;
            string checksum = calcChecksum(data, wordSize);
            cout << "Calculated Checksum: " << checksum << endl;
            string transmitted = data + checksum;
            cout << "Transmitted Frame: " << transmitted << endl;
            cout << "Enter received frame: ";
            string recv;
            cin >> recv;
            if (verifyChecksum(recv, wordSize))
                cout << "No error detected.\n";
            else
                cout << "Error detected in transmission.\n";
        }

        else if (ch == 2)
        {
            string data, poly;
            cout << "\nEnter Data bits: ";
            cin >> data;
            cout << "Enter Generator Polynomial bits: ";
            cin >> poly;
            string fcs = calcFCS(data, poly);
            string frame = data + fcs;
            cout << "Calculated FCS: " << fcs << endl;
            cout << "Transmitted Frame: " << frame << endl;
            cout << "Enter received frame: ";
            string recv;
            cin >> recv;
            if (checkCRC(recv, poly))
                cout << "No error detected.\n";
            else
                cout << "Error detected in received frame.\n";
        }

        else if (ch == 3)
        {
            string data;
            cout << "\nEnter data bits: ";
            cin >> data;
            string hamming = generateHamming(data);
            cout << "Generated Hamming Code: " << hamming << endl;
            cout << "Enter received bits (may include an error): ";
            string recv;
            cin >> recv;
            string fixed = detectAndFix(recv);
            cout << "Corrected Code: " << fixed << endl;
        }

        else
        {
            cout << "Invalid choice. Try again.\n";
        }
    }
    return 0;
}