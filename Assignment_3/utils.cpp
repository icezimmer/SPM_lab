# include <iostream>
# include <vector>
# include <functional>
# include "utimer.cpp"

using namespace std;

vector<int> create_random_vector(int n, int max, int seed) {
    srand(seed);
    vector<int> v(n);
    for(int i=0; i<n; i++) {
        v[i] =  rand() % max;
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

void read_vector(vector<int> v) {
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