#pragma once

namespace redstone
{
	namespace lang
	{
		namespace interpreter
		{
			enum InterpreterStates
			{
				Idle = 0,
				WaitingForVariableName = 1,
				CreatingVariable = 2,
				WaitingForVariableType = 3,
				WaitingForVariableInitialization = 4,
				WaitingForInitializationValue = 5,
				WaitingForVariableCreationEnd = 6,
			};
		}
	}

}