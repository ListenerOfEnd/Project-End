#include "AIManager.h"
#include "../Trace/Trace.h"


AIManager& AIManager::GetInstance()
{
	static AIManager instance;
	return instance;
}


void AIManager::Init()
{
	Trace::GetInstance().Write("AIManager::Init");
	ai = new AI();
}


void AIManager::Start()
{
	Trace::GetInstance().Write("AIManager::Start");

	current_state = AIState::Running;
	next_state = AIState::Running;
	
	ai->Load();
	ai->Init();
}


void AIManager::Update()
{
	Trace::GetInstance().Write("AIManager::Update");

	/* Update the states. */
	current_state = next_state;
	
	if (current_state == AIState::Running)
	{
		ai->Update();
	}
}


void AIManager::Stop()
{
	Trace::GetInstance().Write("AIManager::Stop");

	ai->Shutdown();
	ai->Unload();
}


void AIManager::Shutdown()
{
	Trace::GetInstance().Write("AIManager::Shutdown");

	delete ai;
	ai = nullptr;
}


bool AIManager::Running()
{
	Trace::GetInstance().Write("AIManager::Running");
	return (current_state == AIState::Running || current_state == AIState::Restarting) ? true : false;
}


void AIManager::SetNextState(AIState n)
{
	Trace::GetInstance().Write("AIManager::SetNextState");
	next_state = n;
}


AIState AIManager::GetCurrentState()
{
	Trace::GetInstance().Write("AIManager::GetCurrentState");
	return current_state;
}


AI& AIManager::GetAI() const
{
	return *ai;
}


AIManager::AIManager()
	: current_state(), next_state(), ai(nullptr)
{}
