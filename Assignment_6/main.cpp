#include <iostream>
#include <vector>
#include <functional>
#include <thread>
#include <mutex>
//#include <condition_variable>
#include <omp.h>
#include "utils.cpp"
#include "utimer.cpp"

using namespace std;

void check_results(vector<int> v){
    for(int i=0; i<v.size(); i++) {
        cout << v[i] << " ";
    }
    cout << endl;
} 

int main(int argc, char **argv) {
  
    if(argc != 4 && argc != 5 || atoi(argv[1]) > atoi(argv[2])) {
        cout << "Usage is: " << argv[0] << " left right debug [nw] (with left <= right) "<< endl;
        return -1;
    }

    int left   = atoi(argv[1]);
    int right   = atoi(argv[2]);
    bool debug = (atoi(argv[3]) == 0 ? false : true);
    int nw = argc > 4 ? atoi(argv[4]) : thread::hardware_concurrency();

    function<bool(int)> isPrime = [] (int n) {
        if (n == 2 || n == 3) return true;
        if (n <= 1 || n % 2 == 0 || n % 3 == 0) return false;
        for (int i = 5; i * i <= n; i += 6){
            if (n % i == 0 || n % (i + 2) == 0) return false;
        }
        return true;
    };

    vector<int> primes_nat;

    long t_nat;
    {    
        utimer myprime("Native parallel", &t_nat);

        mutex m_push;
        //condition_variable c;
        function<void(int)> body = [&] (int worker) {
            for(int i=left+worker; i<=right; i+=nw)
                if(isPrime(i)){
                    //unique_lock<mutex> l(m_push);
                    lock_guard<mutex> lg(m_push);
                    primes_nat.push_back(i);
                }
                //c.notify_one();
            return;
        };

        vector<thread> threads;

        for(int worker=0; worker<nw; worker++)
            threads.emplace_back(body, worker);

        for(auto& thread : threads)
            thread.join();
    }

    if(debug) check_results(primes_nat);

    vector<int> primes_omp;

    long t_omp;
    {    
        utimer myprime("OpenMP parallel", &t_omp);

        #pragma parallel for num_thread(nw)
        for(int i=left; i<=right; i++) {
            if(isPrime(i)){
                #pragma omp critical
                    primes_omp.push_back(i); 
            }
        }
    }

    if(debug) check_results(primes_omp);

    return 0;
}
