#pragma once

#include<string>

class Text
{
public:

	Text(); // Default Constructor.
	Text(const Text& other); // Copy Constructor.
	Text(std::string t); // Non-Default Constructor.

	~Text(); // Destructor.

	// Get function.
	// Returns: The text string.
	std::string GetText() const;

	// Set function.
	// Params:
	//	t - The text data.
	void SetText(std::string t);

	// Get function.
	// Returns: Whether or not the text has been modified.
	bool Modified();

	// Set function.
	// Sets the modified tag.
	// Params:
	//	m - The tag to set.
	void SetModified(bool m);

	// Overload text assignment operator.
	Text& operator=(const std::string& data)
	{
		modified = true;
		text = data;

		return *this;
	}


private:

	std::string text;
	bool modified;
};
