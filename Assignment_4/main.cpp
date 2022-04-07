#include <iostream>
//#include <vector>
#include <functional>
#include <memory>
#include <thread>
#include "utils.cpp"
#include "utimer.cpp"
#include "sharedqueue.cpp"

#define EOS -1

using namespace std;

int main(int argc, char **argv) {
  
    if(argc != 5) {
        cout << "Usage is: " << argv[0] << " max-value seed par1 par2 " << endl;
        return -1;
    }

    int max   = atoi(argv[1]);
    int seed  = atoi(argv[2]);
    int a  = atoi(argv[3]);
    int b  = atoi(argv[4]);

    srand(seed);

    auto f = [&] (int x, int a, int b) { 
        active_delay(1000);
        return (x + a) * b;
        };

    auto f_ab = bind(f, placeholders::_1, a, b);

    sharedQueue<int> q;

    auto input_streaming = [&] (int max) {
        for(int i=0;i<max;i++){
            active_delay(100);
            q.push(i);
        }
        q.push(EOS);
    };

    auto work = [&] () {
        while(true) {
            auto item = q.pop();
            if(item == EOS) {
                break;
            }
            cout << f_ab(item)<< " from " << item << endl;
        }
    };

    thread tid1(input_streaming, max);
    thread tid2(work);

    tid1.join();
    tid2.join();

    return 0;
}