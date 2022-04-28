#include <iostream>
#include <vector>
#include <functional>
#include <thread>
#include "utils.cpp"
#include "utimer.cpp"

using namespace std;

void check_results(vector<bool> v, int left){
    for(int i=0; i<v.size(); i++) {
        cout << left + i << ":" << v[i] << "; ";
    }
    cout << endl;

} 

int main(int argc, char **argv) {
  
    if(argc != 4 && argc != 5 || atoi(argv[1]) > atoi(argv[2])) {
        cout << "Usage is: " << argv[0] << " left right debug [nw] (with left <= right) "<< endl;
        return -1;
    }

    int left   = atoi(argv[1]);
    int right  = atoi(argv[2]);
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

    vector<bool> primes_nat(right-left+1);
    long t_nat;
    {    
        utimer myprime("Native parallel", &t_nat);

        function<void(int)> body = [&] (int worker) {
            for(int i=left+worker; i<=right; i+=nw)
                primes_nat[i-left] = isPrime(i);
            return;
        };

        vector<thread> threads;

        for(int worker=0; worker<nw; worker++)
            threads.emplace_back(body, worker);

        for(auto& thread : threads)
            thread.join();
    }
    if(debug) check_results(primes_nat, left);

    vector<bool> primes_omp(right-left+1);
    long t_omp;
    {    
        utimer myprime("OpenMP parallel", &t_omp);

        #pragma parallel for num_thread(nw)
        for(int i=left; i<=right; i++) {
            primes_omp[i-left] = isPrime(i); 
        }
    }
    if(debug) check_results(primes_omp, left);

    return 0;
}