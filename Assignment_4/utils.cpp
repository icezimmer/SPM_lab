#include <iostream>
#include <vector>
#include <functional>
#include "utimer.cpp"

using namespace std;

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
        cout << v[i] << " ";
    cout << endl;
    return;
}