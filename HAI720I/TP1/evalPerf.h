#include <chrono>
#include <x86intrin.h>

class EvalPerf{
	public:
		std::chrono::time_point<std::chrono::system_clock> startTime;
		std::chrono::time_point<std::chrono::system_clock> endTime;
		int nbCyclesAtStart;
		int nbCyclesAtEnd;

	public:
	void start();
	void stop();
	int nbCycles();
	float second();
	int millisecond();
};