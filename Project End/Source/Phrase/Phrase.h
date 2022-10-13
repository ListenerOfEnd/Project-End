#pragma once

#include<string>
#include<vector>
#include "../Response/Response.h"

class Phrase /* Eventually we are going to integrate this into the memories. */
{
public:

	Phrase(); // Default Constructor.
	Phrase(const Phrase& other); // Copy Constructor.
	Phrase(std::string p, std::vector<Response*>* pr, bool m); // Non-Default Constructor.

	~Phrase(); // Destructor.

	// Add function.
	// Adds a response to the pr list.
	// Params:
	//	r - The response to add.
	// Returns: Whether or not the response was added.
	bool Add(Response* r);

	// Add function.
	// Adds a response to the pr list.
	// Params:
	//	s - The string to add.
	// Returns: Whether or not the response was added.
	bool Add(std::string s);

	// Remove function.
	// Removes a response from the pr list.
	// Params:
	//	r - The response to remove.
	void Remove(std::string r);

	// Get function.
	// Returns: The phrase.
	std::string GetPhrase() const;

	// Set function.
	// Sets the phrase.
	// Params:
	//	p - The phrase to set.
	void SetPhrase(std::string p);

	// Get function.
	// Returns: The possible responses list.
	std::vector<Response*>& GetResponseList() const;

	// Set function.
	// Sets the response list.
	// Params:
	//	list - The list to set.
	void SetResponseList(std::vector<Response*>* list);

	// Check function.
	// Returns: Whether or not the phrase is new/modified.
	bool Modified();

private:

	std::string phrase; // Functions as the filename as well.
	std::vector<Response*> *possible_responses;
	bool modified; // Tells us whether or not the phrase is new.
};
