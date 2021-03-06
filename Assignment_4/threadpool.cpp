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
    vector<thread> threads;
    mutex m;
    condition_variable c;
    int size;
    bool stop;

    void body() {
        while(true) {
            function<float(void)> tsk=[](){return 0;}; //dummy initialization
            {
                unique_lock<mutex> l(this->m);
                this->c.wait(l, [&](){ return (!this->tasks.empty() || this->stop); }); // wait until queue is not empty or the threadpool is stopped
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
        for(int worker=0; worker<this->size; worker++)
            this->threads.emplace_back([this](){ this->body(); });
    }

    ~threadPool() {
        string line;
        cout << "Terminate?" << endl;
        cin >> line;

        stopThreadPool();

        for(auto& thread : this->threads)
            thread.join();
    }

    void submit(function<float(void)> f) {
        {
            unique_lock<mutex> l(this->m);
            this->tasks.push(f);
        }
        this->c.notify_one();
    }

  
};
