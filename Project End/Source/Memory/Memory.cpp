#include "Memory.h"


Memory::Memory()
	: ID(), modified(), topic(), memType()
{}


Memory::Memory(const Memory& other)
	: ID(other.ID), modified(other.modified), topic(other.topic), memType(other.memType)
{}


Memory::Memory(unsigned int _ID, bool modded, std::string _topic, MemoryType mT)
	: ID(_ID), modified(modded), topic(_topic), memType(mT)
{}


Memory::~Memory()
{
	ID = NULL;
	modified = NULL;
	topic.clear();

	memType = MemoryType::INVALID;
}


unsigned int Memory::GetID()
{
	Trace::GetInstance().Write("Memory::GetID");
	return ID;
}


void Memory::SetID(unsigned int id)
{
	Trace::GetInstance().Write("Memory::SetID");
	ID = id;
}


bool Memory::Modified()
{
	Trace::GetInstance().Write("Memory::Modified");
	return modified;
}


void Memory::SetModified(bool flag)
{
	Trace::GetInstance().Write("Memory::SetModified");
	modified = flag;
}


std::string Memory::GetTopic()
{
	Trace::GetInstance().Write("Memory::GetTopic");
	return topic;
}


void Memory::SetTopic(std::string _topic)
{
	Trace::GetInstance().Write("Memory::SetTopic");
	topic = _topic;
}


MemoryType Memory::GetMemoryType()
{
	Trace::GetInstance().Write("Memory::GetMemoryType");
	return memType;
}


void Memory::SetMemoryType(MemoryType mT)
{
	Trace::GetInstance().Write("Memory::SetMemoryType");
	memType = mT;
}
