/**************************************************************************************
 * Class     		: CPSC 323-02
 * Assignment		: Programming Assignment 3
 * Assignment Name  : Syntax Analyzer
 *
 * Programmer 1		: Cameron Walters
 * Programmer 2		: Sina Amini
 *************************************************************************************/
#pragma once

#ifndef LEXICALANALYZER_H_
#define LEXICALANALYZER_H_

#include <iostream>
#include <ostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cctype>

#include "FiniteStateMachine.h"
#include "Token.h"

using namespace std;

const int KEYWORDS_ARRAY_SIZE   		= 13;
const int VALID_OPERATORS_ARRAY_SIZE 	= 14;

string keyWords[KEYWORDS_ARRAY_SIZE]
               	 = {"function","int","real","boolean", "write",
					    "while","read","if", "endif", "true", "false",
					    "return", "do"};

string validOperators[VALID_OPERATORS_ARRAY_SIZE]
                	 = {"=",";", "<",">","+","-", "*", "/","!",":",
                			 "!=","=>","<=", ":="};

/***************************************************************
 * LexicalAnalyzer
 * ------------------------------------------------------------
 * Class:
 * 		This class will if wanted will receive a input file
 * 		from main that will be processed for acceptable
 * 		RAT15S code.
 ***************************************************************/
class LexicalAnalyzer
{
	public:

		LexicalAnalyzer()
		{
			currentLineNumber = 1;
		}

//		void readFile(string inputFilePassed);
		/***************************************************************
		 * LexicalAnalyzer::	readFile
		 * ------------------------------------------------------------
		 * Function:
		 * 		This function will receive the passed in string of the
		 * 		file to be process according to RAT15S code. it will
		 * 		first open the file then process each individual
		 * 		character in the file one by one and style types of each
		 * 		code. Once compared through the Finite State Machine
		 * 		it will determine in the group of tokens is acceptable
		 * 		or unknown group of tokens. After which it will assign
		 * 		its type, a identifier, keyword, separator, operator,
		 * 		integer, or real. Once assigned it will output
		 * 		to the file and console if needed.
		 ***************************************************************/
		void readFile(string inputFilePassed)
		{
			Token temp;	//Temp storage of a token.
			FiniteStateMachine tempcurrentFSM; 	//Determines If Token is acceptable

			inputFile.open(inputFilePassed.c_str());

			// process the input file creating a vector of tokens.
			while(inputFile != 0)
			{
				temp.setActualValueOfToken(lexer());

				if(temp.getActualValueOfToken() == "@@")
				{
					temp.setLineNumber((currentLineNumber - 1));
				}
				else
				{
					temp.setLineNumber(currentLineNumber);
				}

				if(temp.getActualValueOfToken() != "")
				{
					temp.setTypeOfToken(returnTypeOfToken(temp.getActualValueOfToken(),
												  	  	  tempcurrentFSM));
					listOfTokens.push_back(temp);
				}
			}

			cout << endl;

//			outputFile.open("resultFile.txt");
//
//			int size = listOfTokens.size();
//
//			cout << left;
//			outputFile << left;
//
//			cout << setw(18) << "\n  Token  " << "  Lexeme\n\n";
//			outputFile << setw(18) << "\n  Token  " << "  Lexeme\n\n";
//
//			// outputs to a file and console. the vector of tokens
//			for(int index = 0; index < size; index++)
//			{
//				cout << setw(18) <<
//						listOfTokens.at(index).getTypeOfToken() <<
//						listOfTokens.at(index).getActualValueOfToken() << endl;
//
//				outputFile << setw(18) <<
//								listOfTokens.at(index).getTypeOfToken() <<
//								listOfTokens.at(index).getActualValueOfToken() << endl;
//
//			}
//			cout << endl << right;
//			outputFile << endl << right;
//
//			inputFile.close();
//			outputFile.close();
		};


//		string returnTypeOfToken(string inString, FiniteStateMachine tempcurrentFSM);
		/***************************************************************
		 * 	LexicalAnalyzer::	returnTypeOfToken
		 * ------------------------------------------------------------
		 * Function:
		 *		This function will return if the current string or soon
		 *		to be token has a valid name, as if it were a integer,
		 *		the token would have integer assign to it.
		 ***************************************************************/
		string returnTypeOfToken
										(string 			inString,
										 FiniteStateMachine tempcurrentFSM)
		{
			string temp = inString;

			// checks to see if a keyword or identifier
			if(isalpha(inString[0]))
			{
				for(int index = 0; index < KEYWORDS_ARRAY_SIZE; index++)
				{
					if(temp == keyWords[index])
					{
						temp = "Keyword";
					}
				}

				if(inString[inString.size()-1] == '_')
				{
					temp = "Unknown";
				}
				else if(temp != "Keyword")
				{
					temp = "Identifier";
				}
			}
			// checks to see if a real or integer
			else if(tempcurrentFSM.isAInteger(inString[0])
					|| tempcurrentFSM.isAnReal(inString[0]))
			{
				bool exitNow = false;
				int size = temp.size();
				int count = 0;

				for(int index = 0; index < size && !exitNow; index++)
				{
					if(temp[index] == '.')
					{
						count++;
					}
				}
				if(temp[0] == '.')
				{
					temp = "Unknown";
				}
				else if(count > 1)
				{
					temp = "Unknown";
				}
				else if(count == 1)
				{
					temp = "Real";
				}
				else
				{
					temp = "Integer";
				}
			}
			//checks to see if its a Operator
			else if(tempcurrentFSM.isAOperator(inString[0]))
			{
				bool exitNow = false;
				for(int index = 0; index < VALID_OPERATORS_ARRAY_SIZE && !exitNow; index++)
				{
					if(inString == validOperators[index])
					{
						exitNow = true;
						temp = "Operator";
					}
					else
					{
						temp = "Unknown";
					}
				}

			}
			//checks to see if its a Separator
			else if (tempcurrentFSM.isASeperator(inString[0]))
			{
				if(inString[1] == '@')
				{
					temp = "Separator";
				}
				else if (inString.size() == 1)
				{
					temp = "Separator";
				}
				else
				{
					temp = "Unknown";
				}
			}
			else
			{
				temp = "Unknown";
			}

			return temp;
		};

