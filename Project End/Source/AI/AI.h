#pragma once

#include<string>
#include<vector>

class AI // This contains the vital functions and objects for the AI.
{
public:

	AI(); // Default Constructor.
	AI(const AI&) = delete; // Disable Copy Constructor.
	~AI(); // Destructor.

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

	// Check function.
	// Checks if there is a command that was entered.
	// Params:
	//	input - The stirng to check.
	// Returns: The index of the command and whether or not it is a command.
	bool IsCommand(std::string input, int &index);

private:

	/* ### DECPRECATED ### */
	// Input function.
	// Grabs the user's input.
	// Returns: Whether or not the user has pressed 'enter'.
	// bool GetUserInput();
	/* ### DEPRECATED ### */

	// Execute function.
	// Executes the given command.
	// Params:
	//	index - The command at the index given.
	void ExecuteCommand(unsigned int index);

	const std::vector<std::string> command_list = { "_toggle_trace_display", "_write_new_data", "_restart", "_stop",
	"_count_modified", "_commands", "_recount_conversation", "_erase", "_show_responses", "_cls" };

	int displayCounter;
};
