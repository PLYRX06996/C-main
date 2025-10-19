#include<bits/stdc++.h>
#include<ctime>
#include<bitset>
using namespace std;

class SelectiveRepeatARQ {
    private:
    int WS, max_seq, total_frames;
    double loss_probability;

    int send_base, next_seq_num;
    vector<bool> acked;
    vector<bool> sent;
    vector<int> timers;

    int receive_base;
    vector<bool> received;
    map<int, bool> buffer;

    int time_unit;

    public:
    SelectiveRepeatARQ(int window_size, int n_frames, double loss_prob) {
        WS = window_size;
        max_seq = 2 * WS;
        total_frames = n_frames;
        loss_probability = loss_prob;

        // Initialize Sender variables
        send_base = 0;
        next_seq_num = 0;
        acked.resize(total_frames, false);
        sent.resize(total_frames, false);
        timers.resize(total_frames, -1);

        // Initialize Receiver variables
        receive_base = 0;
        received.resize(total_frames, false);

        time_unit = 0;

        srand(time(0));
    }
    // loss probability for unreliable niosey channel
    bool channel_loss() {
        return (rand() % 100) < (loss_probability * 100);
    }

    void sender_transmit() {
        while(next_seq_num < send_base + WS && next_seq_num < total_frames) {
            if(!sent[next_seq_num] || timers[next_seq_num] <= 0) {
                if(channel_loss()) {
                    cout << "time " << time_unit << ": Frame " << next_seq_num 
                    << " lost, retransmission required" << endl;
                } else {
                    cout << "time " << time_unit << ": Frame " << next_seq_num 
                    << " transmitted." << endl;
                    sent[next_seq_num] = true;
                    timers[next_seq_num] = 3;
                }
                next_seq_num++;
            } else {
                next_seq_num++;
            }
        }
    }

    void process_ack(int seq_num) {
        if(seq_num >= send_base && seq_num < send_base + WS) {
            acked[seq_num] = true;
            timers[seq_num] = 0;
            cout << "time " << time_unit << ": ACK " << seq_num << " received." << endl;

            while(send_base < total_frames && acked[send_base]) {
                send_base++;
            }
        }
    }

    void check_timeouts() {
        for(int  i = send_base; i < min(send_base + WS, total_frames); i++) {
            if(sent[i] && !acked[i]) {
                timers[i]--;
                if(timers[i] <= 0) {
                    cout << "Time " << time_unit << ":Frame " << i
                    << " timeout, retransmitting." << endl;
                    sent[i] = false;
                }
            }
        }
    }

    void receiver_process(int seq_num) {
        if()
    }
};

int main() {
    
    return 0;
}