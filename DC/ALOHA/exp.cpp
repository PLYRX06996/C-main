// Pure ALOHA simulation
// Simulates N stations that generate frames at random intervals.
// Each frame transmission lasts `frame_duration` time units.
// When two or more transmissions overlap, they collide and are lost.
// Stations retransmit after a random backoff.
// Outputs total successful transmissions, collisions, and efficiency.

#include <bits/stdc++.h>
using namespace std;

enum EventType { FRAME_ARRIVAL, TRANSMIT_START, TRANSMIT_END };

struct Event {
    double time;
    EventType type;
    int station_id;
    int tx_id; // optional: associated transmission id for end events
};

struct EventCompare {
    bool operator()(Event const &a, Event const &b) const {
        if (a.time != b.time) return a.time > b.time;
        return a.type > b.type;
    }
};

struct Transmission {
    int id;
    int station_id;
    double start;
    double end;
    bool collided;
};

struct Station {
    int id;
    bool has_frame = false; // whether station currently has a frame waiting (or being transmitted)
    // For simplicity we hold at most one frame per station (new frames while busy are dropped)
};

int main() {
    // Simulation parameters (can be adjusted or read from stdin)
    int N = 10; // number of stations
    double SIM_TIME = 100.0; // total simulation time units
    double frame_duration = 1.0; // duration of a frame transmission
    int max_backoff = 5; // max backoff (integer time units)
    double min_interarrival = 1.0, max_interarrival = 10.0; // when generating new frames

    cout << "Pure ALOHA simulation\n";
    cout << "Stations (N) [default "<<N<<"] | Simulation time ["<<SIM_TIME<<"] | frame duration ["<<frame_duration<<"]\n";
    cout << "(You can change parameters in the source.)\n\n";

    // Random engine
    std::mt19937 rng((unsigned)chrono::high_resolution_clock::now().time_since_epoch().count());
    uniform_real_distribution<double> initial_dist(0.0, SIM_TIME/2.0);
    uniform_real_distribution<double> interarrival_dist(min_interarrival, max_interarrival);
    uniform_int_distribution<int> backoff_dist(1, max_backoff);

    // Setup
    vector<Station> stations(N);
    for (int i = 0; i < N; ++i) stations[i].id = i;

    priority_queue<Event, vector<Event>, EventCompare> pq;

    // schedule initial frame arrivals at random times
    for (int i = 0; i < N; ++i) {
        double t = initial_dist(rng);
        if (t <= SIM_TIME) pq.push(Event{t, FRAME_ARRIVAL, i, -1});
    }

    // bookkeeping
    int next_tx_id = 1;
    unordered_map<int, Transmission> active_tx_map; // by tx_id (active or finishing soon)
    vector<int> active_tx_ids; // list of active transmission ids (for quick iteration)

    long long total_transmissions = 0; // count of transmit attempts (including retransmissions)
    long long successful = 0;
    long long collisions = 0; // number of lost frames (each colliding frame counted once)

    // Event loop
    while (!pq.empty()) {
        Event ev = pq.top(); pq.pop();
        if (ev.time > SIM_TIME) break; // stop processing events past simulation time

        switch (ev.type) {
            case FRAME_ARRIVAL: {
                Station &s = stations[ev.station_id];
                // schedule next arrival for this station
                double next_arr = ev.time + interarrival_dist(rng);
                if (next_arr <= SIM_TIME) pq.push(Event{next_arr, FRAME_ARRIVAL, ev.station_id, -1});

                if (!s.has_frame) {
                    // station immediately transmits (Pure ALOHA)
                    s.has_frame = true;
                    pq.push(Event{ev.time, TRANSMIT_START, ev.station_id, -1});
                } else {
                    // drop new frame while one is pending (simplifying assumption)
                }
                break;
            }
            case TRANSMIT_START: {
                // start a transmission for this station
                Transmission tx;
                tx.id = next_tx_id++;
                tx.station_id = ev.station_id;
                tx.start = ev.time;
                tx.end = ev.time + frame_duration;
                tx.collided = false;
                active_tx_map[tx.id] = tx;
                active_tx_ids.push_back(tx.id);

                // schedule end
                pq.push(Event{tx.end, TRANSMIT_END, ev.station_id, tx.id});

                // count attempt
                total_transmissions++;

                // if more than one active transmission, mark all as collided
                if (active_tx_ids.size() > 1) {
                    for (int id : active_tx_ids) {
                        active_tx_map[id].collided = true;
                    }
                }
                break;
            }
            case TRANSMIT_END: {
                // find transmission
                int txid = ev.tx_id;
                auto it = active_tx_map.find(txid);
                if (it == active_tx_map.end()) break; // already handled
                Transmission tx = it->second;

                // remove from active list
                active_tx_map.erase(it);
                active_tx_ids.erase(remove(active_tx_ids.begin(), active_tx_ids.end(), txid), active_tx_ids.end());

                Station &s = stations[tx.station_id];

                if (!tx.collided) {
                    successful++;
                    // frame delivered; station no longer has a pending frame
                    s.has_frame = false;
                } else {
                    // collision: frame lost, schedule retransmission after random backoff
                    collisions++;
                    double backoff = (double)backoff_dist(rng);
                    double next_attempt = ev.time + backoff;
                    if (next_attempt <= SIM_TIME) {
                        pq.push(Event{next_attempt, TRANSMIT_START, tx.station_id, -1});
                        // station still has_frame = true (keeps frame until successful)
                    } else {
                        // will not retransmit within simulation window; leave frame pending
                    }
                }

                // If this station had more frames queued, this simplified model doesn't queue them.
                break;
            }
        }
    }

    cout.setf(std::ios::fixed); cout<<setprecision(4);
    cout << "Simulation finished (time <= "<<SIM_TIME<<")\n";
    cout << "Total transmissions (attempts): " << total_transmissions << '\n';
    cout << "Successful transmissions: " << successful << '\n';
    cout << "Collisions (frames lost): " << collisions << '\n';
    double efficiency = total_transmissions? (double)successful / (double)total_transmissions : 0.0;
    cout << "Efficiency (success / total attempts): " << efficiency << '\n';

    return 0;
}
