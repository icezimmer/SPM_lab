#include <iostream>
#include <vector>
#include <functional>
#include <thread>
#include "utimer.cpp"
#include <ff/ff.hpp>
#include <ff/parallel_for.hpp>

using namespace std;
using namespace ff;

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

    {
        utimer paromp("FastFlow parallel");

        // Create the object ParallelFor
        ParallelFor pf(nw, true); // param spinwaittrue: true non blocking, false blocking.

        int it = 0;
        while(it < k){
            
            pf.parallel_for(1,n-1,1,0,
                [&] (const int i) {
                    for(int i=1; i<n-1; i++)
                        buffer[i] = (v[i-1] + v[i] + v[i+1]) / 3.0;
                    });
            // implicit barrier
            it = it + 1;

            pf.parallel_for(1,n-1,1,0,
                [&] (const int i) {
                    for(int i=1; i<n-1; i++)
                        v[i] = (buffer[i-1] + buffer[i] + buffer[i+1]) / 3.0;
                    });
            // implicit barrier
            it = it+1;
        }
    }

    if(debug) {
        if(k%2 ==0)
            read_v(v);
        else
            read_v(buffer);
    }

    return 0;
}