		//Gives the token list
		vector<Token> getTokenList()
		{
		  return listOfTokens;
		}

		//Empties the token list
		void emptyTokenList()
		{
			listOfTokens.clear();
		}

//		void setLineNumberBacktoDefault();
		void setLineNumberBacktoDefault()
		{
			currentLineNumber = 1;
		};

	private:
		ifstream 			 inputFile;	   // Input File
		ofstream			 outputFile;   // Output File
		vector <Token> 		 listOfTokens; // Vector of Tokens

		int					 currentLineNumber;

//		string lexer();
		/***************************************************************
		 * LexicalAnalyzer::	lexer
		 * ------------------------------------------------------------
		 * Function:
		 * 		This function will be called to help assist the Finite
		 * 		state machine, it will grab each individual token from
		 * 		the input file and process it. it will check to see if
		 * 		the current characters is an acceptable char and if the
		 * 		arrangement of the char are acceptable to create a valid
		 * 		token.
		 ***************************************************************/
		string lexer()
		{
			FiniteStateMachine currentFSM;
			string 			   stringTemp;
			char   			   charTemp;
			char			   secondCharTemp;

			bool  			   exitNow        = false;
			bool			   emegrencyBreak = false;

			charTemp 	   = ' ';
			secondCharTemp = ' ';
			stringTemp.clear();

			charTemp = inputFile.get();

			//This While will read in one char at a time making sure
			//that there is white space in front and behind some kind of data
			while(charTemp != ' '
					&& exitNow == false
						&& !inputFile.eof()
							&& emegrencyBreak == false
								&& charTemp != '\n'
									&& charTemp != '\t')
			{
					if(charTemp == '\n')
					{
						exitNow = true;
					}

					//COMMENT EATER
					if(charTemp == '/' && stringTemp.empty())
					{
						bool commentExitCheck = false;
						secondCharTemp = inputFile.get();

						if(secondCharTemp == '*')
						{
							secondCharTemp = inputFile.get();

							while(commentExitCheck == false)
							{
								if(charTemp == '\n' || secondCharTemp == '\n')
								{
									currentLineNumber = currentLineNumber + 1;
								}

								if(secondCharTemp == '*')
								{
									secondCharTemp = inputFile.get();

									if(secondCharTemp == '/')
									{
										commentExitCheck = true;
									}
									else
									{
										inputFile.unget();
									}
								}
								else
								{
									secondCharTemp = inputFile.get();
								}

							}

							charTemp = inputFile.get();
						}
						else
						{
							inputFile.unget();
						}
					}

					/***************************************************************
					 * if (currentFSM.isAOperator(charTemp) && exitNow != true)
					 * ------------------------------------------------------------
					 * If Statement:
					 *		This if Statement will compare to see if the current
					 *		character that was input from a file is an operator, and if so
					 *		it will add it to the current string of characters, and if not
					 *		it will un-grab the character and exit, sending a string back
					 *		to the original program.
					 ***************************************************************/
					if (currentFSM.isAOperator(charTemp) && exitNow != true)
					{

						if(charTemp == '!' && stringTemp.empty())
						{
							secondCharTemp = inputFile.get();

							if(secondCharTemp == '=')
							{
								stringTemp = stringTemp + charTemp + secondCharTemp;
							}
							else
							{
								stringTemp = stringTemp + charTemp;
								inputFile.unget();
							}
						}
						else if(charTemp == '=' && stringTemp.empty())
						{
							secondCharTemp = inputFile.get();

							if(secondCharTemp == '>')
							{
								stringTemp = stringTemp + charTemp + secondCharTemp;
							}
							else
							{
								stringTemp = stringTemp + charTemp;
								inputFile.unget();
							}
						}
						else if(charTemp == '<' && stringTemp.empty())
						{
							secondCharTemp = inputFile.get();

							if(secondCharTemp == '=')
							{
								stringTemp = stringTemp + charTemp + secondCharTemp;
							}
							else
							{
								stringTemp = stringTemp + charTemp;
								inputFile.unget();
							}
						}
						else if(charTemp == ':' && stringTemp.empty())
						{
							secondCharTemp = inputFile.get();

							if(secondCharTemp == '=')
							{
								stringTemp = stringTemp + charTemp + secondCharTemp;
							}
							else
							{
								stringTemp = stringTemp + charTemp;
								inputFile.unget();
							}
						}
						else if(stringTemp.empty())
						{
							stringTemp = stringTemp + charTemp;
							exitNow    = true;
							inputFile.unget();
						}
						else
						{
							inputFile.unget();
							emegrencyBreak = true;
						}

					}
					/***************************************************************
					 * else if(currentFSM.isASeperator(charTemp) && exitNow != true)
					 * ------------------------------------------------------------
					 * If Statement:
					 *		This if Statement will compare to see if the current
					 *		character that was input from a file is an Separator, and if so
					 *		it will add it to the current string of characters, and if not
					 *		it will un-grab the character and exit, sending a string back
					 *		to the original program. also It will check to see if its
					 *		another @@ because RAT15S has to have a specific set of chars
					 *		with this separator.
					 ***************************************************************/
					else if(currentFSM.isASeperator(charTemp) && exitNow != true)
					{
						if(charTemp == '@' && stringTemp.empty())
						{
							secondCharTemp = inputFile.get();

							if(secondCharTemp == '@')
							{
								stringTemp = stringTemp + charTemp + secondCharTemp;
		//						currentLineNumber = currentLineNumber - 1;
							}
							else
							{
								stringTemp = stringTemp + charTemp;
								inputFile.unget();
							}
						}
						else if(stringTemp.empty())
						{
							stringTemp = stringTemp + charTemp;
							exitNow    = true;
							inputFile.unget();
						}
						else
						{
							inputFile.unget();
							emegrencyBreak = true;
						}
					}

					/***************************************************************
					 * 	else if(currentFSM.isAInteger(charTemp) && exitNow != true)
					 * ------------------------------------------------------------
					 * If Statement:
					 *		This if Statement will compare to see if the current
					 *		character that was input from a file is an Integer. if
					 *		so it will then add it to the current string. if not
					 *		it will exit the function.
					 ***************************************************************/
					else if(currentFSM.isAInteger(charTemp) && exitNow != true)
					{
						if(!currentFSM.isAOperator(stringTemp[0]))
						{
							stringTemp = stringTemp + charTemp;
						}
						else
						{
							inputFile.unget();
							emegrencyBreak = true;
						}

					}
					/***************************************************************
					 * 	else if(currentFSM.isAnReal(charTemp) && exitNow != true)
					 * ------------------------------------------------------------
					 * If Statement:
					 *		This if Statement will compare to see if the current
					 *		character that was input from a file is an Real, if so
					 *		it will then grab every additional valid integer and then
					 *		exit the loop sending back a real number.
					 ***************************************************************/
					else if(currentFSM.isAnReal(charTemp) && exitNow != true)
					{
						stringTemp = stringTemp + charTemp;
						charTemp   = inputFile.get();

						while(	(currentFSM.isAInteger(charTemp)
							  || currentFSM.isAnReal(charTemp))
							  && exitNow != true)
						{
								stringTemp = stringTemp + charTemp;
								charTemp   = inputFile.get();
						}

						if(!currentFSM.isAInteger(charTemp))
						{
							emegrencyBreak = true;
							inputFile.unget();
						}
					}
					/***************************************************************
					 * 	else if(	currentFSM.isAnAlphabetLetter(charTemp)
					 *		&& !currentFSM.isASeperator	     (charTemp)
					 *		&& !currentFSM.isAOperator	     (charTemp)
					 *		&& exitNow != true)
					 * ------------------------------------------------------------
					 * If Statement:
					 *		This if Statement will compare to see if the current
					 *		character that was input from a file is an a alphabet
					 *		character. if so it will add it to the current string
					 *		and then when an invalid character enters it will
					 *		ungrab and exit the function sending a valid token string
					 ***************************************************************/
					else if(	currentFSM.isAnIdentifier(charTemp)
								&& !currentFSM.isASeperator(charTemp)
								&& !currentFSM.isAOperator(charTemp)
								&& exitNow != true)
					{

						if(isdigit(stringTemp[0])
								|| currentFSM.isASeperator(charTemp)
								|| currentFSM.isAOperator(stringTemp[0]))
						{
							inputFile.unget();
							emegrencyBreak = true;
						}
						else
						{
							stringTemp = stringTemp + charTemp;
						}
					}
					else
					{
						exitNow = true;
						inputFile.unget();
					}

					if(stringTemp.empty() && charTemp != ' '
										&& charTemp != '\n'
										&& charTemp != '\t')
					{
						stringTemp = stringTemp + charTemp;
						exitNow = true;
					}

					if(emegrencyBreak == false)
					{
						charTemp = inputFile.get();
					}

				}

			if(charTemp == '\n')
			{
				currentLineNumber = currentLineNumber + 1;
			}

			return stringTemp;
		};
};

