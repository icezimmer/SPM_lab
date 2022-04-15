#include <iostream>
#include <functional>
#include <memory>
#include <thread>
#include "utils.cpp"
#include "utimer.cpp"
#include "sharedqueue.cpp"

//#define EOS -1

using namespace std;

int main(int argc, char **argv) {
  
    if(argc != 6) {
        cout << "Usage is: " << argv[0] << " max-value seed time_arrival time_computation num_thread " << endl;
        return -1;
    }

    int max   = atoi(argv[1]);
    int seed  = atoi(argv[2]);
    int ta    = atoi(argv[3]);
    int tf    = atoi(argv[4]);
    int thread_pool_size = argc > 5 ? atoi(argv[5]) : thread::hardware_concurrency() - 1;

    srand(seed);

    function<float(float)> f = [&] (float x) { 
        active_delay(tf);
        return x * x;
        };

    sharedQueue<function<float(void)>> q;

    auto input_streaming = [&] (int max) {
        for(int i=0;i<max;i++){
            active_delay(ta);
            q.push(bind(f, i));
        }
        //q.push(EOS);
    };

    auto work = [&] () {
        while(true) {
            auto item = q.pop();
            //if(item == EOS) {
            //    break;
            //}
            cout << item() << endl;
        }
    };

    // Streaming thread
    thread tid_str(input_streaming, max);

    // Thread pool
    vector<thread*> tids(thread_pool_size);
    for(int worker=0; worker<thread_pool_size; worker++)
        tids[worker] = new thread(work);

    
    // Await their termination 
    tid_str.join();
    for(int worker=0; worker<thread_pool_size; worker++)
        tids[worker]->join();

    return 0;
}