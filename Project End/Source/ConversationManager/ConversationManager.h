#pragma once

#include "../Phrase/Phrase.h"
#include "../Response/Response.h"
#include "../GlobalDefines.hpp"

#include<iostream>
#include<fstream>
#include<filesystem>

class ConversationManager
{
public:

	// Get function.
	// Returns: An instance of the class.
	static ConversationManager& GetInstance();

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

	// Recount function.
	// Pushes the conversation in its entierety to the window.
	void RecountConversation();

	// Grab function.
	// Grabs the input from the user.
	// Returns: Whether or not the input contains any commands.
	bool GrabInput();

	// Get function.
	// Returns: The user input.
	std::string GetUserInput();

	// Response function.
	// Responds to the user.
	void Respond();

	// Expand function.
	// Expands the AI's vocabulary.
	void ExpandVocab();

	// Add function.
	// Adds a string to the conversation.
	// Params:
	//	data - The data to add.
	void AddToConversation(std::string data);

	// Add function.
	// Adds a phrase to the kp list.
	// Params:
	//	data - The data to add.
	void AddToKnownPhrases(std::string data);

	// Add function.
	// Adds a response to a given string.
	// Params:
	//	data - The data to add the response to.
	//	response - The response to add.
	void AddResponse(std::string data, std::string response);

	// Get function.
	// Returns: The conversation.
	std::vector<std::string>& GetConversation() const;

	// Get function.
	// Returns: The known phrases list.
	std::vector<Phrase*>& GetKnownPhrases();

private:

	ConversationManager(); // Default Constructor.

	// Disable Copy Constructor and Assignment Operator.
	ConversationManager(const ConversationManager&) = delete;
	ConversationManager& operator=(const ConversationManager&) = delete;

	// Parse function.
	// Parses the data given.
	// Params:
	//	data - The data to parse.
	void Parse(std::string data);

	std::vector<std::string>* conversation; // The current conversation.
	std::string userInput; // The input from the user.
	std::string previousOutput; // The previous output from the AI.

	std::vector<Phrase*>* known_phrases; // The phrases that the AI knows.
	const std::filesystem::path kp_path = "known_phrases.KP"; // The path for the known_phrases file.
};