///***************************************************************
// * LexicalAnalyzer::	readFile
// * ------------------------------------------------------------
// * Function:
// * 		This function will receive the passed in string of the
// * 		file to be process according to RAT15S code. it will
// * 		first open the file then process each individual
// * 		character in the file one by one and style types of each
// * 		code. Once compared through the Finite State Machine
// * 		it will determine in the group of tokens is acceptable
// * 		or unknown group of tokens. After which it will assign
// * 		its type, a identifier, keyword, separator, operator,
// * 		integer, or real. Once assigned it will output
// * 		to the file and console if needed.
// ***************************************************************/
//void LexicalAnalyzer::readFile(string inputFilePassed)
//{
//	Token temp;	//Temp storage of a token.
//	FiniteStateMachine tempcurrentFSM; 	//Determines If Token is acceptable
//
//	inputFile.open(inputFilePassed.c_str());
//
//	// process the input file creating a vector of tokens.
//	while(inputFile != 0)
//	{
//		temp.setActualValueOfToken(lexer());
//
//		if(temp.getActualValueOfToken() == "@@")
//		{
//			temp.setLineNumber((currentLineNumber - 1));
//		}
//		else
//		{
//			temp.setLineNumber(currentLineNumber);
//		}
//
//		if(temp.getActualValueOfToken() != "")
//		{
//			temp.setTypeOfToken(returnTypeOfToken(temp.getActualValueOfToken(),
//										  	  	  tempcurrentFSM));
//			listOfTokens.push_back(temp);
//		}
//	}
//
//	outputFile.open("resultFile.txt");
//
//	int size = listOfTokens.size();
//
//	cout << left;
//	outputFile << left;
//
//	cout << setw(18) << "\n  Token  " << "  Lexeme\n\n";
//	outputFile << setw(18) << "\n  Token  " << "  Lexeme\n\n";
//
//	// outputs to a file and console. the vector of tokens
//	for(int index = 0; index < size; index++)
//	{
//		cout << setw(18) <<
//				listOfTokens.at(index).getTypeOfToken() <<
//				listOfTokens.at(index).getActualValueOfToken() << endl;
//
//		outputFile << setw(18) <<
//						listOfTokens.at(index).getTypeOfToken() <<
//						listOfTokens.at(index).getActualValueOfToken() << endl;
//
//	}
//	cout << endl << right;
//	outputFile << endl << right;
//
//	inputFile.close();
//	outputFile.close();
//}

