#pragma once

struct BCD
{
private:
	static int num;
public:
	static void setBCD(unsigned char c);
	static unsigned char getBCD(int pos);
};