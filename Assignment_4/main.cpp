#include <iostream>
#include <functional>
#include <memory>
#include <thread>
#include "utils.cpp"
#include "utimer.cpp"
#include "threadpool.cpp"

using namespace std;

int main(int argc, char **argv) {
  
    if(argc != 5 && argc != 4) {
        cout << "Usage is: " << argv[0] << " max-value time_arrival time_computation [num_thread] " << endl;
        return -1;
    }

    int max   = atoi(argv[1]);
    int ta    = atoi(argv[2]);
    int tf    = atoi(argv[3]);
    int tp_size = argc > 4 ? atoi(argv[4]) : thread::hardware_concurrency();


    // Thread pool
    threadPool tp(tp_size);

    function<float(float)> f = [&] (float x) { 
            active_delay(tf);
            float res = x * x;
            cout << res << endl;
            return res;
        };

    for(int i=0;i<max;i++){
        active_delay(ta);
        cout << "Task " << i << endl;
        tp.submit(bind(f,i));
    }

    tp.execute();

    return 0;
}