///***************************************************************
// * LexicalAnalyzer::	lexer
// * ------------------------------------------------------------
// * Function:
// * 		This function will be called to help assist the Finite
// * 		state machine, it will grab each individual token from
// * 		the input file and process it. it will check to see if
// * 		the current characters is an acceptable char and if the
// * 		arrangement of the char are acceptable to create a valid
// * 		token.
// ***************************************************************/
//string LexicalAnalyzer::lexer()
//{
//	FiniteStateMachine currentFSM;
//	string 			   stringTemp;
//	char   			   charTemp;
//	char			   secondCharTemp;
//
//	bool  			   exitNow        = false;
//	bool			   emegrencyBreak = false;
//
//	charTemp 	   = ' ';
//	secondCharTemp = ' ';
//	stringTemp.clear();
//
//	charTemp = inputFile.get();
//
//	//This While will read in one char at a time making sure
//	//that there is white space in front and behind some kind of data
//	while(charTemp != ' '
//			&& exitNow == false
//				&& !inputFile.eof()
//					&& emegrencyBreak == false
//						&& charTemp != '\n'
//							&& charTemp != '\t')
//	{
//			if(charTemp == '\n')
//			{
//				exitNow = true;
//			}
//
//			//COMMENT EATER
//			if(charTemp == '/' && stringTemp.empty())
//			{
//				bool commentExitCheck = false;
//				secondCharTemp = inputFile.get();
//
//				if(secondCharTemp == '*')
//				{
//					secondCharTemp = inputFile.get();
//
//					while(commentExitCheck == false)
//					{
//						if(charTemp == '\n' || secondCharTemp == '\n')
//						{
//							currentLineNumber = currentLineNumber + 1;
//						}
//
//						if(secondCharTemp == '*')
//						{
//							secondCharTemp = inputFile.get();
//
//							if(secondCharTemp == '/')
//							{
//								commentExitCheck = true;
//							}
//							else
//							{
//								inputFile.unget();
//							}
//						}
//						else
//						{
//							secondCharTemp = inputFile.get();
//						}
//
//					}
//
//					charTemp = inputFile.get();
//				}
//				else
//				{
//					inputFile.unget();
//				}
//			}
//
//			/***************************************************************
//			 * if (currentFSM.isAOperator(charTemp) && exitNow != true)
//			 * ------------------------------------------------------------
//			 * If Statement:
//			 *		This if Statement will compare to see if the current
//			 *		character that was input from a file is an operator, and if so
//			 *		it will add it to the current string of characters, and if not
//			 *		it will un-grab the character and exit, sending a string back
//			 *		to the original program.
//			 ***************************************************************/
//			if (currentFSM.isAOperator(charTemp) && exitNow != true)
//			{
//
//				if(charTemp == '!' && stringTemp.empty())
//				{
//					secondCharTemp = inputFile.get();
//
//					if(secondCharTemp == '=')
//					{
//						stringTemp = stringTemp + charTemp + secondCharTemp;
//					}
//					else
//					{
//						stringTemp = stringTemp + charTemp;
//						inputFile.unget();
//					}
//				}
//				else if(charTemp == '=' && stringTemp.empty())
//				{
//					secondCharTemp = inputFile.get();
//
//					if(secondCharTemp == '>')
//					{
//						stringTemp = stringTemp + charTemp + secondCharTemp;
//					}
//					else
//					{
//						stringTemp = stringTemp + charTemp;
//						inputFile.unget();
//					}
//				}
//				else if(charTemp == '<' && stringTemp.empty())
//				{
//					secondCharTemp = inputFile.get();
//
//					if(secondCharTemp == '=')
//					{
//						stringTemp = stringTemp + charTemp + secondCharTemp;
//					}
//					else
//					{
//						stringTemp = stringTemp + charTemp;
//						inputFile.unget();
//					}
//				}
//				else if(charTemp == ':' && stringTemp.empty())
//				{
//					secondCharTemp = inputFile.get();
//
//					if(secondCharTemp == '=')
//					{
//						stringTemp = stringTemp + charTemp + secondCharTemp;
//					}
//					else
//					{
//						stringTemp = stringTemp + charTemp;
//						inputFile.unget();
//					}
//				}
//				else if(stringTemp.empty())
//				{
//					stringTemp = stringTemp + charTemp;
//					exitNow    = true;
//					inputFile.unget();
//				}
//				else
//				{
//					inputFile.unget();
//					emegrencyBreak = true;
//				}
//
//			}
//			/***************************************************************
//			 * else if(currentFSM.isASeperator(charTemp) && exitNow != true)
//			 * ------------------------------------------------------------
//			 * If Statement:
//			 *		This if Statement will compare to see if the current
//			 *		character that was input from a file is an Separator, and if so
//			 *		it will add it to the current string of characters, and if not
//			 *		it will un-grab the character and exit, sending a string back
//			 *		to the original program. also It will check to see if its
//			 *		another @@ because RAT15S has to have a specific set of chars
//			 *		with this separator.
//			 ***************************************************************/
//			else if(currentFSM.isASeperator(charTemp) && exitNow != true)
//			{
//				if(charTemp == '@' && stringTemp.empty())
//				{
//					secondCharTemp = inputFile.get();
//
//					if(secondCharTemp == '@')
//					{
//						stringTemp = stringTemp + charTemp + secondCharTemp;
////						currentLineNumber = currentLineNumber - 1;
//					}
//					else
//					{
//						stringTemp = stringTemp + charTemp;
//						inputFile.unget();
//					}
//				}
//				else if(stringTemp.empty())
//				{
//					stringTemp = stringTemp + charTemp;
//					exitNow    = true;
//					inputFile.unget();
//				}
//				else
//				{
//					inputFile.unget();
//					emegrencyBreak = true;
//				}
//			}
//
//			/***************************************************************
//			 * 	else if(currentFSM.isAInteger(charTemp) && exitNow != true)
//			 * ------------------------------------------------------------
//			 * If Statement:
//			 *		This if Statement will compare to see if the current
//			 *		character that was input from a file is an Integer. if
//			 *		so it will then add it to the current string. if not
//			 *		it will exit the function.
//			 ***************************************************************/
//			else if(currentFSM.isAInteger(charTemp) && exitNow != true)
//			{
//				if(!currentFSM.isAOperator(stringTemp[0]))
//				{
//					stringTemp = stringTemp + charTemp;
//				}
//				else
//				{
//					inputFile.unget();
//					emegrencyBreak = true;
//				}
//
//			}
//			/***************************************************************
//			 * 	else if(currentFSM.isAnReal(charTemp) && exitNow != true)
//			 * ------------------------------------------------------------
//			 * If Statement:
//			 *		This if Statement will compare to see if the current
//			 *		character that was input from a file is an Real, if so
//			 *		it will then grab every additional valid integer and then
//			 *		exit the loop sending back a real number.
//			 ***************************************************************/
//			else if(currentFSM.isAnReal(charTemp) && exitNow != true)
//			{
//				stringTemp = stringTemp + charTemp;
//				charTemp   = inputFile.get();
//
//				while(	(currentFSM.isAInteger(charTemp)
//					  || currentFSM.isAnReal(charTemp))
//					  && exitNow != true)
//				{
//						stringTemp = stringTemp + charTemp;
//						charTemp   = inputFile.get();
//				}
//
//				if(!currentFSM.isAInteger(charTemp))
//				{
//					emegrencyBreak = true;
//					inputFile.unget();
//				}
//			}
//			/***************************************************************
//			 * 	else if(	currentFSM.isAnAlphabetLetter(charTemp)
//			 *		&& !currentFSM.isASeperator	     (charTemp)
//			 *		&& !currentFSM.isAOperator	     (charTemp)
//			 *		&& exitNow != true)
//			 * ------------------------------------------------------------
//			 * If Statement:
//			 *		This if Statement will compare to see if the current
//			 *		character that was input from a file is an a alphabet
//			 *		character. if so it will add it to the current string
//			 *		and then when an invalid character enters it will
//			 *		ungrab and exit the function sending a valid token string
//			 ***************************************************************/
//			else if(	currentFSM.isAnIdentifier(charTemp)
//						&& !currentFSM.isASeperator(charTemp)
//						&& !currentFSM.isAOperator(charTemp)
//						&& exitNow != true)
//			{
//
//				if(isdigit(stringTemp[0])
//						|| currentFSM.isASeperator(charTemp)
//						|| currentFSM.isAOperator(stringTemp[0]))
//				{
//					inputFile.unget();
//					emegrencyBreak = true;
//				}
//				else
//				{
//					stringTemp = stringTemp + charTemp;
//				}
//			}
//			else
//			{
//				exitNow = true;
//				inputFile.unget();
//			}
//
//			if(stringTemp.empty() && charTemp != ' '
//								&& charTemp != '\n'
//								&& charTemp != '\t')
//			{
//				stringTemp = stringTemp + charTemp;
//				exitNow = true;
//			}
//
//			if(emegrencyBreak == false)
//			{
//				charTemp = inputFile.get();
//			}
//
//		}
//
//	if(charTemp == '\n')
//	{
//		currentLineNumber = currentLineNumber + 1;
//	}
//
//	return stringTemp;
//}

