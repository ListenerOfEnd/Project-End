#pragma once

#include<string>
#include "../Trace/Trace.h"

enum MemoryType
{
	INVALID = 0,
	Happy,
	Sad,
	Angry,

	NUM_MEMORY_TYPES
};

class Memory
{
public:

	Memory(); // Default Constructor.
	Memory(const Memory& other); // Copy Constructor.
	Memory(unsigned int _ID, bool modded, std::string _topic, MemoryType mT); // Non-Default Constructor.

	~Memory(); // Destructor.

	// Get function.
	// Returns: The ID of the memory.
	unsigned int GetID();

	// Set function.
	// Sets the ID of the memory.
	// Params:
	//	id - The ID to set.
	void SetID(unsigned int id);

	// Get function.
	// Returns: Whether or not the memory has been modified.
	bool Modified();

	// Set function.
	// Sets the moddified flag.
	// Params:
	//	flag - The flag to set.
	void SetModified(bool flag);

	// Get function.
	// Returns: The topic of the memory.
	std::string GetTopic();

	// Set function.
	// Sets the topic of the memory.
	// Params:
	//	_topic - The topic to set.
	void SetTopic(std::string _topic);

	// Get function.
	// Returns: The type of memory.
	MemoryType GetMemoryType();

	// Set function.
	// Sets the memory type.
	// Params:
	//	mT - The memory type to set.
	void SetMemoryType(MemoryType mT);

private:

	unsigned int ID; // The ID of the memory. Acts as the filename.
	bool modified; // The flag to tell the program to write the data to the corresponding file.

	std::string topic; // The topic the memory is based around.
	MemoryType memType; // The type of memory.
};
