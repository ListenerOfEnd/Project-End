#include "ConversationManager.h"
#include "../Random/Random.h"
#include "../Trace/Trace.h"

#include<filesystem>
#include<Windows.h>
#include "../AI/AI.h"
#include "../AIManager/AIManager.h"


ConversationManager& ConversationManager::GetInstance()
{
	static ConversationManager instance;
	return instance;
}

void ConversationManager::Load()
{
	Trace::GetInstance().Write("ConversationManager::Load");

	/* Allocate the memory for the variables. */

	conversation = new std::vector<std::string>();
	known_phrases = new std::vector<Phrase*>();

	// We need to grab the known phrases from the file.

	std::fstream kpFile;
	kpFile.open(kp_path, std::fstream::in | std::fstream::out);

	std::string data;

	// Check if the file is open.
	if (kpFile.good())
	{
		// Iterate through the file, grabbing each line.
		while (std::getline(kpFile, data))
		{
			Parse(data); // Parse out the data.
		}
	}
}


void ConversationManager::Init()
{
	Trace::GetInstance().Write("ConversationManager::Init");

	// Make sure the conversation is empty.
	if (!conversation->empty())
	{
		conversation->clear();
	}

	// Make sure the user input is clear.
	if (!userInput.empty())
	{
		userInput.clear();
	}
}


void ConversationManager::Update()
{
	Trace::GetInstance().Write("ConversationManager::Update");

	/* This function only activates if the user input is complete, so we can assume we need to respond. */

	Random::GetInstance().Seed();

	// First, check if we know what the user said.
	bool known = false;
	for (UINT i = 0; i < known_phrases->size(); ++i)
	{
		if (userInput == known_phrases->at(i)->GetPhrase())
		{
			known = true;

			// Choose whether to respond with a known response or expand vocabulary.
			// Any value above 0 is expand. Any value below 0 is respond.
			LL val = Random::GetInstance().Generate(-10, 25);

			if (val < 0)
			{
				ExpandVocab();
				break;
			}

			else
			{
				Respond();
				break;
			}
		}
	}

	if (!known)
	{
		// Add it as a new phrase.
		Phrase* phrase = new Phrase(userInput, new std::vector<Response*>(), true);
		known_phrases->push_back(phrase);

		// Respond.
		Respond();
	}
}


void ConversationManager::Shutdown()
{
	Trace::GetInstance().Write("ConversationManager::Shutdown");

	userInput.clear();
	conversation->clear();

	std::fstream kpFile;
	kpFile.open(kp_path, std::fstream::in | std::fstream::out);

	// Put all the new/modified phrases into the file.
	if (kpFile.good())
	{
		for (UINT i = 0; i < known_phrases->size(); ++i)
		{
			std::string data;
			if (!data.empty())
				data.clear();

			data = known_phrases->at(i)->GetPhrase();
			data.append("|");

			for (UINT j = 0; j < known_phrases->at(i)->GetResponseList().size(); ++j)
			{
				data.append(known_phrases->at(i)->GetResponseList().at(j)->GetResponse());
				data.append(":");
			}

			data.append(">\n"); // Signals EOL;
			kpFile.write(data.c_str(), data.size());
		}
	}

	kpFile.close();

	known_phrases->clear();
}


void ConversationManager::Unload()
{
	Trace::GetInstance().Write("ConversationManager::Unload");

	delete known_phrases;
	delete conversation;

	known_phrases = nullptr;
	conversation = nullptr;
}


void ConversationManager::RecountConversation()
{
	Trace::GetInstance().Write("ConversationManager::RecountXConversation");

	std::cout << "\n### CONVERSATION START ###";
	for (UINT i = 0; i < conversation->size(); ++i)
	{
		if (i % 2 == 0)
			std::cout << "\nUSER: ";

		else
			std::cout << "\nAI: ";

		std::cout << conversation->at(i);
	}
	std::cout << "\n### CONVERSATION CURRENT ###\n" << std::endl;
}


bool ConversationManager::GrabInput()
{
	Trace::GetInstance().Write("ConversationManager::GrabInput");

	std::string input;
	userInput.clear(); // Gotta clear the stream.

	std::cout << "You: ";

	int c = NULL;
	int counter = 0;
	while (c != ENTER)
	{
		c = getchar();

		if (c == '\n' && counter == 0)
		{
			c = getchar();
			counter++;
		}


		if (c == BACKSPACE)
			input.pop_back();

		else
			input.push_back((char)c);
	}

	// Remove the last character (\n) from the userInput.
	input.pop_back();

	userInput = input;

	// Add the user input to the conversation if it's not a command.
	int index = NULL;
	if (!AIManager::GetInstance().GetAI().IsCommand(userInput, index))
	{
		conversation->push_back(userInput);

		// Put the user's input as a response to the last thing the AI said.
		AddResponse(previousOutput, userInput);
	}
	

	return true;
}


