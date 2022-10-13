#include "Random.h"
#include<ctime>
#include<cstdlib>


Random& Random::GetInstance()
{
	static Random instance;
	return instance;
}


void Random::Seed()
{
	srand(static_cast<unsigned int>(time(0)));
}


LL Random::Generate(LL MIN, LL MAX)
{
	return rand() % MAX + MIN;
}


Random::Random()
{}
