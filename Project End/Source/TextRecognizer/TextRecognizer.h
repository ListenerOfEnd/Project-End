#pragma once

#include<string>
#include<vector>
#include<fstream>

#include<iostream>
#include<filesystem>
#include "../Text/Text.h"

#include "../Memory/Memory.h"
#include "../ConversationManager/ConversationManager.h"
#include "../MemoryManager/MemoryManager.h"

enum SentenceType
{
	//INVALID = 0,
	Question,
	Statement,

	NUM_SENTENCE_TYPES
};

class TextRecognizer
{
public:

	// Get function.
	// Returns: An instance of the class.
	static TextRecognizer& GetInstance();

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

	// Find function.
	// Finds out whether the given sentence is a question or a statement.
	// Params:
	//	sentence - The string to check.
	// Returns: Whether the sentence is a statement or a question.
	SentenceType FindSentenceType(std::string sentence);

	// Find function.
	// Finds out if a sentence has a reference to a previous memory.
	// Params:
	//	sentence - The string to check.
	// Returns: The memory (if any) the sentence is in reference to.
	Memory* FindMemoryReference(std::string sentence);

	// Find function.
	// Finds out if a given sentence is in reference to something said in the conversation previously.
	// NOTE: This function has an age restriction on the conversation's memory. Any string that is too close to the current place
	//	in the conversation is overlooked, much like real life, as close items are considered context to the current topic.
	// NOTE: This function also makes the referenced string a memory.
	// Params:
	//	sentence - The sentence to check.
	// Returns: The string being referenced earlier in the conversation.
	std::string FindConversationReference(std::string sentence);

	// Parse function.
	// Parses the sentence the user inputted to find the current topic
	// while also forming memories out of the sentence if need be.
	void ParseSentence();


private:

	TextRecognizer(); // Default Constructor.

	// Disable Copy Constructor and Assignment Operator.
	TextRecognizer(const TextRecognizer&) = delete;
	TextRecognizer& operator=(const TextRecognizer&) = delete;

	// Discern function.
	// Figures out the topic of a given sentence.
	// FOR INTERNAL USE ONLY.
	// Params:
	//	sentence - The sentence to look through.
	// Returns: The topic(s) of the sentence.
	std::string DiscernTopic(std::string sentence);

	// Parse function.
	// Takes data (mainly from a file) and parses it into a text object.
	// Params:
	//	data - The data to parse.
	void Parse(std::string data);

	std::vector<Text*>* keywords; // List of keywords known and to look for.

	/* While this may normally be put in the CM, I've put it here because of its need in tandem with memories. */
	Text* current_topic;

	// Tells the Text Recognizer whether the current user input has changed.
	Text* previous_input;

	std::string keywordFilePath = "./keywords.KEYWORD";

	const UINT MIN_AGE = 8;
};
