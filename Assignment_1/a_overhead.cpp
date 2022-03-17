# include <iostream>
# include <vector>
# include <vector>
# include <future>
# include <chrono>
# include <utility>
# include <functional>

#define START(timename) auto timename = std::chrono::system_clock::now();
#define STOP(timename,elapsed)  auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - timename).count();

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

auto compute_async(function<void(int)> fun, int nw) {
    START(start);
    vector<shared_future<void>> atids(nw);
    for(int i=0; i<nw; i++)
        atids[i] = async(launch::async, fun, i);
    for(int i=0; i<nw; i++)
        try {
	        atids[i].get();
        } catch (const std::exception& e) {
	        cout << "Got " << e.what() << " i= " << i << endl;
        }
    STOP(start, elapsed_time);
    return elapsed_time;
}

int main(int argc, char **argv) {
  
    if(argc < 3) {
        cout << "Usage is: " << argv[0] << " n-test n-worker " << endl;
        return(0);
    }

    // lambda function
    auto fun = [] (int j) {
        return;
    };

    int nt = atoi(argv[1]);
    int nw = atoi(argv[2]);

    float mean_time = 0;

    for(int i=0; i<nt; i++) 
        mean_time += compute_async(fun, nw) / nt;

    cout <<"Overhead (mean value time): " << mean_time << " musec" << endl;

    return 0;
}