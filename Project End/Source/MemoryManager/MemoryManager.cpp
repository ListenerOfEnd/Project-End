#include "MemoryManager.h"
#include "../ConversationManager/ConversationManager.h"
#include "../Random/Random.h"



MemoryManager& MemoryManager::GetInstance()
{
	static MemoryManager instance;
	return instance;
}


void MemoryManager::Load()
{
	Trace::GetInstance().Write("MemoryManager::Load");

	FirstTimeSetup();

	memoryList = new std::vector<Memory*>();

	for (std::filesystem::directory_entry p : std::filesystem::directory_iterator(memoryPath))
	{
		// Open up the current file.
		std::ifstream file;
		file.open(p.path().string(), std::ifstream::in);
		

		if (file.good())
		{
			std::string data;
			file >> data; // Pipe the data from the file into the variable.
			Parse(data);

			file.close(); // Close the file for the next file.
		}
	}
}


void MemoryManager::Init()
{
	Trace::GetInstance().Write("MemoryManager::Init");

	// Go through the memory list and make sure there are none without an ID, or are nullptr.
	// Also set all the modifed flags to false.
	for (UINT i = 0; i < memoryList->size(); ++i)
	{
		if (memoryList->at(i) == nullptr)
		{
			memoryList->erase(memoryList->begin() + i);
			--i;
		}

		if (memoryList->at(i)->GetID() == NULL)
			memoryList->at(i)->SetID(i);

		memoryList->at(i)->SetModified(false);
	}
}


void MemoryManager::Update()
{
	Trace::GetInstance().Write("MemoryManager::Update");

	/* This is a glorifed integrity check for now. */
	for (UINT i = 0; i < memoryList->size(); ++i)
	{
		if (memoryList->at(i) == nullptr)
		{
			memoryList->erase(memoryList->begin() + i);
			--i;
		}
	}

}


void MemoryManager::Shutdown()
{
	Trace::GetInstance().Write("MemoryManager::Shutdown");
	StoreLearnedInformation();
}


void MemoryManager::Unload()
{
	Trace::GetInstance().Write("MemoryManager::Unload");

	memoryList->clear();
	delete memoryList;
	memoryList = nullptr;
}


void MemoryManager::StoreLearnedInformation()
{
	Trace::GetInstance().Write("MemoryManager::StoreLearnedInformation");

	// Go through the list and write to the corresponding files from the ID's of the modified memories.
	for (UINT i = 0; i < memoryList->size(); ++i)
	{
		if (memoryList->at(i)->Modified()) // The current memory has been modifed. Store it.
		{
			std::string path = "./Memories/" + memoryList->at(i)->GetID();
			path.append(".MEM");

			// Open the file.
			std::ofstream file;
			file.open(path, std::ofstream::out | std::ofstream::trunc); // Truncate it.

			if (file.good())
			{
				// Write to the file (read Notes.txt to find out how these are formatted).

				std::string data;
				
				data.push_back(memoryList->at(i)->GetID());
				data.push_back('|');
				data.append(memoryList->at(i)->GetTopic());

				data.push_back('|');
				data.push_back(memoryList->at(i)->GetMemoryType());

				file << data;
				file.close();
			}
		}
	}
}


Memory& MemoryManager::Find(std::string topic_) const
{
	Trace::GetInstance().Write("MemoryManager::Find(topic)");

	for (UINT i = 0; i < memoryList->size(); ++i)
	{
		if (memoryList->at(i)->GetTopic() == topic_)
		{
			return *memoryList->at(i);
		}
	}

	// Create a new memory with the last memory's ID + 1 as the ID
	// and the topic given as the topic. MemoryType will be INVALID.
	Memory* mem = new Memory(memoryList->at(memoryList->size())->GetID() + 1, true, topic_, MemoryType::INVALID);
	memoryList->push_back(mem);
	return *memoryList->at(memoryList->size()); // This is the one we just added.
}


