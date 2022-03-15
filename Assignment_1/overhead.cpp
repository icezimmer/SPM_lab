# include <iostream>
# include <vector>
# include <thread>
# include <vector>
# include <future>
# include <chrono>
# include <utility>
# include <functional>
# include "utimer.cpp"

using namespace std;

vector<int> create_random_vector(int n, int max) {
    vector<int> v(n);
    for(int i=0; i<n; i++) {
        v[i] = rand() % max;
    }
    return v;
}

void active_delay(int msecs) {
    // read current time
    auto start = std::chrono::high_resolution_clock::now();
    auto end   = false;
    while(!end) {
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
        if(msec>msecs)
            end = true;
    }
    return;
}

void reading_vector(vector<int> v) {
    utimer read("Reading");
    for(int i=0; i<v.size(); i++)
        cout << "v[" << i << "] is " << v[i] << endl;
    return;
}

void check_chunks(vector<pair<int,int>> chunks) {
    utimer check("Checking chunks");
    for(int i=0; i<chunks.size(); i++)
        cout << "chunk " << i << " is " << chunks[i].first << "," << chunks[i].second << endl;
    return;
}

void check_results(vector<int> res){
    utimer checkt("Checking results");
    for(int i=0; i<res.size(); i++)
        cout << "res[" << i << "] is " << res[i] << endl;
    return;
}

void compute_thread(function<void(int)> fun, int nw) {
    utimer compt("Parallel computing by threads");
    vector<thread*> tids(nw);
    for(int i=0; i<nw; i++)
        tids[i] = new thread(fun, i);
    for(int i=0; i<nw; i++)
        tids[i]->join();
    return;
}

void compute_async(function<void(int)> fun, int nw) {
    utimer compt("Parallel computing by async");
    vector<shared_future<void>> atids(nw);
    for(int i=0; i<nw; i++)
        atids[i] = async(launch::async, fun, i);
    for(int i=0; i<nw; i++)
        try {
	        atids[i].get();
        } catch (const std::exception& e) {
	        cout << "Got " << e.what() << " i= " << i << endl;
        }
    return;
}

vector<pair<int,int>> make_chunks(int n, int nw) {
    utimer ovh("Overhead");
    vector<pair<int,int>> chunks(nw);
    int chunk_size = n / nw;
    for(int i=0; i<nw; i++) {
        if(i!=(nw-1))
            chunks[i] = make_pair(i*chunk_size, (i+1)*chunk_size-1);
        else
            chunks[i] = make_pair(i*chunk_size, n-1);
    }
    active_delay(10);
    return chunks;
}

int odouble(int x) {
    active_delay(1);
    return 2 * x;
}

int main(int argc, char **argv) {
  
    if(argc == 1) {
        cout << "Usage is: " << argv[0] << " v-len max-v n-worker seed " << endl;
        return(0);
    }

    int n    = atoi(argv[1]);
    int max  = atoi(argv[2]);
    int nw   = atoi(argv[3]);
    int seed = atoi(argv[4]);

    vector<int> v = create_random_vector(n, max);
    vector<int> res(n);

    {
        utimer tim("Parallel computation");

        vector<pair<int,int>> chunks = make_chunks(n, nw);

        auto body = [&] (int j) {
            for(int i=chunks[j].first; i<=chunks[j].second; i++)
	            res[i] = odouble(v[i]);
            return;
        };

        compute_thread(body, nw);
    }

    return 0;
}