std::string ConversationManager::GetUserInput()
{
	Trace::GetInstance().Write("ConversationManager::GetUserInput");
	return userInput;
}


void ConversationManager::Respond()
{
	Trace::GetInstance().Write("ConversationManager::Respond");

	// Take a response from the phrase the user gave us.
	UINT index = 0;
	for (; index < known_phrases->size(); ++index)
	{
		if (known_phrases->at(index)->GetPhrase() == userInput)
		{
			break;
		}
	}

	if (known_phrases->at(index)->GetResponseList().empty()) // Response list is empty. Expand vocab.
	{
		ExpandVocab();
		return;
	}

	else
	{
		Random::GetInstance().Seed(); // Seed for the hell of it.

		std::string response = known_phrases->at(index)->GetResponseList().at(Random::GetInstance().Generate(0,
			known_phrases->at(index)->GetResponseList().size()))->GetResponse();

		// Add the response to the conversation.
		conversation->push_back(response);

		std::cout << "END: " << response << std::endl;

		previousOutput = response;
	}
}


void ConversationManager::ExpandVocab()
{
	Trace::GetInstance().Write("ConversationManager::ExpandVocab");

	/* This is essentially responding randomly. */

	Random::GetInstance().Seed();
	unsigned int val = Random::GetInstance().Generate(0, known_phrases->size());

	// Add the selected phrase to the conversation.
	Phrase* selected_phrase = known_phrases->at(val);
	conversation->push_back(selected_phrase->GetPhrase());

	// Push the response to the console.
	std::cout << "\nEND: " << selected_phrase->GetPhrase() << std::endl;

	// Ask the user whether or not it was a good response.
	std::string choice;
	std::cout << "\nWas that a good response? (y/n): ";
	std::cin >> choice;

	// If it was a good response...
	if (choice == "y")
	{
		// Add the selected phrase to the response list of what the user entered.
		for (UINT i = 0; i < known_phrases->size(); ++i)
		{
			if (userInput == known_phrases->at(i)->GetPhrase())
			{
				known_phrases->at(i)->Add(selected_phrase->GetPhrase());
				break;
			}
		}

		return;
	}
}

void ConversationManager::AddToConversation(std::string data)
{
	conversation->push_back(data);
}


void ConversationManager::AddToKnownPhrases(std::string data)
{
	// Check to see if the data is already in the kp list.
	for (UINT i = 0; i < known_phrases->size(); ++i)
		if (known_phrases->at(i)->GetPhrase() == data)
			return;

	// Add it to the phrase list.
	Phrase* phrase = new Phrase(data, new std::vector<Response*>(), true);
	known_phrases->push_back(phrase);
}

void ConversationManager::AddResponse(std::string data, std::string response)
{
	for (UINT i = 0; i < known_phrases->size(); ++i)
	{
		if (known_phrases->at(i)->GetPhrase() == data)
		{
			known_phrases->at(i)->Add(response);
		}
	}
}

std::vector<std::string>& ConversationManager::GetConversation() const
{
	return *conversation;
}


std::vector<Phrase*>& ConversationManager::GetKnownPhrases()
{
	return *known_phrases;
}


ConversationManager::ConversationManager()
	: conversation(), userInput(), known_phrases()
{}


void ConversationManager::Parse(std::string data)
{
	Trace::GetInstance().Write("ConversationManager::Parse");

	/* Look to the notes to see how the file is formatted. */

	std::string parent_phrase;
	std::vector<Response*>* response_list = new std::vector<Response*>();

	// Grab everything up to the '|'
	UINT index = 0;
	for (; index < data.size(); ++index)
	{
		if (data.at(index) == '|')
		{
			index++; // Skip the seperator.
			break;
		}

		parent_phrase.push_back(data.at(index));
	}

	// Grab everything (seperated  by ':' up until '>'
	std::string response;
	for (; index < data.size(); ++index)
	{
		if (data.at(index) == ':')
		{
			Response* r = new Response(response);
			response_list->push_back(r);
			response.clear();

			continue;
		}

		else if (data.at(index) == '>')
		{
			break;
		}

		response.push_back(data.at(index));
	}

	// Put it all into a Phrase object.
	Phrase* phrase = new Phrase(parent_phrase, response_list, false);

	// Put the newly created phrase into the list.
	known_phrases->push_back(phrase);
}
