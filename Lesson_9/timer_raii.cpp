#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <thread>


class timer_raii {
  std::chrono::system_clock::time_point start;
  std::chrono::system_clock::time_point stop;
  std::string message; 
  using usecs = std::chrono::microseconds;
  using msecs = std::chrono::milliseconds;
 
public:

  timer_raii(const std::string m) : message(m) {
    start = std::chrono::system_clock::now();
  }
    
  ~timer_raii() {
    stop = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed = stop - start;
    auto musec = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    
    std::cerr << message << " computed in " << musec << " usec " << std::endl;
  }
};
