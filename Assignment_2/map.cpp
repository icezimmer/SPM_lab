# include <iostream>
# include <vector>
# include <thread>
# include <vector>
# include <future>
# include <chrono>
# include <utility>
# include <functional>
# include <string>
# include "utimer.cpp"

using namespace std;

vector<float> create_random_vector(int n, int max, int seed) {
    srand(seed);
    vector<float> v(n);
    for(int i=0; i<n; i++) {
        v[i] =  static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max)));;
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

void read_vector(vector<float> v) {
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

void check_results(vector<float> res){
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

vector<float> map(vector<float> v, function<float(float)> f, int nw, bool chunks, bool th) {
    vector<float> res(v.size());
    if(chunks) {
        utimer ch("Parallel computing using chunks");
        vector<pair<int,int>> chunks = make_chunks(v.size(), nw);
        function<void(int)> body = [&] (int j) {
            for(int i=chunks[j].first; i<=chunks[j].second; i++)
	            res[i] = f(v[i]);
            return;
        };
        if(th)
            compute_thread(body, nw);
        else
            compute_async(body, nw);
    }
    else {
        utimer rr("Parallel computing using round-robin");
        function<void(int)> body = [&] (int j) {
            for(int i=j; i<v.size(); i+=nw)
	            res[i] = f(v[i]);
            return;
        };
        if(th)
            compute_thread(body, nw);
        else
            compute_async(body, nw);
    }
    return res;
}

int main(int argc, char **argv) {
  
    if(argc != 7) {
        cout << "Usage is: " << argv[0] << " v-len max-v seed n-worker chunks/round-robin thread/async " << endl;
        return(0);
    }

    int  n      = atoi(argv[1]);
    int  max    = atoi(argv[2]);
    int  seed   = atoi(argv[3]);
    int  nw     = atoi(argv[4]);
    bool chunks = atoi(argv[5]);
    bool th     = atoi(argv[6]);

    vector<float> v = create_random_vector(n, max, seed);

    function<float(float)> f = [] (float x) { active_delay(1); return 2 * x; };

    vector<float> res = map(v, f, nw, chunks, th);

    return 0;
}