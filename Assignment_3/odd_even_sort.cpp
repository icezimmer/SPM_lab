#include <iostream>
#include <vector>
#include <barrier>
#include <thread>
#include <future>
#include <utility>
#include <functional>
#include "utimer.cpp"

using namespace std;

void compute_thread(function <void(int)> fun, int nw) {
    utimer compt("Parallel computing by threads");
    // Create threads
    vector<thread*> tids(nw);
    for(int worker=0; worker<nw; worker++)
        tids[worker] = new thread(fun, worker);
    // Await their termination 
    for(int worker=0; worker<nw; worker++)
        tids[worker]->join();
    return;
}

vector<pair<int,int>> overlapped_chunks(int n, int nw, int over) {
    vector<pair<int,int>> chunks(nw);
    int chunk_size = n / nw;
    for(int i=0; i<nw; i++) {
        if(i!=(nw-1))
            chunks[i] = make_pair(i*chunk_size, (i+1)*chunk_size-1+over);
        else
            chunks[i] = make_pair(i*chunk_size, n-1);
    }
    return chunks;
}

void odd_even_sort_sequential(vector<int> &v) {
    bool sorted = false; 
    while(not(sorted)) {
        sorted = true;
        // Odd phase
        for(int i=1; i<v.size()-1; i+=2)
            if(v[i] > v[i+1]) { swap(v[i], v[i+1]); sorted = false; }
        // Even phase  
        for(int i=0; i<v.size()-1; i+=2)
            if(v[i] > v[i+1]) { swap(v[i], v[i+1]); sorted = false; }
    }
    return;
}

void odd_even_sort_parallel(vector<int> &v, int nw) {
    vector<pair<int,int>> ochunks = overlapped_chunks(v.size(), nw, 1);
    //check_chunks(ochunks);
        bool sorted = false; // doesn't need any protection mechanism because can be set only to false (in other cases use atomic var.)
    auto callback_odd = [&] () {
        sorted = true; // now we are sure that all threads runned the code before the barrier 
        //static auto phase = "... odd phase done\n" "Cleaning up...\n";
        //cout << phase;
        //phase = "... odd phase done\n";
    };
    barrier sync_odd(nw, callback_odd);
    barrier sync_even(nw, [&] () {
        //static auto phase = "... even phase done\n" "Cleaning up...\n";
        //cout << phase;
        //phase = "... even phase done\n";
        return;
        });
    function<void(int)> work = [&] (int worker) {
        bool local_sorted = false; 
        while(not(sorted)) {
            local_sorted = true;
            // Odd phase
            for(int i=ochunks[worker].first+1; i<ochunks[worker].second; i+=2)
	            if(v[i] > v[i+1]) { swap(v[i], v[i+1]); local_sorted = false; }
            // Wait all
            sync_odd.arrive_and_wait();
            // Even phase (starts with global sorted true)
            for(int i=ochunks[worker].first; i<ochunks[worker].second; i+=2)
                if(v[i] > v[i+1]) { swap(v[i], v[i+1]); local_sorted = false; }
            //if(sorted && not(local_sorted)) in this case every thread check the global var. sorted (bad!)
            if(not(local_sorted))
                if(sorted)
                    sorted = false;
            // Wait all
            sync_even.arrive_and_wait();
        }
        return;
    };
    //std::cout << "Starting...\n";
    std::vector<thread> threads;
    for(int worker=0;worker<nw;worker++) {
        threads.emplace_back(work, worker);
    }
    for(auto& thread : threads) {
        thread.join();
    }
    //compute_thread(work, nw);
    return;
}