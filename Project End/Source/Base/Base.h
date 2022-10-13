#pragma once

class Base
{
public:

	// Get function.
	// Returns: An instance of the class.
	static Base& GetInstance();

	// Initialization function.
	void Init();

	// Update function.
	void Update();

	// Shutdown function.
	void Shutdown();

private:

	Base(); // Default Constructor.

	// Disable Copy Constructor and Assignment Operator.
	Base(const Base&) = delete;
	Base& operator=(const Base&) = delete;
};
