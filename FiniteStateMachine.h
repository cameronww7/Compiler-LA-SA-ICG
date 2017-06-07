/**************************************************************************************
 * Class     		: CPSC 323-02
 * Assignment		: Programming Assignment 3
 * Assignment Name  : Syntax Anaylzer
 *
 * Programmer 1		: Cameron Walters
 * Programmer 2		: Sina Amini
 *************************************************************************************/

#ifndef FINITESTATEMACHINE_H_
#define FINITESTATEMACHINE_H_

#pragma once

#include <iostream> //for console io
#include <string>   //for using the string datatype
#include <cctype>

using namespace std;

const int OPERATORS_ARRAY_SIZE  = 10;
const int SEPARATORS_ARRAY_SIZE = 9;

 string operators[OPERATORS_ARRAY_SIZE]
                 	 = {"=",";", "<",">","+","-", "*", "/","!",":"};

 string separators[SEPARATORS_ARRAY_SIZE]
                  	  = {",", "}", "{", "(", ")", "[", "]", "|","@"};


 /***************************************************************
  * FiniteStateMachine
  * ------------------------------------------------------------
  * Class:
  * 		The Finite State Machine, takes in one character at
  * 		a time and checks to see if the current character is
  * 		valid in depending on the case, it will then return
  * 		a true or false or a acceptable or unacceptable state.
  ***************************************************************/
class FiniteStateMachine
{

public:
	FiniteStateMachine()
	{

	}

//	bool isAnIdentifier	   (char inChar);
	/***************************************************************
	 * FiniteStateMachine::		isAnAlphabetLetter
	 * ------------------------------------------------------------
	 * Function:
	 * 		The function will take in a character that will be
	 * 		checked to see if it fits the RAT15S coding style
	 * 		for an identifier and turns a true or false
	 ***************************************************************/
	bool isAnIdentifier(char inChar)
	{
		bool isALetter = false;

		if((isalpha(inChar) || isdigit(inChar) || inChar == '_')
				&& !isASeperator(inChar))
		{
			isALetter = true;
		}

		return isALetter;
	};

//	bool isAOperator	   (char inChar);
	/***************************************************************
	 * FiniteStateMachine::		isAOperator
	 * ------------------------------------------------------------
	 * Function:
	 * 		The function will take in a character that will be
	 * 		checked to see if it fits the RAT15S coding style
	 * 		for an Operator and turns a true or false
	 ***************************************************************/
	bool isAOperator(char inChar)
	{
		bool isAOperator = false;

		string temp;
		temp = "";
		temp = temp + inChar;

		for(int index = 0; index < OPERATORS_ARRAY_SIZE; index++ )
		{
			if(temp == operators[index])
			{
				isAOperator = true;
			}
		}

		return isAOperator;
	};


//	bool isASeperator	   (char inChar);
	/***************************************************************
	 * FiniteStateMachine::		isASeperator
	 * ------------------------------------------------------------
	 * Function:
	 * 		The function will take in a character that will be
	 * 		checked to see if it fits the RAT15S coding style
	 * 		for an Separator and turns a true or false
	 ***************************************************************/
	bool isASeperator(char inChar)
	{
		bool isASeperator = false;

		string temp;
		temp = "";
		temp = temp + inChar;

		for(int index = 0; index < SEPARATORS_ARRAY_SIZE; index++ )
		{
			if(temp == separators[index])
			{
				isASeperator = true;
			}
		}

		return isASeperator;
	};

//	bool isAnReal		   (char inChar);
	/***************************************************************
	 * FiniteStateMachine::		isAnReal
	 * ------------------------------------------------------------
	 * Function:
	 * 		The function will take in a character that will be
	 * 		checked to see if it fits the RAT15S coding style
	 * 		for an Real and turns a true or false
	 ***************************************************************/
	bool isAnReal(char inChar)
	{
		bool isAReal = false;

		if(inChar == '.')
		{
			isAReal = true;
		}

		return isAReal;
	};
//	bool isAInteger		   (char inChar);
	/***************************************************************
	 * FiniteStateMachine::		isAInteger
	 * ------------------------------------------------------------
	 * Function:
	 * 		The function will take in a character that will be
	 * 		checked to see if it fits the RAT15S coding style
	 * 		for an Integer and turns a true or false
	 ***************************************************************/
	bool isAInteger(char inChar)
	{
		bool isAInteger = false;

		string temp;
		temp = "";
		temp = temp + inChar;

		if(isdigit(temp[0]) && (!isASeperator(temp[0]) && !isalpha(temp[0])))
		{
			isAInteger = true;
		}

		return isAInteger;
	};

private:

};