Memory& MemoryManager::Find(UINT _ID) const
{
	Trace::GetInstance().Write("MemoryManager::Find(ID)");

	for (UINT i = 0; i < memoryList->size(); ++i)
	{
		if (memoryList->at(i)->GetID() == _ID)
			return *memoryList->at(i);
	}

	memoryList->push_back(nullptr); // Integrity check will deal with this next update.
	return *memoryList->at(memoryList->size());
}


void MemoryManager::CountModified()
{
	Trace::GetInstance().Write("MemoryManager::CountModified");

	int modified = 0;
	for (UINT i = 0; i < memoryList->size(); ++i)
	{
		if (memoryList->at(i)->Modified())
			modified++;
	}

	std::cout << "\n" << modified << "/" << memoryList->size() << " memories are modified." << std::endl;
}


Memory& MemoryManager::GetMemory(std::string topic_)
{
	Trace::GetInstance().Write("MemoryManager::GetMemory(topic)");

	for (UINT i = 0; i < memoryList->size(); ++i)
	{
		if (memoryList->at(i)->GetTopic() == topic_)
		{
			return *memoryList->at(i);
		}
	}

	// Create a new memory with the last memory's ID + 1 as the ID
	// and the topic given as the topic. MemoryType will be INVALID.
	Add(new Memory(memoryList->at(memoryList->size())->GetID() + 1, true, topic_, MemoryType::INVALID));
	return *memoryList->at(memoryList->size()); // This is the one we just added.
}


Memory& MemoryManager::GetMemory(UINT _ID)
{
	Trace::GetInstance().Write("MemoryManager::GetMemory(ID)");

	for (UINT i = 0; i < memoryList->size(); ++i)
	{
		if (memoryList->at(i)->GetID() == _ID)
			return *memoryList->at(i);
	}

	memoryList->push_back(nullptr); // Integrity check will deal with this next update.
	return *memoryList->at(memoryList->size());
}


void MemoryManager::Add(Memory* mem)
{
	Trace::GetInstance().Write("MemoryManager::Add(Memory)");

	// Check if this memory has the same ID as any other memory in the list.
	for (UINT i = 0; i < memoryList->size(); ++i)
	{
		if (memoryList->at(i)->GetID() == mem->GetID())
		{
			// Push the memory to the back of the list and reset the ID.
			mem->SetID(memoryList->size() + 1);
			memoryList->push_back(mem);

			return;
		}
	}

	memoryList->push_back(mem);
}


void MemoryManager::Add(UINT _ID, std::string topic_, MemoryType mT, bool modded)
{
	Trace::GetInstance().Write("MemoryManager::Add(Incomplete Memory)");

	Memory* mem = new Memory(_ID, modded, topic_, mT);

	// Check if this memory has the same ID as any other memory in the list.
	for (UINT i = 0; i < memoryList->size(); ++i)
	{
		if (memoryList->at(i)->GetID() == mem->GetID())
		{
			// Push the memory to the back of the list and reset the ID.
			mem->SetID(memoryList->size() + 1);
			memoryList->push_back(mem);

			return;
		}
	}

	memoryList->push_back(mem);
}


void MemoryManager::Remove(std::string topic_)
{
	Trace::GetInstance().Write("MemoryManager::Rmove(topic)");

	// Go through the list and remove the memory with the given topic if it exists.
	/* This means we also have to remove the file from the memory directory. */
	
	// Grab the ID from the given topic.
	std::string ID;
	for (UINT i = 0; i < memoryList->size(); ++i)
	{
		if (memoryList->at(i)->GetTopic() == topic_)
		{
			ID = memoryList->at(i)->GetID();

			// Remove it from the list.
			memoryList->erase(memoryList->begin() + i);
			break;
		}
	}

	if (!ID.empty())
	{
		ID.append(".MEM");

		for (std::filesystem::directory_entry p : std::filesystem::directory_iterator(memoryPath))
		{
			if (p.path().filename().string() == ID) // Look for the file.
			{
				std::filesystem::remove(p.path().filename());
				break;
			}
		}
	}
}


