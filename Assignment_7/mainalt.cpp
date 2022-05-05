#include <iostream>
#include <vector>
#include <functional>
#include <thread>
#include <barrier>
#include "utimer.cpp"

using namespace std;

vector<float> start_v(int n) {
    vector<float> v(n);
    v[0] = 0.0;
    v[n/2] = 25.0;
    v[n-1] = 100.0;
    return v;
}

void read_v(vector<float> &v) {
    cout << "[ ";
    for(int i=0; i<v.size(); i++) {
        if (i==v.size()-1)
            cout << v[i];
        else
            cout << v[i] << ", ";
    }
    cout << "]" << endl;
    return;
}

vector<vector<float>> split_v(vector<float> v, int nw) {
    vector<vector<float>> vth(nw);
    int split_size = v.size() / nw;
    for(int i=0; i<nw; i++) {
        if(i!=0 && i!=nw-1)
            for(int j=i*split_size-1; j<=(i+1)*split_size-1+1; j++)
                vth[i].push_back(v[j]);
        else if(i==0)
            for(int j=i*split_size; j<=(i+1)*split_size-1+1; j++)
                vth[i].push_back(v[j]);
        else if(i==nw-1)
            for(int j=i*split_size-1; j<=v.size()-1; j++)
                vth[i].push_back(v[j]);
    }
    return vth;
}

void reconstruct(vector<vector<float>> vth, vector<float> &v) {
    int i = 0;
    int k = 0;
    for(vector<float> vs : vth) {
        if(k!=0 && k!=vth.size()-1)
            for(int j=1; j<=vs.size()-2; j++) {
                v[i] = vs[j];
                i=i+1;
            }
        else if(k==0)
            for(int j=0; j<=vs.size()-2; j++) {
                v[i] = vs[j];
                i=i+1;
            }
        else if(k==vth.size()-1)
            for(int j=1; j<=vs.size()-1; j++) {
                v[i] = vs[j];
                i=i+1;
            }
        k = k+1;
    }
}

void check_split(vector<vector<float>> vth) {
    for(vector<float> v : vth) {
        for(int j=0; j<v.size(); j++)
            cout << v[j] << " ";
        cout << endl;
    }
    return;
}

int main(int argc, char** argv) {
    if (argc!=4 && argc!=5) {
        cout << "Usage is: " << argv[0]  << " n k debug [nworkers] " << endl;
        return -1;
    }

    int n      = atoi(argv[1]);
    int k      = atoi(argv[2]);
    bool debug = atoi(argv[3]) == 0 ? false : true;
    int nw     = argc > 4 ? atoi(argv[4]) : thread::hardware_concurrency(); //FF_AUTO;

    vector<float> v = start_v(n);
    vector<vector<float>> vth = split_v(v, nw); 

    if(debug) {
        read_v(v);
        check_split(vth);
    }


    auto body = [&] (int worker) {
        vector<float> buffer = vth[worker];
        int it = 0;
        while(it < k) {
            for(int i=1; i<=vth[worker].size()-2; i++)
                buffer[i] = (vth[worker][i-1] + vth[worker][i] + vth[worker][i+1]) / 3;
            vth[worker] = buffer;
            it = it+1;
        }
    };

    {   
        utimer parnat("Native parallel"); 

        vector<thread> threads;

        for(int worker=0; worker<nw; worker++)
            threads.emplace_back(body, worker);

        for(auto& thread : threads)
            thread.join();

        reconstruct(vth, v);
    }

    if(debug){
        check_split(vth);
        read_v(v);
    }

    return 0;
}