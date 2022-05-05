#include <iostream>
#include <vector>
#include <functional>
#include <thread>
#include <omp.h>
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

int main(int argc, char** argv) {
    if (argc!=4 && argc!=5) {
        cout << "Usage is: " << argv[0]  << " n k debug [nworkers] " << endl;
        return -1;
    }

    int  n     = atoi(argv[1]);
    int  k     = atoi(argv[2]);
    bool debug = atoi(argv[3]) == 0 ? false : true;
    int  nw    = argc > 4 ? atoi(argv[4]) : thread::hardware_concurrency(); //FF_AUTO;

    vector<float> v = start_v(n);
    vector<float> buffer = v;

    if(debug)
        read_v(v);


    {
        utimer paromp("OpenMP parallel");

        int it = 0;
        while(it < k){

            omp_set_num_threads(nw);
            #pragma omp parallel for
            for(int i=1; i<n-1; i++)
                buffer[i] = (v[i-1] + v[i] + v[i+1]) / 3;

            v = buffer;
            it = it+1;
        }
    }

    if(debug)
        read_v(v);

    return 0;
}