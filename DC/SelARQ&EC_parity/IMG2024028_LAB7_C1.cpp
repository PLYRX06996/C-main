#include <bits/stdc++.h>
using namespace std;

class SelectiveRepeatARQ {
private:
    int WS, total_frames, max_seq, timeout, time_unit, send_base, rcv_base;
    double loss_prob;
    vector<bool> acked, sent, received;
    vector<int> timer;

public:
    SelectiveRepeatARQ(int ws, int n, double p, int tout) {
        WS = ws;
        total_frames = n;
        loss_prob = p;
        timeout = tout;
        max_seq = 2 * WS - 1;
        send_base = 0;
        rcv_base = 0;
        time_unit = 0;
        acked.assign(total_frames, false);
        sent.assign(total_frames, false);
        received.assign(total_frames, false);
        timer.assign(total_frames, 0);
        srand(time(0));
    }

    bool isLost() {
        return ((double)rand() / RAND_MAX) < loss_prob;
    }

    void senderTransmit() {
        for (int i = send_base; i < min(send_base + WS, total_frames); i++) {
            if (!acked[i] && !sent[i]) {
                cout << "Time " << time_unit << ": Frame " << i << " sent\n";
                sent[i] = true;
                timer[i] = timeout;
            }
        }
    }

    void checkTimeouts() {
        for (int i = send_base; i < min(send_base + WS, total_frames); i++) {
            if (sent[i] && !acked[i]) {
                timer[i]--;
                if (timer[i] <= 0) {
                    cout << "Time " << time_unit << ": Timeout for Frame " << i << ", retransmitting\n";
                    sent[i] = false;
                }
            }
        }
    }

    void processACK(int seq) {
        if (seq < 0 || seq >= total_frames || acked[seq]) return;
        acked[seq] = true;
        sent[seq] = false;
        timer[seq] = 0;
        cout << "Time " << time_unit << ": ACK " << seq << " received\n";
        while (send_base < total_frames && acked[send_base]) {
            send_base++;
        }
    }

    void receiverReceive(int seq) {
        if (seq < rcv_base || seq >= rcv_base + WS) {
            cout << "Time " << time_unit << ": Frame " << seq << " outside window, discarded\n";
            return;
        }
        if (!received[seq]) {
            if (seq == rcv_base)
                cout << "Time " << time_unit << ": Frame " << seq << " received in order\n";
            else
                cout << "Time " << time_unit << ": Frame " << seq << " received out of order, buffered\n";
            received[seq] = true;
        }
        while (rcv_base < total_frames && received[rcv_base]) {
            cout << "Time " << time_unit << ": Frame " << rcv_base << " delivered to app layer\n";
            received[rcv_base] = false;
            rcv_base++;
        }
        if (isLost())
            cout << "Time " << time_unit << ": ACK " << seq << " lost\n";
        else
            processACK(seq);
    }

    void simulateTime() {
        cout << "\n=== TIME " << time_unit << " ===\n";
        checkTimeouts();
        senderTransmit();
        for (int i = send_base; i < min(send_base + WS, total_frames); i++) {
            if (sent[i] && !acked[i]) {
                if (isLost())
                    cout << "Time " << time_unit << ": Frame " << i << " lost\n";
                else
                    receiverReceive(i);
            }
        }
        time_unit++;
    }

    void run() {
        cout << "\n=== Selective Repeat ARQ Simulation ===\n";
        cout << "Window Size: " << WS << ", Frames: " << total_frames
             << ", Loss Probability: " << loss_prob << ", Timeout: " << timeout << "\n";

        while (send_base < total_frames && time_unit < 200) {
            simulateTime();
        }

        if (send_base >= total_frames)
            cout << "\nAll frames transmitted successfully!\n";
        else
            cout << "\nSimulation ended (max time reached).\n";
    }
};

class ParityChecker {
private:
    string data;
    double error_prob;

public:
    ParityChecker(string d, double p) {
        data = d;
        error_prob = p;
        srand(time(0));
    }

    char evenParity() {
        int ones = count(data.begin(), data.end(), '1');
        return (ones % 2 == 0) ? '0' : '1';
    }

    char oddParity() {
        int ones = count(data.begin(), data.end(), '1');
        return (ones % 2 == 0) ? '1' : '0';
    }

    string channelError(string s) {
        if (((double)rand() / RAND_MAX) < error_prob) {
            int pos = rand() % s.length();
            s[pos] = (s[pos] == '0') ? '1' : '0';
            cout << "Bit flipped at position " << pos << endl;
        } else {
            cout << "No bit error occurred\n";
        }
        return s;
    }

    bool evenCheck(string s) {
        int ones = count(s.begin(), s.end(), '1');
        return (ones % 2 == 0);
    }

    bool oddCheck(string s) {
        int ones = count(s.begin(), s.end(), '1');
        return (ones % 2 == 1);
    }

    void run() {
        cout << "\n=== Parity Bit Error Detection ===\n";
        cout << "Data Word: " << data << endl;

        char e = evenParity();
        string tx = data + e;
        cout << "\nTransmitted (Even Parity): " << tx << endl;
        string rx = channelError(tx);
        cout << "Received Word: " << rx << endl;
        if (evenCheck(rx)) cout << "No error detected (Even Parity)\n";
        else cout << "Error detected (Even Parity)\n";

        char o = oddParity();
        string tx2 = data + o;
        cout << "\nTransmitted (Odd Parity): " << tx2 << endl;
        string rx2 = channelError(tx2);
        cout << "Received Word: " << rx2 << endl;
        if (oddCheck(rx2)) cout << "No error detected (Odd Parity)\n";
        else cout << "Error detected (Odd Parity)\n";
    }
};

int main() {
    while (true) {
        cout << "\n=== MENU ===\n";
        cout << "1. Selective Repeat ARQ\n";
        cout << "2. Parity Bit Error Detection\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        int ch;
        cin >> ch;

        if (ch == 1) {
            int ws, n, t;
            double p;
            cout << "Enter Window Size: ";
            cin >> ws;
            cout << "Enter Total Frames: ";
            cin >> n;
            cout << "Enter Loss Probability (0-1): ";
            cin >> p;
            cout << "Enter Timeout (time units): ";
            cin >> t;
            SelectiveRepeatARQ sr(ws, n, p, t);
            sr.run();
        }
        else if (ch == 2) {
            string word;
            double p;
            cout << "Enter Data Word (binary): ";
            cin >> word;
            cout << "Enter Bit Error Probability (0-1): ";
            cin >> p;
            ParityChecker pc(word, p);
            pc.run();
        }
        else if (ch == 3) {
            cout << "Exiting...\n";
            break;
        }
        else {
            cout << "Invalid choice.\n";
        }
    }
    return 0;
}