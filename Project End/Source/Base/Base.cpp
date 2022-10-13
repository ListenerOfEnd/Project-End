#include "Base.h"
#include "../Trace/Trace.h"
#include "../AIManager/AIManager.h"


Base& Base::GetInstance()
{
	static Base instance;
	return instance;
}


void Base::Init()
{
	Trace::GetInstance().Write("Base::Init");
	AIManager::GetInstance().Init();
	AIManager::GetInstance().Start();
}


void Base::Update()
{
	Trace::GetInstance().Write("Base::Update");
	AIManager::GetInstance().Update();
}


void Base::Shutdown()
{
	Trace::GetInstance().Write("Base::Shutdown");
	AIManager::GetInstance().Stop();
	AIManager::GetInstance().Shutdown();
}


Base::Base()
{}
