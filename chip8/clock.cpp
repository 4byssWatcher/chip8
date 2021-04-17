#include "clock.h"

#include <chrono>
#include <ctime>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

void Clock::init()
{
	lastTimer = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

int Clock::getCycles(int frequency)
{
	long long nextTimer = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	int cycles = (nextTimer - lastTimer) * frequency / 1000;
	if (cycles > 0)lastTimer = nextTimer;
	return cycles;
}