#include "TextRecognizer.h"


TextRecognizer& TextRecognizer::GetInstance()
{
	static TextRecognizer instance;
	return instance;
}


void TextRecognizer::Load()
{
	keywords = new std::vector<Text*>();
	current_topic = new Text();
	previous_input = new Text();

	// Open up the text file.
	std::fstream keywordFile;
	keywordFile.open(keywordFilePath, std::fstream::in);

	if (keywordFile.good())
	{
		std::string line;
		while (std::getline(keywordFile, line))
		{
			// Check if there's a newline at the end of the line.
			if (line.at(line.size()) == '\n')
			{
				line.pop_back();
			}

			Text* text = new Text(line);
			keywords->push_back(text);
		}
	}

	keywordFile.close();
}


void TextRecognizer::Init()
{
	// Go through and conduct an integrity check.
	for (UINT i = 0; i < keywords->size(); ++i)
		if (keywords->at(i)->GetText().empty())
			keywords->erase(keywords->begin() + i);
}


void TextRecognizer::Update()
{
	// Conduct integrity check.
	for (UINT i = 0; i < keywords->size(); ++i)
		if (keywords->at(i)->GetText().empty())
			keywords->erase(keywords->begin() + i);

	// Check if the current user input is the same as the previous input.
	if (ConversationManager::GetInstance().GetUserInput() != previous_input->GetText())
	{
		std::string input = ConversationManager::GetInstance().GetUserInput();
		previous_input->SetText(input);

		// If it's not, we need to discern the new topic.
		ParseSentence();
	}
}


void TextRecognizer::Shutdown()
{
	std::ofstream file;
	file.open(keywordFilePath, std::ios::out);

	// Add any new keywords to the file.
	for (UINT i = 0; i < keywords->size(); ++i)
	{
		if (keywords->at(i)->Modified())
		{
			

			if (file.good())
			{
				file.write(keywords->at(i)->GetText().c_str() + '\n', keywords->at(i)->GetText().size() + 1);
			}
		}
	}

	file.close();
}


void TextRecognizer::Unload()
{
	// Clear out the keywords list.
	keywords->clear();
	delete keywords;
	keywords = nullptr;

	delete current_topic;
	current_topic = nullptr;

	delete previous_input;
	previous_input = nullptr;

	keywordFilePath.clear();
}


SentenceType TextRecognizer::FindSentenceType(std::string sentence)
{
	// Go through the string and find a question mark.
	for (UINT i = 0; i < sentence.size(); ++i)
	{
		if (sentence.at(i) == '?')
		{
			return SentenceType::Question;
		}
	}

	return SentenceType::Statement;
}


Memory* TextRecognizer::FindMemoryReference(std::string sentence)
{
	return &MemoryManager::GetInstance().Find(DiscernTopic(sentence));
}


std::string TextRecognizer::FindConversationReference(std::string sentence)
{
	std::string topic = DiscernTopic(sentence);

	if (ConversationManager::GetInstance().GetConversation().size() - MIN_AGE < 0)
		return nullptr;

	// Go through the conversation and find the any topic references (up to a certain age).
	for (UINT i = 0; i < ConversationManager::GetInstance().GetConversation().size() - MIN_AGE; ++i)
	{
		// Go through each sentence and find the earliest reference of the topic.
		std::string current_sentence = ConversationManager::GetInstance().GetConversation().at(i);
		for (UINT j = 0; j < current_sentence.size(); ++j)
		{
			if (current_sentence.find(topic.c_str(), 0, current_sentence.size()))
			{
				return current_sentence;
			}
		}
	}

	return nullptr;
}


void TextRecognizer::ParseSentence()
{
	std::string userInput = ConversationManager::GetInstance().GetUserInput();

	int first_noun = 0;
	while (userInput.size() > 0)
	{
		// Look for nouns.
		if (first_noun == 0)
		{
			// This will be the current topic.
			std::string topic = DiscernTopic(userInput);

			if (!topic.empty())
			{
				current_topic->SetText(topic);
				current_topic->SetModified(true);
			}
		}
	}

	// Look through memories and see if the current topic is in one.
	if (!MemoryManager::GetInstance().Remember());
}


TextRecognizer::TextRecognizer()
	: keywords(), current_topic(), previous_input()
{}


std::string TextRecognizer::DiscernTopic(std::string sentence)
{
	std::string upperSentence;
	for (unsigned int i = 0; i < sentence.size(); ++i)
	{
		upperSentence.push_back((char)toupper(sentence.at(i)));
	}

	// Return the first noun we come across.
	for (unsigned int i = 0; i < keywords->size(); ++i)
	{	
		if (upperSentence.find(keywords->at(i)->GetText()))
		{
			return keywords->at(i)->GetText();
		}
	}


	return std::string();
}


void TextRecognizer::Parse(std::string data)
{
	Text* newText = new Text();
	newText->SetModified(false);

	for (UINT i = 0; i < data.size(); ++i)
	{
		if (data.at(i) == '\n')
		{
			data.erase(data.begin() + i);
			break;
		}
	}

	newText->SetText(data);
	keywords->push_back(newText);
}
