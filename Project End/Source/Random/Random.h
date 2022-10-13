#pragma once

#include "../GlobalDefines.hpp"

class Random
{
public:

	// Get function.
	// Returns: An instance of the class.
	static Random& GetInstance();

	// Seed function.
	// Seeds the random number generator.
	// NOTE: CALL THIS FUNCTION AT LEAST ONCE BEFORE ANY USE OF GENERATION.
	void Seed();

	// Generate function.
	// Generates a random number between MIN and MAX.
	// Params:
	//	MIN - The minimum bound.
	//	MAX - The maximum bound.
	// Returns: The generated number.
	LL Generate(LL MIN, LL MAX);

private:

	Random(); // Default Constructor.

	// Disable Copy Constructor and Assignment Operator.
	Random(const Random&) = delete;
	Random& operator=(const Random&) = delete;
};
