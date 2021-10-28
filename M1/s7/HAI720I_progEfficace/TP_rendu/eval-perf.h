#include <string>
#include <iostream>
#include <chrono>
#include <x86intrin.h>



class EvalPerf{
 private:
  
  std::chrono::time_point<std::chrono::system_clock> debutSec;
  std::chrono::time_point<std::chrono::system_clock> finSec;
  std::chrono::time_point<std::chrono::high_resolution_clock> debutMili;
  std::chrono::time_point<std::chrono::high_resolution_clock> finMili;
  uint64_t debutCycles;
  uint64_t finCycles;
  
 public:

  EvalPerf();
  void start();
  void stop();
  std::chrono::duration<double> second();
  std::chrono::duration<double> milisecond();
  uint64_t nb_cycles();
  float CPI(int n);
  
};
