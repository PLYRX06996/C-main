#include <bits/stdc++.h>
using namespace std;

double r() { return (rand() + 1.0) / (RAND_MAX + 2.0); }
double ex(double l)
{
    if (l <= 0)
        return 1e9;
    return -log(r()) / l;
}
double u(double m) { return r() * m; }

void pureAloha(int n, double T, double ft, double l, int mr, int bw,
               long long &s, long long &c, long long &tot)
{
    vector<double> t;
    vector<int> st, rt;

    for (int i = 0; i < n; i++)
    {
        double x = 0;
        while (1)
        {
            x += ex(l);
            if (x >= T)
                break;
            t.push_back(x);
            st.push_back(i);
            rt.push_back(0);
        }
    }

    s = c = tot = 0;

    while (!t.empty())
    {
        vector<int> id(t.size());
        for (int i = 0; i < id.size(); i++)
            id[i] = i;
        sort(id.begin(), id.end(), [&](int a, int b)
             { return t[a] < t[b]; });

        vector<double> nt;
        vector<int> ns, nr;
        nt.reserve(t.size());
        ns.reserve(t.size());
        nr.reserve(t.size());

        for (int k : id)
        {
            nt.push_back(t[k]);
            ns.push_back(st[k]);
            nr.push_back(rt[k]);
        }

        t.clear();
        st.clear();
        rt.clear();

        int i = 0;
        while (i < nt.size())
        {
            int j = i + 1;
            while (j < nt.size() && nt[j] < nt[i] + ft)
                j++;
            int g = j - i;
            tot += g;

            if (g == 1)
            {
                s++;
            }
            else
            {
                c += g;
                for (int k = i; k < j; k++)
                {
                    if (nr[k] < mr)
                    {
                        double ntm = nt[k] + u(bw * ft);
                        if (ntm < T)
                        {
                            t.push_back(ntm);
                            st.push_back(ns[k]);
                            rt.push_back(nr[k] + 1);
                        }
                    }
                }
            }
            i = j;
        }
    }
}

void slottedAloha(int n, double T, double ft, double l,
                  long long &sl, long long &s, long long &c, long long &tot)
{
    sl = max(1, (int)floor(T / ft));
    vector<int> cnt(sl, 0);

    for (int i = 0; i < n; i++)
    {
        double x = 0;
        while (1)
        {
            x += ex(l);
            if (x >= T)
                break;
            int k = (int)floor(x / ft);
            if (k >= 0 && k < sl)
                cnt[k]++;
        }
    }

    s = c = tot = 0;
    for (int i = 0; i < sl; i++)
    {
        if (cnt[i] == 1)
        {
            s++;
            tot++;
        }
        else if (cnt[i] > 1)
        {
            c++;
            tot += cnt[i];
        }
    }
}

int main()
{
    srand(time(NULL));

    int n, mr, bw;
    double T, ft, l;

    cout << "Enter number of stations: ";
    cin >> n;
    cout << "Enter total simulation time: ";
    cin >> T;
    cout << "Enter frame time: ";
    cin >> ft;
    cout << "Enter lambda (arrival rate per station): ";
    cin >> l;
    cout << "Enter max retries (Pure ALOHA): ";
    cin >> mr;
    cout << "Enter backoff window (in frame units): ";
    cin >> bw;

    long long ps, pc, pt;
    pureAloha(n, T, ft, l, mr, bw, ps, pc, pt);
    double pe = (pt == 0 ? 0 : (double)ps / pt);

    long long sl, ss, sc, st;
    slottedAloha(n, T, ft, l, sl, ss, sc, st);
    double se = (st == 0 ? 0 : (double)ss / st);

    cout << "\n--- Pure ALOHA Results ---\n";
    cout << "Total Transmissions: " << pt << "\n";
    cout << "Successful: " << ps << "\n";
    cout << "Collisions: " << pc << "\n";
    cout << "Efficiency: " << pe << "\n";

    cout << "\n--- Slotted ALOHA Results ---\n";
    cout << "Slots: " << sl << "\n";
    cout << "Total Transmissions: " << st << "\n";
    cout << "Successful Slots: " << ss << "\n";
    cout << "Collision Slots: " << sc << "\n";
    cout << "Efficiency: " << se << "\n";

    return 0;
}