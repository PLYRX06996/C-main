// csma_all.cpp
#include <bits/stdc++.h>
using namespace std;

double r(){ return (rand()+1.0)/(RAND_MAX+2.0); }
double ex(double l){ if(l<=0) return 1e9; return -log(r())/l; }
double u(double m){ return r()*m; }

void csma_sim(int n,double T,double ft,double l,int mr,int bw,double pd,
              long long &succ,long long &coll,long long &tot)
{
    vector<double> t; vector<int> rt;
    for(int i=0;i<n;i++){
        double x=0;
        while(1){
            x+=ex(l);
            if(x>=T) break;
            t.push_back(x);
            rt.push_back(0);
        }
    }
    succ=coll=tot=0;
    double busy_until=0;
    while(!t.empty()){
        vector<int> id(t.size()); for(int i=0;i<id.size();++i) id[i]=i;
        sort(id.begin(),id.end(),[&](int a,int b){ return t[a]<t[b]; });
        vector<double> tt; vector<int> tr;
        for(int i:id){ tt.push_back(t[i]); tr.push_back(rt[i]); }
        t.clear(); rt.clear();
        int i=0;
        while(i<tt.size()){
            if(tt[i] < busy_until){
                // sensed busy -> backoff and retry
                if(tr[i] < mr){
                    double nt = tt[i] + u(bw*ft);
                    if(nt < T){ t.push_back(nt); rt.push_back(tr[i]+1); }
                }
                i++;
                continue;
            }
            int j=i+1;
            while(j<tt.size() && tt[j] < tt[i] + pd) j++;
            int g = j - i;
            tot += g;
            if(g==1){
                succ++;
                busy_until = tt[i] + ft; // occupies channel full frame
            } else {
                coll += g;
                busy_until = tt[i] + ft; // collided frames still occupy channel
                for(int k=i;k<j;k++){
                    if(tr[k] < mr){
                        double nt = tt[k] + ft + u(bw*ft);
                        if(nt < T){ t.push_back(nt); rt.push_back(tr[k]+1); }
                    }
                }
            }
            i = j;
        }
    }
}

void csmacd_sim(int n,double T,double ft,double l,int mr,int bw,double pd,double slot,
                long long &succ,long long &coll,long long &tot)
{
    vector<double> t; vector<int> rt;
    for(int i=0;i<n;i++){
        double x=0;
        while(1){
            x+=ex(l);
            if(x>=T) break;
            t.push_back(x);
            rt.push_back(0);
        }
    }
    succ=coll=tot=0;
    double busy_until=0;
    while(!t.empty()){
        vector<int> id(t.size()); for(int i=0;i<id.size();++i) id[i]=i;
        sort(id.begin(),id.end(),[&](int a,int b){ return t[a]<t[b]; });
        vector<double> tt; vector<int> tr;
        for(int i:id){ tt.push_back(t[i]); tr.push_back(rt[i]); }
        t.clear(); rt.clear();
        int i=0;
        while(i<tt.size()){
            if(tt[i] < busy_until){
                // sense busy -> backoff
                if(tr[i] < mr){
                    int k = tr[i]+1;
                    int maxexp = min(k, 10);
                    int rwin = (1<<maxexp) - 1;
                    if(rwin < 1) rwin = 1;
                    int slotcnt = rand() % rwin;
                    double nt = tt[i] + (slotcnt+1)*slot;
                    if(nt < T){ t.push_back(nt); rt.push_back(tr[i]+1); }
                }
                i++;
                continue;
            }
            int j=i+1;
            while(j<tt.size() && tt[j] < tt[i] + pd) j++;
            int g = j - i;
            tot += g;
            if(g==1){
                succ++;
                busy_until = tt[i] + ft;
            } else {
                coll += g;
                // immediate detection -> short collision occupy small time
                busy_until = tt[i] + 0.01; // short abort
                for(int k=i;k<j;k++){
                    if(tr[k] < mr){
                        int kk = tr[k]+1;
                        int maxexp = min(kk,10);
                        int rwin = (1<<maxexp) - 1; if(rwin<1) rwin=1;
                        int slotcnt = rand()%rwin;
                        double nt = tt[k] + (slotcnt+1)*slot;
                        if(nt < T){ t.push_back(nt); rt.push_back(tr[k]+1); }
                    }
                }
            }
            i=j;
        }
    }
}