///***************************************************************
// * FiniteStateMachine::		isAnAlphabetLetter
// * ------------------------------------------------------------
// * Function:
// * 		The function will take in a character that will be
// * 		checked to see if it fits the RAT15S coding style
// * 		for an identifier and turns a true or false
// ***************************************************************/
//bool FiniteStateMachine::isAnIdentifier(char inChar)
//{
//	bool isALetter = false;
//
//	if((isalpha(inChar) || isdigit(inChar) || inChar == '_')
//			&& !isASeperator(inChar))
//	{
//		isALetter = true;
//	}
//
//	return isALetter;
//}

///***************************************************************
// * FiniteStateMachine::		isAInteger
// * ------------------------------------------------------------
// * Function:
// * 		The function will take in a character that will be
// * 		checked to see if it fits the RAT15S coding style
// * 		for an Integer and turns a true or false
// ***************************************************************/
//bool FiniteStateMachine::isAInteger(char inChar)
//{
//	bool isAInteger = false;
//
//	string temp;
//	temp = "";
//	temp = temp + inChar;
//
//	if(isdigit(temp[0]) && (!isASeperator(temp[0]) && !isalpha(temp[0])))
//	{
//		isAInteger = true;
//	}
//
//	return isAInteger;
//}

///***************************************************************
// * FiniteStateMachine::		isAnReal
// * ------------------------------------------------------------
// * Function:
// * 		The function will take in a character that will be
// * 		checked to see if it fits the RAT15S coding style
// * 		for an Real and turns a true or false
// ***************************************************************/
//bool FiniteStateMachine::isAnReal(char inChar)
//{
//	bool isAReal = false;
//
//	if(inChar == '.')
//	{
//		isAReal = true;
//	}
//
//	return isAReal;
//}

///***************************************************************
// * FiniteStateMachine::		isASeperator
// * ------------------------------------------------------------
// * Function:
// * 		The function will take in a character that will be
// * 		checked to see if it fits the RAT15S coding style
// * 		for an Separator and turns a true or false
// ***************************************************************/
//bool FiniteStateMachine::isASeperator(char inChar)
//{
//	bool isASeperator = false;
//
//	string temp;
//	temp = "";
//	temp = temp + inChar;
//
//	for(int index = 0; index < SEPARATORS_ARRAY_SIZE; index++ )
//	{
//		if(temp == separators[index])
//		{
//			isASeperator = true;
//		}
//	}
//
//	return isASeperator;
//}

///***************************************************************
// * FiniteStateMachine::		isAOperator
// * ------------------------------------------------------------
// * Function:
// * 		The function will take in a character that will be
// * 		checked to see if it fits the RAT15S coding style
// * 		for an Operator and turns a true or false
// ***************************************************************/
//bool FiniteStateMachine::isAOperator(char inChar)
//{
//	bool isAOperator = false;
//
//	string temp;
//	temp = "";
//	temp = temp + inChar;
//
//	for(int index = 0; index < OPERATORS_ARRAY_SIZE; index++ )
//	{
//		if(temp == operators[index])
//		{
//			isAOperator = true;
//		}
//	}
//
//	return isAOperator;
//}




#endif /* FINITESTATEMACHINE_H_ */
