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
#include <future>


using namespace std;

class threadPool {

private:
    queue<packaged_task<void(void)>> tasks; // by packaged task we can get a future
    vector<future<void>> vf; // future of the tasks
    vector<thread> threads; // vector of threads (nm. of threads = size)
    mutex m;
    condition_variable c;
    int size;
    bool stop;

    void body() {
        while(true) {
            packaged_task<void(void)> tsk; //initialization
            {
                unique_lock<mutex> l(this->m);
                this->c.wait(l, [&](){ return (!this->tasks.empty() || this->stop); }); // wait until queue is not empty or the threadpool is stopped
                if(!this->tasks.empty()){
                    tsk = move(this->tasks.front());
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
        // i task vengono assegnati al thread pool nel costruttore affinché siano eseguibili appena entrano nella coda
        for(int worker=0; worker<this->size; worker++)
            this->threads.emplace_back([this](){ this->body(); });
    }

    ~threadPool() {
        //in this way i'm sure that all the result has been computed before that stop is setted to true
        for(int i=0;i<vf.size();i++)
            this->vf[i].get();

        stopThreadPool();

        for(auto& thread : this->threads)
            thread.join();
    }

    void submit(function<void(void)> fx) {
        {
            unique_lock<mutex> l(this->m);
            packaged_task<void(void)> pt(fx);
            this->vf.push_back(pt.get_future());
            this->tasks.push(move(pt));
        }
        this->c.notify_one();
    }
  
};
