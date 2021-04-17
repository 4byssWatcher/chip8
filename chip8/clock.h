#pragma once

struct Clock
{
public:
	void init();

	Clock() { init(); }

	int getCycles(int frequency);

private:
	long long lastTimer;
};