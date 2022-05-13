#include <iostream>
#include <vector>
#include <functional>
#include <thread>
#include <barrier>
#include "utimer.cpp"

using namespace std;

vector<float> start_v(int n) {
    vector<float> v(n, 25.0);
    v[0] = 0.0;
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

vector<pair<int,int>> overlapped_chunks(int n, int nw, int over) {
    vector<pair<int,int>> chunks(nw);
    int chunk_size = n / nw;
    for(int i=0; i<nw; i++) {
        if(i!=0 && i!=nw-1)
            chunks[i] = make_pair(i*chunk_size-over, (i+1)*chunk_size-1+over);
        else if(i==0)
            chunks[i] = nw > 1 ? make_pair(i*chunk_size, (i+1)*chunk_size-1+over) : make_pair(0, n-1);
        else if(i==nw-1)
            chunks[i] = make_pair(i*chunk_size-over, n-1);
    }
    return chunks;
}

void check_chunks(vector<pair<int,int>> chunks) {
    for(int i=0; i<chunks.size(); i++)
        cout << "chunk " << i << " is " << chunks[i].first << "," << chunks[i].second << endl;
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
    vector<float> buffer = v;

    if(debug)
        read_v(v);

    vector<pair<int,int>> ochunks = overlapped_chunks(n, nw, 1);

    if(debug)
        check_chunks(ochunks);

    auto callback = [] () { return; };
    barrier sync_point(nw, callback);

    // To avoid copy buffer to v at each iteration we apply the stencil alternately to v and buffer
    auto body = [&] (int worker) {
        int it = 0;
        while(it < k){
            for(int i=ochunks[worker].first+1; i<=ochunks[worker].second-1; i++)
                buffer[i] = (v[i-1] + v[i] + v[i+1]) / 3.0;
            // Wait all 
            sync_point.arrive_and_wait();
            it = it + 1;

            for(int i=ochunks[worker].first+1; i<=ochunks[worker].second-1; i++)
                v[i] = (buffer[i-1] + buffer[i] + buffer[i+1]) / 3.0;
            // Wait all 
            sync_point.arrive_and_wait();
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
    }

    if(debug) {
        if(k%2 ==0)
            read_v(v);
        else
            read_v(buffer);
    }

    return 0;
}