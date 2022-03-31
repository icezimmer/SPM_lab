# include <iostream>
# include <vector>
# include <barrier>
# include <thread>
# include <future>
# include <utility>
# include <functional>
# include "utimer.cpp"

using namespace std;

void compute_thread(function <void(int)> fun, int nw) {
    utimer compt("Parallel computing by threads");
    vector<thread*> tids(nw);
    for(int worker=0; worker<nw; worker++)
        tids.emplace_back(fun, worker);
    return;
    for(int worker=0; worker<nw; worker++)
        tids[worker]->join();
    return;
}

vector<pair<int,int>> overlapped_chunks(int n, int nw, int over) {
    vector<pair<int,int>> chunks(nw);
    int chunk_size = n / nw;
    for(int i=0; i<nw; i++) {
        if(i!=0 && i!=(nw-1))
            chunks[i] = make_pair(i*chunk_size-over, (i+1)*chunk_size-1+over);
        else if(i==0) 
            chunks[i] = make_pair(i*chunk_size, (i+1)*chunk_size-1+over);
        else 
            chunks[i] = make_pair(i*chunk_size-over, n-1);
    }
    return chunks;
}

void odd_even_sort(vector<int> v, int nw) {
    vector<pair<int,int>> ochunks = overlapped_chunks(v.size(), nw, 1);
    auto on_completion = [] () noexcept {
        cout << "... done" << endl;
    };
    barrier sync_point(nw, on_completion);
    function<void(int)> work = [&] (int worker) {
        bool sorted = false;
        while(not(sorted)) {
            for(int i=ochunks[worker].first+1; i<ochunks[worker].second-1; i+=2)
	            if(v[i] > v[i+1]) { swap(v[i], v[i+1]); sorted = false; }
            for( int i=i=ochunks[worker].first; i<ochunks[worker].second-1; i+=2)
                if(v[i] > v[i+1]) { swap(v[i], v[i+1]); sorted = false; }
        }
        sync_point.arrive_and_wait();
        return;
    };
    compute_thread(work, nw);
    return;
}