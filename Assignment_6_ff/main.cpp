#include <iostream>
#include <vector>
#include <functional>
#include <thread>
//#include <mutex>
//#include <condition_variable>
//#include <omp.h>
//#include "utils.cpp"
#include "utimer.cpp"
#include <ff/ff.hpp>
#include <ff/parallel_for.hpp>

using namespace std;
using namespace ff;

void check_results(vector<vector<int>> v){
    for(vector<int> vt : v) {
        for(int i=0; i<vt.size(); i++)
            cout << vt[i] << " ";
        cout << " [END] " << endl;
    }
}

int main(int argc, char** argv){
    if (argc!=5 && argc!=6 || atoi(argv[1]) > atoi(argv[2])) {
        cout << "Usage is: " << argv[0]  << " left right chunk debug [nworkers] (with left <= right) " << endl;
        return -1;
    }

    int left   = atoi(argv[1]);
    int right  = atoi(argv[2]);
    int chunk  = atoi(argv[3]);
    bool debug = atoi(argv[4]) == 0 ? false : true;
    int nw     = argc > 5 ? atoi(argv[5]) : thread::hardware_concurrency(); //FF_AUTO;

    function<bool(int)> isPrime = [] (int n) {
        if (n == 2 || n == 3) return true;
        if (n <= 1 || n % 2 == 0 || n % 3 == 0) return false;
        for (int i = 5; i * i <= n; i += 6)
            if (n % i == 0 || n % (i + 2) == 0) return false;
        return true;
    };

    vector<vector<int>> primes(nw); 

    auto body = [&](const long left, const long right, const long worker) {
        for(long i=left; i<right; ++i)
            if(isPrime(i))
                primes[worker].push_back(i);
    };

    ffTime(START_TIME);
    parallel_for_idx(left, right+1, 1, chunk, body, nw);
    ffTime(STOP_TIME);

    if(debug)
        check_results(primes);

    cout << "Time: " << ffTime(GET_TIME) << " ms " << endl; 

    return 0;
}