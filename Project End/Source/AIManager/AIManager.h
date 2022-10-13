#pragma once

#include "../AI/AI.h"

enum class AIState
{
	INVALID = 0,

	Running,
	Stopping,
	Restarting,

	NUM_AI_STATES
};

class AIManager
{
public:

	// Get function.
	// Returns: An instance of the class.
	static AIManager& GetInstance();

	// Initialization function.
	void Init();

	// Start function.
	// Starts the AI.
	void Start();

	// Update function.
	// Updates the AI.
	void Update();

	// Stop function.
	// Stops the AI.
	void Stop();

	// Shutdown function.
	void Shutdown();

	// Check function.
	// Returns: Whether or not the AI is running.
	bool Running();

	// Set function.
	// Sets the next state.
	// Params:
	//	n - The next state.
	void SetNextState(AIState n);

	// Get function.
	// Returns: The current state of the AI.
	AIState GetCurrentState();

	// Get function.
	// Returns: The AI object.
	AI& GetAI() const;

private:

	AIManager(); // Default Constructor.

	// Disable Copy Constructor and Assignment Operator.
	AIManager(const AIManager&) = delete;
	AIManager& operator=(const AIManager&) = delete;

	AI* ai; // The AI object.

	AIState current_state; // The current state of the AIManager.
	AIState next_state; // The next state of the AIManager.
};
