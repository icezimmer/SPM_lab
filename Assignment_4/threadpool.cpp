/*
 
  Shared queue, one producer, multiple consumers. 
  We assume the queue could never be "filled". 
  Queue item producer just mutex and notify
  Queue item consumer may block upon empty queue

 */

#include <mutex>
#include <queue>
#include <condition_variable>
#include <functional>
#include <memory>
#include <vector>


using namespace std;

class threadPool {

private:
    queue<function<float(void)>> tasks;
    mutex m;
    condition_variable c;
    int size;
    bool stop;

    /*void input_streaming(int max, int ta, int tf) {
        function<float(float)> f = [&] (float x) { 
            active_delay(tf);
            return x * x;
            };
        for(int i=0;i<max;i++){
            active_delay(ta);
            cout << "Task " << i << endl;
            {
                unique_lock<mutex> l(this->m);
                this->tasks.push(bind(f, i));
            }
            this->c.notify_one();
        }
    }*/

    void body() {
        while(true) {
            function<float(void)> tsk=[](){return 0;}; //dummy initialization
            {
                unique_lock<mutex> l(this->m);
                this->c.wait(l, [&]{ return (!this->tasks.empty() || this->stop); }); // wait until queue is not empty or stopped
                if(!this->tasks.empty()){
                    tsk = this->tasks.front();
                    this->tasks.pop();
                }
                if(this->stop) return;    
            }
            tsk();
        }
    }

    void stopThreadPool() {
        {
            unique_lock<mutex> l(this->m);
            this->stop = true;
        }
        this->c.notify_all();
    }

public:

    threadPool(int size) {
        this->size = size;
        this->stop = false;
    }

    void execute() {
        vector<thread*> tids(this->size);

        for(int worker=0; worker<this->size; worker++)
            tids[worker] = new thread([this](){ this->body(); });

        string line;
        cout << "Terminate?" << endl;
        cin >> line;

        stopThreadPool();

        for(int worker=0; worker<this->size; worker++)
            tids[worker]->join();
    }

    void submit(function<float(void)> f) {
        {
            unique_lock<mutex> l(this->m);
            this->tasks.push(f);
        }
        this->c.notify_one();
    }

  
};
