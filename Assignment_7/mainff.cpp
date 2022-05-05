#include <iostream>
#include <vector>
#include <functional>
#include <thread>
//#include <barrier>
//#include <future>
//#include <utility>
//#include <mutex>
//#include <condition_variable>
//#include <omp.h>
//#include "utils.cpp"
#include "utimer.cpp"
#include <ff/ff.hpp>

using namespace std;
using namespace ff;

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

vector<pair<int,int>> overlapped_chunks(int n, int nw, int over) {
    vector<pair<int,int>> chunks(nw);
    int chunk_size = n / nw;
    for(int i=0; i<nw; i++) {
        if(i!=0 && i!=nw-1)
            chunks[i] = make_pair(i*chunk_size-over, (i+1)*chunk_size-1+over);
        else if(i==0)
            chunks[i] = make_pair(i*chunk_size, (i+1)*chunk_size-1+over);
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



    if(debug)
        read_v(v);

    return 0;
}