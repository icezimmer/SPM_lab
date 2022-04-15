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
  
    if(argc != 5) {
        cout << "Usage is: " << argv[0] << " max-value seed time_arrival time_computation " << endl;
        return -1;
    }

    int max   = atoi(argv[1]);
    int seed  = atoi(argv[2]);
    int ta    = atoi(argv[3]);
    int tf    = atoi(argv[4]);

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

    thread tid1(input_streaming, max);
    thread tid2(work);

    tid1.join();
    tid2.join();

    return 0;
}