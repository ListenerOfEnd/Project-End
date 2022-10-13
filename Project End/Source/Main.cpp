#include "./Trace/Trace.h"
#include "./Base/Base.h"
#include "./AIManager/AIManager.h"


int main()
{
	Trace::GetInstance().Init();
	Trace::GetInstance().Write("### PROGRAM START ###");
	
	Base::GetInstance().Init();

	while (AIManager::GetInstance().Running())
	{
		if (AIManager::GetInstance().GetCurrentState() == AIState::Restarting)
		{
			AIManager::GetInstance().Stop();
			AIManager::GetInstance().Start();
		}

		Base::GetInstance().Update();
	}

	Base::GetInstance().Shutdown();

	Trace::GetInstance().Write("### PROGRAM STOP ###");
	Trace::GetInstance().Shutdown();

	return 0;
}
