# include <iostream>
# include <vector>
# include <thread>
# include <future>
# include <utility>
# include <functional>
# include "utimer.cpp"

using namespace std;

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