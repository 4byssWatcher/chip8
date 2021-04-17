#include "utils.h"

int BCD::num = 0;

void BCD::setBCD(unsigned char c)
{
	num = c;
}

unsigned char BCD::getBCD(int pos)
{
	int res = num;
	while (--pos)
		res /= 10;
	return res % 10;
}