///***************************************************************
// * 	LexicalAnalyzer::	returnTypeOfToken
// * ------------------------------------------------------------
// * Function:
// *		This function will return if the current string or soon
// *		to be token has a valid name, as if it were a integer,
// *		the token would have integer assign to it.
// ***************************************************************/
//string LexicalAnalyzer::returnTypeOfToken
//								(string 			inString,
//								 FiniteStateMachine tempcurrentFSM)
//{
//	string temp = inString;
//
//	// checks to see if a keyword or identifier
//	if(isalpha(inString[0]))
//	{
//		for(int index = 0; index < KEYWORDS_ARRAY_SIZE; index++)
//		{
//			if(temp == keyWords[index])
//			{
//				temp = "Keyword";
//			}
//		}
//
//		if(inString[inString.size()-1] == '_')
//		{
//			temp = "Unknown";
//		}
//		else if(temp != "Keyword")
//		{
//			temp = "Identifier";
//		}
//	}
//	// checks to see if a real or integer
//	else if(tempcurrentFSM.isAInteger(inString[0])
//			|| tempcurrentFSM.isAnReal(inString[0]))
//	{
//		bool exitNow = false;
//		int size = temp.size();
//		int count = 0;
//
//		for(int index = 0; index < size && !exitNow; index++)
//		{
//			if(temp[index] == '.')
//			{
//				count++;
//			}
//		}
//		if(temp[0] == '.')
//		{
//			temp = "Unknown";
//		}
//		else if(count > 1)
//		{
//			temp = "Unknown";
//		}
//		else if(count == 1)
//		{
//			temp = "Real";
//		}
//		else
//		{
//			temp = "Integer";
//		}
//	}
//	//checks to see if its a Operator
//	else if(tempcurrentFSM.isAOperator(inString[0]))
//	{
//		bool exitNow = false;
//		for(int index = 0; index < VALID_OPERATORS_ARRAY_SIZE && !exitNow; index++)
//		{
//			if(inString == validOperators[index])
//			{
//				exitNow = true;
//				temp = "Operator";
//			}
//			else
//			{
//				temp = "Unknown";
//			}
//		}
//
//	}
//	//checks to see if its a Separator
//	else if (tempcurrentFSM.isASeperator(inString[0]))
//	{
//		if(inString[1] == '@')
//		{
//			temp = "Separator";
//		}
//		else if (inString.size() == 1)
//		{
//			temp = "Separator";
//		}
//		else
//		{
//			temp = "Unknown";
//		}
//	}
//	else
//	{
//		temp = "Unknown";
//	}
//
//	return temp;
//}

//void LexicalAnalyzer::setLineNumberBacktoDefault()
//{
//	currentLineNumber = 1;
//}



#endif /* LEXICALANALYZER_H_ */
