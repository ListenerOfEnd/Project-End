#include "Trace.h"


Trace& Trace::GetInstance()
{
	static Trace instance;
	return instance;
}


void Trace::Init()
{
	// Open up the file.
	output.open(path, std::ofstream::out);
	if (output.good())
	{
		return;
	}

	std::cout << "Couldn't open trace file." << std::endl;
}


void Trace::Shutdown()
{
	// Close the file.
	if (output.is_open())
	{
		output.close();
	}
}


void Trace::Write(std::string data)
{
	if (output.good())
		output << data + '\n';

	if (displayToggle)
		Display(data);
}


void Trace::ToggleDisplay()
{
	displayToggle = !displayToggle;
}


bool Trace::DisplayIsOn()
{
	return displayToggle;
}


Trace::Trace()
	: displayToggle(false)
{}


void Trace::Display(std::string data)
{
	std::cout << "\n" << data << std::endl;
}
