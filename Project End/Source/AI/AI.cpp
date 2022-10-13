#include "AI.h"
#include "../Trace/Trace.h"
#include<Windows.h>

#include "../GlobalDefines.hpp"
#include "../AIManager/AIManager.h"
#include "../MemoryManager/MemoryManager.h"

#include "../ConversationManager/ConversationManager.h"


AI::AI()
	: displayCounter()
{}


AI::~AI()
{
	displayCounter = NULL;
}


void AI::Load()
{
	Trace::GetInstance().Write("AI::Load");
	MemoryManager::GetInstance().Load();
	ConversationManager::GetInstance().Load();
}


void AI::Init()
{
	Trace::GetInstance().Write("AI::Init");
	displayCounter = 0;
	MemoryManager::GetInstance().Init();

	ConversationManager::GetInstance().Init();

	if (!Trace::GetInstance().DisplayIsOn())
		ExecuteCommand(9); // Clears the screen.
}


void AI::Update()
{
	Trace::GetInstance().Write("AI::Update");
	
	if (ConversationManager::GetInstance().GrabInput())
	{
		// Check if the userInput contains any commands.
		int i = NULL;
		if (IsCommand(ConversationManager::GetInstance().GetUserInput(), i))
		{
			ExecuteCommand(i);
		}

		else
		{
			MemoryManager::GetInstance().Update();

			/* TODO: Make this work. */
			// if (!MemoryManager::GetInstance().Remember()) // Experimental for now.
				ConversationManager::GetInstance().Update();
		}
	}
}


void AI::Shutdown()
{
	Trace::GetInstance().Write("AI::Shutdown");
	MemoryManager::GetInstance().Shutdown();
	ConversationManager::GetInstance().Shutdown();

	displayCounter = NULL;
}


void AI::Unload()
{
	Trace::GetInstance().Write("AI::Unload");
	MemoryManager::GetInstance().Unload();
	ConversationManager::GetInstance().Unload();
}


bool AI::IsCommand(std::string input, int& index)
{
	for (unsigned int i = 0; i < command_list.size(); ++i)
	{
		if (command_list.at(i) == ConversationManager::GetInstance().GetUserInput())
		{
			index = i;
			return true;
		}
	}

	return false;
}


void AI::ExecuteCommand(unsigned int index)
{
	Trace::GetInstance().Write("AI::ExecuteCommand");

	std::string searchS;
	std::string sPhraseResponse;

	switch (index)
	{
	case 0: // _toggle_trace_display
		Trace::GetInstance().ToggleDisplay();
		break;

	case 1: // _write_new_data
		MemoryManager::GetInstance().StoreLearnedInformation();
		break;

	case 2: // _restart
		AIManager::GetInstance().SetNextState(AIState::Restarting);
		break;

	case 3: // _stop
		AIManager::GetInstance().SetNextState(AIState::Stopping);
		break;

	case 4: // _count_modified
		MemoryManager::GetInstance().CountModified();
		break;

	case 5: // _commands
		std::cout << "\nCommands are as follows: " << std::endl;
		for (UINT i = 0; i < command_list.size(); ++i)
			std::cout << command_list.at(i) << std::endl;

		std::cout << std::endl;
		break;

	case 6: // _recount_conversation
		ConversationManager::GetInstance().RecountConversation();
		break;

	case 7: // _erase
		std::cout << "\nEnter phrase to search in with spaces as underscores: ";
		std::cin >> searchS;

		// Replace the underscores with spaces.
		for (unsigned int i = 0; i < searchS.size(); ++i)
			if (searchS.at(i) == '_')
				searchS.at(i) = ' ';

		// Go through the kp list and find the phrase.
		for (unsigned int i = 0; i < ConversationManager::GetInstance().GetKnownPhrases().size(); ++i)
		{
			if (ConversationManager::GetInstance().GetKnownPhrases().at(i)->GetPhrase() == searchS)
			{
				// We found the phrase.
				// Ask the user for the response to erase.
				std::string searchR;
				std::cout << "\nEnter response to search for with spaces as underscores: ";
				std::cin >> searchR;

				for (unsigned int j = 0; j < searchR.size(); ++j)
					if (searchR.at(j) == '_')
						searchR.at(j) = ' ';

				// Check for the response in the response list.
				/* Jesus christ this is a mess. */
				for (unsigned int h = 0; h < ConversationManager::GetInstance().GetKnownPhrases().size(); ++h)
				{
					for (unsigned int g = 0; g < ConversationManager::GetInstance().GetKnownPhrases().at(h)->GetResponseList().size(); ++g)
					{
						if (ConversationManager::GetInstance().GetKnownPhrases().at(h)->GetResponseList().at(g)->GetResponse() == searchR)
						{
							// Erase the found response.
							ConversationManager::GetInstance().GetKnownPhrases().at(h)->GetResponseList().erase(
								ConversationManager::GetInstance().GetKnownPhrases().at(h)->GetResponseList().begin() + g);
						}
					}
				}
			}
		}
		break;

	case 8: // _show_responses
		// Ask the user which phrase to show the responses of.
		std::cout << "\nEnter the phrase to look for, replace spaces with underscores: ";
		std::cin >> sPhraseResponse;

		// Replace the underscores with spaces.
		for (unsigned int i = 0; i < sPhraseResponse.size(); ++i)
			if (sPhraseResponse.at(i) == '_')
				sPhraseResponse.at(i) = ' ';

		for (unsigned int i = 0; i < ConversationManager::GetInstance().GetKnownPhrases().size(); ++i)
		{
			if (ConversationManager::GetInstance().GetKnownPhrases().at(i)->GetPhrase() == sPhraseResponse)
			{
				// Display the response list.
				std::cout << "\n\nResponse list for '" << sPhraseResponse << "': ";
				for (unsigned int j = 0; j < ConversationManager::GetInstance().GetKnownPhrases().at(i)->GetResponseList().size(); ++j)
				{
					std::cout << "\n" << ConversationManager::GetInstance().GetKnownPhrases().at(i)->GetResponseList().at(j)->GetResponse();
				}

				break;
			}
		}

		std::cout << std::endl;
		break;

	case 9: // _cls
		system("cls");
		break;

	default:
		break;
	}
}
