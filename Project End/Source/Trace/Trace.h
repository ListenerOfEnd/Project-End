#pragma once

#include<string>
#include<iostream>
#include<fstream>

class Trace
{
public:

	// Get function.
	// Returns: An instance of the class.
	static Trace& GetInstance();

	// Initialization function.
	void Init();

	// Shutdown function.
	void Shutdown();

	// Write function.
	// Writes a string to the trace file.
	// Params:
	//	data - The data to write.
	void Write(std::string data);

	// Toggle function.
	// Toggles whether or not to display to the screen.
	void ToggleDisplay();

	// Check function.
	// Returns: Whether or not the display is toggled on.
	bool DisplayIsOn();

private:

	Trace(); // Default Constructor.

	// Disable Copy Constructor and Assignment Operator.
	Trace(const Trace&) = delete;
	Trace& operator=(const Trace&) = delete;

	// Display function.
	// Displays the Trace log to the screen.
	// Params:
	//	data - The data to display.
	void Display(std::string data);

	const std::string path = "./trace.log";
	std::ofstream output;
	bool displayToggle;
};
