#include "eval-perf.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

uint64_t rdtsc(){
  unsigned int lo, hi;
  __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
  return ((uint64_t)hi << 32) | lo;
}

EvalPerf::EvalPerf() {
}

void EvalPerf::start(){
  this->debutSec = std::chrono::system_clock::now();
  this->debutMili = std::chrono::high_resolution_clock::now();
  this->debutCycles=rdtsc();
  
}

void EvalPerf::stop(){
  this->finCycles=rdtsc();
  this->finSec = std::chrono::system_clock::now();
  this->finMili = std::chrono::high_resolution_clock::now();
}

std::chrono::duration<double> EvalPerf::second(){
  std::chrono::time_point<std::chrono::system_clock> ds=this->debutSec;
  std::chrono::time_point<std::chrono::system_clock> fs=this->finSec;
  return fs-ds;
}


std::chrono::duration<double> EvalPerf::milisecond(){
  std::chrono::time_point<std::chrono::high_resolution_clock> dm=this->debutMili;
  std::chrono::time_point<std::chrono::high_resolution_clock> fm=this->finMili;
  return (fm-dm)*1000;
}

uint64_t EvalPerf::nb_cycles(){
  uint64_t fc=this->finCycles;
  uint64_t dc=this->debutCycles;
  return fc-dc;
}

float EvalPerf::CPI(int n){
  return this->nb_cycles()*1.0/n;
}
