#include <iostream>
#include <functional>
#include <memory>
#include <thread>
#include <future>
#include "utils.cpp"
#include "utimer.cpp"
#include "threadpool2.cpp"

using namespace std;

int main(int argc, char **argv) {
  
    if(argc != 4 && argc != 5) {
        cout << "Usage is: " << argv[0] << " max-value time_arrival time_computation [num_thread] " << endl;
        return -1;
    }

    int max   = atoi(argv[1]);
    int ta    = atoi(argv[2]);
    int tf    = atoi(argv[3]);
    int tp_size = argc > 4 ? atoi(argv[4]) : thread::hardware_concurrency();

    // Thread pool
    {
        threadPool tp(tp_size);

        mutex m_print;
        function<void(float)> f = [&] (float x) { 
                active_delay(tf);
                auto res = x * x;
                {
                    lock_guard<mutex> lg(m_print);
                    cout << res << endl;
                }
                return;
            };

        auto input_streaming = [&] (int max) {
            for(int i=0;i<max;i++){
                active_delay(ta);
                cout << "Task " << i << endl;
                auto fx = bind(f,i);
                tp.submit(fx); //submit the task
            }
        };

        thread tid_str(input_streaming, max);
        tid_str.join();

        tp.submit([](){cout << "Ciao, questa è una prova " << endl; return;});

        tp.submit([](){cout << "Chi è più veloce? " << endl; return;});
    }


    return 0;
}