void MemoryManager::Remove(UINT _ID)
{
	Trace::GetInstance().Write("MemoryManager::Remove(ID)");

	bool exists = false;
	for (UINT i = 0; i < memoryList->size(); ++i)
	{
		if (memoryList->at(i)->GetID() == _ID)
		{
			memoryList->erase(memoryList->begin() + i);
			exists = true;
			break;
		}
	}

	if (exists)
	{
		std::string ID;
		ID = _ID;
		ID.append(".MEM");

		for (std::filesystem::directory_entry p : std::filesystem::directory_iterator(memoryPath))
		{
			if (p.path().filename().string() == ID) // Look for the file.
			{
				std::filesystem::remove(p.path().filename());
				break;
			}
		}
	}
}


bool MemoryManager::Remember()
{
	UINT index = 0;
	for (; index < memoryList->size(); ++index)
	{
		if (ConversationManager::GetInstance().GetUserInput().find(memoryList->at(index)->GetTopic()) !=
			ConversationManager::GetInstance().GetUserInput().npos)
		{
			/* we found it. */

			std::string feeling;
			switch (memoryList->at(index)->GetMemoryType())
			{
			case 1:
				feeling = "happy";
				break;

			case 2:
				feeling = "sad";
				break;

			case 3:
				feeling = "angry";
				break;

			default:
				break;
			}

			std::string out = "Does that have anything to do with ";
			out.append(memoryList->at(index)->GetTopic());
			out.append("? It's a ");

			out.append(feeling);
			out.append(" memory for me.");

			std::cout << out << std::endl;

			ConversationManager::GetInstance().AddToConversation(out);
			ConversationManager::GetInstance().AddToKnownPhrases(out);

			// Ask the user if that was a good response.
			std::cout << "\nWas that a good response? (y/n): ";
			std::string choice;
			std::cin >> choice;

			if (choice == "y")
			{
				ConversationManager::GetInstance().AddResponse(ConversationManager::GetInstance().GetUserInput(), out);
			}

			return true;
		}
	}

	return false;
}


MemoryManager::MemoryManager()
	: memoryList(), memoryPath()
{}


void MemoryManager::Parse(std::string data)
{
	Trace::GetInstance().Write("MemoryManager::Parse");

	/* Refer to Notes.txt to figure out how the MEM files are formatted. */

	Memory* mem = new Memory();
	mem->SetModified(false); // Set this from the beginning.

	std::string id;
	while (data.at(0) != '|')
	{
		id.push_back(data.at(0));
		data.erase(data.begin());
	}

	mem->SetID(atoi(id.c_str()));

	if (data.at(0) == '|')
		data.erase(data.begin());

	std::string topic_;
	while (data.at(0) != '|')
	{
		topic_.push_back(data.at(0));
		data.erase(data.begin());
	}

	mem->SetTopic(topic_);

	if (data.at(0) == '|')
		data.erase(data.begin());

	std::string mT;
	while (data.at(0) != '|')
	{
		mT.push_back(data.at(0));
		data.erase(data.begin());
	}

	mem->SetMemoryType((MemoryType)atoi(mT.c_str()));

	if (data.at(0) == '|')
		data.erase(data.begin());

	Add(mem);
}

void MemoryManager::FirstTimeSetup()
{
	Trace::GetInstance().Write("MemoryManager::FirstTimeSetup");

	// Check to see if the file/folder is created.
	/* C:/Users/[username]/Documents/Memories/ */

	// Grab the username.
	char username[UNLEN + 1];
	DWORD size = UNLEN + 1;
	GetUserNameA(username, &size);

	std::string un(username);
	std::string pathToCheck = "C:/Users/";

	pathToCheck.append(un);
	pathToCheck.append("/Documents/Memories/");

	if (!std::filesystem::exists(pathToCheck))
	{
		// Create the directory.
		if (std::filesystem::create_directory(pathToCheck))
		{
			std::cout << "First time setup performed correctly." << std::endl;
			memoryPath = pathToCheck;
			return;
		}

		else
		{
			std::cout << "Setup failed. Creating memory files in documents.";
			memoryPath = "C:/Users/" + un + "/Documents";
		}
	}

	memoryPath = pathToCheck;
}
