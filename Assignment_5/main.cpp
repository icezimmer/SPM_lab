#include <iostream>
#include <functional>
//#include <memory>
#include <thread>
#include <omp.h>
#include "sharedqueue.cpp"
#include "utils.cpp"
#include "utimer.cpp"

int main(int argc, char **argv) {
  
    if(argc != 5 && argc != 4) {
        cout << "Usage is: " << argv[0] << " max-value time_arrival time_computation [num_thread] " << endl;
        return -1;
    }

    int max   = atoi(argv[1]);
    int ta    = atoi(argv[2]);
    int tf    = atoi(argv[3]);
    int nw = argc > 4 ? atoi(argv[4]) : thread::hardware_concurrency();

    function<float(float)> f = [&] (float x) { 
        active_delay(tf);
        float res = x * x;
        cout << res << endl;
        return res;
        };

    sharedQueue<function<float(void)>> q;

    #pragma omp parallel num_threads(nw)
    {
        #pragma omp single
        {
            for(int i=0;i<max;i++){
                active_delay(ta);
                cout << "Task " << i << endl;
                q.push(bind(f, i));
            }
        }

        while(true) {
            auto item = q.pop();
            item();
        }
    }
}
