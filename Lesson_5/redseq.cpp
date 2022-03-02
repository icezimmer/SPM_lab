#include <iostream>
#include <vector>
#include <numeric>

#include <chrono>
void active_delay(int msecs) {
	// add delay of msecs time
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

#include "utimer.cpp"

int main(int argc, char * argv[]) {
	int n = atoi(argv[1]); // vector lenght
	int seed = atoi(argv[2]); // seed
	srand(seed);

	std::vector<int> v(n);
	for(int i=0; i<n; i++) {
		v[i] = rand() % 8;
		std::cout << v[i] << " ";
  }
	std::cout << std::endl; // endline

  {
		utimer u("reduce"); // compute the elapsed time of this computation block
    auto oplus = [](const int& x, const int& y) {
			// variables with & are accessible by reference
			// const specify that the variable is not modifiable
			auto r = x + y;
			active_delay(1); // add a delay of 1 msec
			return(r);
		};

		//accumulate the operation oplus from the begin to the end of the vector
		auto res = std::accumulate(v.begin(), v.end(), 0, oplus);
		std::cout << res << std::endl;
  }
	return(0);
}
