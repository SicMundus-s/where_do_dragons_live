#pragma once
#ifndef GETRANDOMNUMBER_H
#define GETRANDOMNUMBER_H

#include <cstdlib>
int getRandomNumber(int min, int max)
{
	return static_cast<int>((rand() % (max - min + 1)) + min);
}

#endif#pragma once
