/**************************************************************************************
 * Class     		: CPSC 323-02
 * Assignment		: Programming Assignment 3
 * Assignment Name  : Syntax Analyzer
 *
 * Programmer 1		: Cameron Walters
 * Programmer 2		: Sina Amini
 *************************************************************************************/
#pragma once

#ifndef TOKEN_H_
#define TOKEN_H_

#include <iostream>
#include <string>

using namespace std;

/***************************************************************
 * Token
 * ------------------------------------------------------------
 * Class:
 * 		The Token class is a class which holds two strings,
 * 		first a string of the type of class (separator, integer..)
 * 		and the actual value of the string (int or abcd3)
 ***************************************************************/
class Token
{
	public:
		Token()
		{
			typeOfToken 	   = "n/a";
			actualValueOfToken = "n/a";
			lineNumber 		   = -1;
		}

		//Copy Constructor
		Token(const Token& outToken)
		{
			typeOfToken        = outToken.typeOfToken;
			actualValueOfToken = outToken.actualValueOfToken;
			lineNumber 		   = outToken.lineNumber;
		}

		~Token()
		{

		}

	    //Overloaded Operators
	    bool operator== (const Token& t)
	    {
	      return (typeOfToken == t.typeOfToken
	    		  && actualValueOfToken == t.actualValueOfToken);
	    };


		/***************************************************************
		 * setTypeOfToken
		 * ------------------------------------------------------------
		 * Function:
		 * 		This Function will take in a new type of token and store
		 * 		it in the typeOfToken string.
		 ***************************************************************/
		void setTypeOfToken(string inString)
		{
			typeOfToken = inString;
		};

		/***************************************************************
		 * setActualValueOfToken
		 * ------------------------------------------------------------
		 * Function:
		 * 		This Function will take in a new actual value and store
		 * 		it in the actualValueOfToken string.
		 ***************************************************************/
		void setActualValueOfToken(string inString)
		{
			actualValueOfToken = inString;
		};

		void setLineNumber (int inLineNumber)
		{
			lineNumber = inLineNumber;
		};

		/***************************************************************
		 * getTypeOfToken
		 * ------------------------------------------------------------
		 * Function:
		 * 		This function will return the current type of the token
		 ***************************************************************/
		const string getTypeOfToken()
		{
			return typeOfToken;
		};

		/***************************************************************
		 * getActualValueOfToken
		 * ------------------------------------------------------------
		 * Function:
		 * 		This Function will return the actual value of the token
		 ***************************************************************/
		const string getActualValueOfToken()
		{
			return actualValueOfToken;
		};

		const int getLineNumber()
		{
			return lineNumber;
		};

		/***************************************************************
		 * isItEmpty
		 * ------------------------------------------------------------
		 * Function:
		 * 		This Function will check to see if the token is empty
		 ***************************************************************/
		bool isItEmpty()
		{
			bool isitempty = false;

			if(actualValueOfToken != "")
			{
				isitempty = true;
			}

			return isitempty;
		};

	private:
		string typeOfToken;		   // Type of String (Int or Keyword, etc)
		string actualValueOfToken; // The Actual Value
		int    lineNumber;

};

#endif /* TOKEN_H_ */
