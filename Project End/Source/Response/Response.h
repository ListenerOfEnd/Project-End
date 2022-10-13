#pragma once

#include<string>

class Response
{
public:

	Response(); // Default Constructor.
	Response(const Response& other); // Copy Constructor.
	Response(std::string r); // Non-Default Constructor.

	~Response(); // Destructor.

	// Get function.
	// Returns: The response string.
	std::string GetResponse() const;

	// Set function.
	// Sets the response string.
	// Params:
	//	res - The response to set.
	void SetResponse(std::string res);

private:

	std::string r; // The string that contains the response.
};