void csmaca_sim(int n,double T,double ft,double l,int mr,int bw,double pd,double DIFS,double SIFS,
                long long &rtsCnt,long long &ctsCnt,long long &succ,long long &coll)
{
    vector<double> t; vector<int> rt;
    for(int i=0;i<n;i++){
        double x=0;
        while(1){
            x+=ex(l);
            if(x>=T) break;
            t.push_back(x);
            rt.push_back(0);
        }
    }
    rtsCnt=ctsCnt=succ=coll=0;
    double busy_until=0;
    while(!t.empty()){
        vector<int> id(t.size()); for(int i=0;i<id.size();++i) id[i]=i;
        sort(id.begin(),id.end(),[&](int a,int b){ return t[a]<t[b]; });
        vector<double> tt; vector<int> tr;
        for(int i:id){ tt.push_back(t[i]); tr.push_back(rt[i]); }
        t.clear(); rt.clear();
        int i=0;
        while(i<tt.size()){
            double ready = max(tt[i], busy_until) + DIFS;
            // collect other stations that will also finish DIFS close enough -> RTS collision
            int j=i+1;
            while(j<tt.size() && max(tt[j], busy_until) + DIFS < ready + pd) j++;
            int g = j - i;
            rtsCnt += g;
            if(g==1){
                // single RTS -> AP replies CTS after SIFS, then data -> channel busy
                ctsCnt++;
                succ++;
                busy_until = ready + SIFS + ft; // occupy for CTS+data
            } else {
                coll += g; // RTS collision, no CTS
                for(int k=i;k<j;k++){
                    if(tr[k] < mr){
                        double nt = ready + u(bw*ft);
                        if(nt < T){ t.push_back(nt); rt.push_back(tr[k]+1); }
                    }
                }
            }
            i=j;
        }
    }
}

int main(){
    srand(time(NULL));
    int n,mr,bw;
    double T,ft,l;
    double pd,slot;
    double DIFS,SIFS;

    cout<<"Enter number of stations: "; cin>>n;
    cout<<"Enter total simulation time: "; cin>>T;
    cout<<"Enter frame time: "; cin>>ft;
    cout<<"Enter lambda (arrival rate per station): "; cin>>l;
    cout<<"Enter max retries: "; cin>>mr;
    cout<<"Enter backoff window (in frame units): "; cin>>bw;
    cout<<"Enter propagation delay: "; cin>>pd;
    cout<<"Enter slot time (for CSMA/CD exp backoff): "; cin>>slot;
    cout<<"Enter DIFS (for CSMA/CA): "; cin>>DIFS;
    cout<<"Enter SIFS (for CSMA/CA): "; cin>>SIFS;

    long long s1,c1,t1;
    csma_sim(n,T,ft,l,mr,bw,pd,s1,c1,t1);
    cout<<"\n--- CSMA Results ---\n";
    cout<<"Total attempts: "<<t1<<"\n";
    cout<<"Successful: "<<s1<<"\n";
    cout<<"Collisions: "<<c1<<"\n";
    cout<<"Efficiency: "<<(t1==0?0.0: (double)s1/t1)<<"\n";

    long long s2,c2,t2;
    csmacd_sim(n,T,ft,l,mr,bw,pd,slot,s2,c2,t2);
    cout<<"\n--- CSMA/CD Results ---\n";
    cout<<"Total attempts: "<<t2<<"\n";
    cout<<"Successful: "<<s2<<"\n";
    cout<<"Collisions: "<<c2<<"\n";
    cout<<"Efficiency: "<<(t2==0?0.0: (double)s2/t2)<<"\n";

    long long rts,cts,s3,c3;
    csmaca_sim(n,T,ft,l,mr,bw,pd,DIFS,SIFS,rts,cts,s3,c3);
    cout<<"\n--- CSMA/CA Results ---\n";
    cout<<"RTS attempts: "<<rts<<"\n";
    cout<<"CTS responses: "<<cts<<"\n";
    cout<<"Successful frames: "<<s3<<"\n";
    cout<<"RTS collisions: "<<c3<<"\n";
    cout<<"Efficiency (success/RTS): "<<(rts==0?0.0:(double)s3/rts)<<"\n";

    return 0;
}