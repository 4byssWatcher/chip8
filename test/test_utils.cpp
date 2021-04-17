#include "pch.h"
#include "../chip8/utils.h"

TEST(Utils, BCD1)
{
	BCD::setBCD(125);
	EXPECT_EQ(BCD::getBCD(3), 1);
	EXPECT_EQ(BCD::getBCD(2), 2);
	EXPECT_EQ(BCD::getBCD(1), 5);
}

TEST(Utils, BCD2)
{
	BCD::setBCD(1);
	EXPECT_EQ(BCD::getBCD(3), 0);
	EXPECT_EQ(BCD::getBCD(2), 0);
	EXPECT_EQ(BCD::getBCD(1), 1);
}

TEST(Utils, BCD3)
{
	BCD::setBCD(98);
	EXPECT_EQ(BCD::getBCD(3), 0);
	EXPECT_EQ(BCD::getBCD(2), 9);
	EXPECT_EQ(BCD::getBCD(1), 8);
}
