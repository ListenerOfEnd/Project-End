#pragma once

#include "../Memory/Memory.h"
#include<vector>
#include<iostream>

#include<fstream>
#include<filesystem>

#include "../Trace/Trace.h"
#include<Windows.h>
#include<lmcons.h>

#include "../Phrase/Phrase.h"

class MemoryManager
{
public:

	// Get function.
	// Returns: An instance of the class.
	static MemoryManager& GetInstance();

	// Load function.
	void Load();

	// Initialization function.
	void Init();

	// Update function.
	void Update();

	// Shutdown function.
	void Shutdown();

	// Unload function.
	void Unload();

	// Store function.
	// Stores information from memories marked as modified.
	void StoreLearnedInformation();

	// Find function.
	// Finds a memory with a given topic.
	// Params:
	//	topic_ - The topic to find.
	// Returns: The unmodifiable memory.
	Memory& Find(std::string topic_) const;

	// Find function.
	// Finds a memory with a given ID.
	// Params:
	//	_ID - The id to find.
	// Returns: The unmodifiable memory.
	Memory& Find(UINT _ID) const;

	// Counter function.
	// Displays the number of memories out of all memories that have been modified.
	void CountModified();

	// Get function.
	// Gets a memory from the list.
	// Params:
	//	topic_ - The topic of the memory.
	// Returns: The memory of the corresponding topic.
	Memory& GetMemory(std::string topic_);

	// Get function.
	// Gets a memory from the list.
	// Params:
	//	_ID - The id of the memory.
	// Returns: The memory of the corresponding ID.
	Memory& GetMemory(UINT _ID);

	// Add function.
	// Adds a memory to the list.
	void Add(Memory* mem);

	// Add function.
	// Adds a memory to the list.
	void Add(UINT _ID, std::string topic_, MemoryType mT = MemoryType::INVALID, bool modded = true);

	// Remove function.
	// Removes a memory from the list.
	// Params:
	//	topic_ - The topic to search for.
	void Remove(std::string topic_);

	// Remove function.
	// Removes a memory from the list.
	// Params;
	//	_ID - The id to search for.
	void Remove(UINT _ID);

	// Remember function.
	// Goes through memories and finds out if the user input correlates to anything learned from the past.
	// Returns: Whether or not it remembered something.
	bool Remember();

private:

	MemoryManager(); // Default Constructor.
	
	// Disable Copy Constructor and Assignment Operator.
	MemoryManager(const MemoryManager&) = delete;
	MemoryManager& operator=(const MemoryManager&) = delete;

	// Parse function.
	// Parses out the text from mem files into working Memories.
	// Params:
	//	data - The data to parse.
	void Parse(std::string data);

	// Setup function.
	// Runs the first time setup.
	void FirstTimeSetup();

	std::vector<Memory*>* memoryList; // The list of memories.
	std::filesystem::path memoryPath;
};

