#include <iostream>
#include <fstream>
#include <string>

#include "ThyFileReader.h"
#include "keywords.h"
#include "states.h"



//	TODO: This function goes inside Interpreter class.
inline bool StringToInt(const std::string& string, int& value)
{
	std::string		AuxStr = string;

	int				StrSz = string.size();
	int				Num = 0;

	bool			AllNumbers = true;



	for (int i = 0; i < AuxStr.size(); i++)
	{
		if ((AuxStr[i] < 48) || (AuxStr[i] > 57))
		{
			AllNumbers = false;
		}
	}


	if (!AllNumbers)
	{
		//	Here, manage special case for other characters.

		//	Return false if none of the special cases was found!
		return false;
	}

	for (int i = 0; i < AuxStr.size(); i++)
	{
		Num += (AuxStr[i] - 48) * pow(10, StrSz - i - 1);
	}
	value = Num;



	return true;
}


int main()
{
	ThyFileReader thyFileReader = ThyFileReader();
	thyFileReader.ReadFile();
	std::string& script = thyFileReader.GetString();

	//	Analyzing.
	int i = 0;
	int size = script.size();
	int numericWordValue = 0;
	redstone::lang::interpreter::InterpreterStates lectorState = redstone::lang::interpreter::InterpreterStates::Idle;
	redstone::lang::interpreter::InterpreterStates interpreterState = redstone::lang::interpreter::InterpreterStates::Idle;


	//	TODO: Replace for variable type as a custom class...
	redstone::lang::Keywords variableType = redstone::lang::Keywords::NOT_A_KEYWORD;


	while (i < size)
	{
		//	Read a word; until it makes sense.
		while ((i + 1) < size)
		{
			numericWordValue += static_cast<int>(script[i]);

			if (
				(script[i] == ' ')	|| (script[i + 1] == ' ') ||
				(script[i] == '\n') || (script[i + 1] == '\n') ||
				(script[i] == ':')	|| (script[i + 1] == ':') ||
				(script[i] == '=') || (script[i + 1] == '=')
			)
			{
				break;
			}

			++i;
		}




		switch (lectorState)
		{
		case redstone::lang::interpreter::InterpreterStates::WaitingForVariableName:
			numericWordValue = redstone::lang::Keywords::KEYWORDS_FALLBACK;
			break;
		}

		switch (numericWordValue)
		{
		case redstone::lang::Keywords::_var:
		{
			std::cout << "User wants to create a variable.\n";
			std::cout << "Ok, I am expecting the name of the variable.\n";

			numericWordValue = 0;

			lectorState = redstone::lang::interpreter::InterpreterStates::WaitingForVariableName;
			interpreterState = redstone::lang::interpreter::InterpreterStates::CreatingVariable;
		}
		break;

		case redstone::lang::Keywords::__colon:
		{
			switch (interpreterState)
			{
			case redstone::lang::interpreter::InterpreterStates::CreatingVariable:
			{
				std::cout << "You are using the colon operator for defining the variable type.\n";
				std::cout << "Ok then, I'll read your type definition and see if it exists.\n";

				numericWordValue = 0;

				lectorState = redstone::lang::interpreter::InterpreterStates::WaitingForVariableType;
			}
			break;
			}
		}
		break;

		case redstone::lang::Keywords::__equal:
		{
			switch (interpreterState)
			{
			case redstone::lang::interpreter::InterpreterStates::CreatingVariable:
			{
				std::cout << "You are using the assign operator to initialize your variable.\n";
				std::cout << "I am expecting a initial value for it.\n";

				numericWordValue = 0;

				lectorState = redstone::lang::interpreter::InterpreterStates::WaitingForInitializationValue;
			}
			break;
			}
		}
		break;

		case redstone::lang::Keywords::__semicolon:
		{
			switch (interpreterState)
			{
			case redstone::lang::interpreter::InterpreterStates::CreatingVariable:
			{
				std::cout << "You have successfully created a variable.\n";
				std::cout << "The address of your allocated variable is @0x" << &numericWordValue << ".\n";

				numericWordValue = 0;
				lectorState = redstone::lang::interpreter::InterpreterStates::Idle;
				interpreterState = redstone::lang::interpreter::InterpreterStates::Idle;
				variableType = redstone::lang::Keywords::NOT_A_KEYWORD;
			}
			break;
			}
		}
		break;

		default:
			switch (lectorState)
			{
			case redstone::lang::interpreter::InterpreterStates::WaitingForVariableName:
			{
				//	For variable naming, following characters like
				//	' ', ':', or any other character that makes the reading end
				//	will set the reading cursor at that character.
				//	val a   : int = 0;
				//	     ^ There, that 'space' made the reading end. 

				bool readingVariableNameDone = false;
				bool readingVariableName = false;
				std::string variableName = std::string();

				while ((i < size) && (!readingVariableNameDone))
				{
					if (
						(('A' <= static_cast<int>(script[i])) &&
							(static_cast<int>(script[i]) <= 'Z')) ||
						(('a' <= static_cast<int>(script[i])) &&
							(static_cast<int>(script[i]) <= 'z')) ||
						(static_cast<int>(script[i]) == '_')
						)
					{
						variableName += script[i];
						readingVariableName = true;
					}
					else
					{
						if (readingVariableName)
						{
							readingVariableNameDone = true;
							break;
						}
					}

					++i;
				}
				
				lectorState = redstone::lang::interpreter::InterpreterStates::Idle;
				numericWordValue = 0;

				std::cout << script[i] << " | This is the last character @" << i << "\n";
				std::cout << "And the variable name is '" << variableName << "'.\n";

				--i;
			}
			break;

			case redstone::lang::interpreter::InterpreterStates::WaitingForVariableType:
			{
				//	For type definiton, following characters like
				//	' ', ':', or any other character that makes the reading end
				//	will set the reading cursor at that character.
				//	val a   : int = 0;
				//	             ^ There, that 'space' made the reading end. 

				bool readingVariableNameDone = false;
				bool readingVariableName = false;
				int numericTypeValue = 0;

				while ((i < size) && (!readingVariableNameDone))
				{
					if (
						(('A' <= static_cast<int>(script[i]))	&&
						(static_cast<int>(script[i]) <= 'Z')	)	||
						(('a' <= static_cast<int>(script[i]))	&&
						(static_cast<int>(script[i]) <= 'z')	)	||
						(static_cast<int>(script[i]) == '_')
						)
					{
						readingVariableName = true;
						numericTypeValue += static_cast<int>(script[i]);
					}
					else
					{
						if (readingVariableName)
						{
							readingVariableNameDone = true;
							break;
						}
					}

					++i;
				}
				--i;
				numericWordValue = 0;
				switch (numericTypeValue)
				{
				case redstone::lang::Keywords::_int:
					std::cout << "Seems like your variable type is an 'int' type.\n";
					std::cout << "I am expecting an initial value for your variable.\n";

					variableType = redstone::lang::Keywords::_int;
					break;

				default:
					std::cout << "That variable type does not exist, silly.\n";
					std::cout << "Reading paused!\n";
				}

				lectorState = redstone::lang::interpreter::InterpreterStates::WaitingForVariableInitialization;
			}
			break;

			case redstone::lang::interpreter::InterpreterStates::WaitingForVariableInitialization:
			{
				//	Ignore anything until find a assign operator '='.
				numericWordValue = 0;
			}
			break;

			case redstone::lang::interpreter::InterpreterStates::WaitingForInitializationValue:
			{
				//	For initialization value, following characters like
				//	';', ':', or any other character that makes the reading end
				//	will set the reading cursor at that character.
				//	val a   : int = 0;
				//	                 ^ There, that 'semicolon' made the reading end. 

				bool readingVariableNameDone = false;
				bool readingVariableName = false;
				int numericTypeValue = 0;
				std::string word = std::string();

				switch (variableType)
				{
				case redstone::lang::Keywords::_int:
				{
					while ((i < size) && (!readingVariableNameDone))
					{
						if (
							(('A' <= static_cast<int>(script[i]))	&&
							(static_cast<int>(script[i]) <= 'Z')	)	||
							(('a' <= static_cast<int>(script[i]))	&&
							(static_cast<int>(script[i]) <= 'z')	)	||
							(('0' <= static_cast<int>(script[i]))	&&
							(static_cast<int>(script[i]) <= '9'))	||
							(static_cast<int>(script[i]) == '_')	||
							(static_cast<int>(script[i]) == '\'')	||
							(static_cast<int>(script[i]) == '(')	||
							(static_cast<int>(script[i]) == ')')	||
							(static_cast<int>(script[i]) == '[')	||
							(static_cast<int>(script[i]) == ']')	
						)
						{
							word += script[i];
							readingVariableName = true;
							numericTypeValue += static_cast<int>(script[i]);
						}
						else
						{
							if (readingVariableName)
							{
								readingVariableNameDone = true;
								break;
							}
						}

						++i;
					}
					--i;
					numericWordValue = 0;

					std::cout << "The initial value for your variable of type 'int' is " << word << ".\n";
				}
				break;
				}

				lectorState = redstone::lang::interpreter::InterpreterStates::Idle;
			}
			break;

			case redstone::lang::interpreter::InterpreterStates::WaitingForVariableCreationEnd:
			{
				//	Ignore anything until find a semicolon operator ';'.
				numericWordValue = 0;
			}
			break;

			case redstone::lang::interpreter::InterpreterStates::Idle:
			{
				//	Ignore anything until find something interesting.
				numericWordValue = 0;
			}
			break;
			}
		}



		++i;
	}


	std::cin.get();
	return 0;
}