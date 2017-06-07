/**************************************************************************************
 * Class     		: CPSC 323-02
 * Assignment		: Programming Assignment 3
 * Assignment Name  : Syntax Analyzer
 *
 * Programmer 1		: Cameron Walters
 * Programmer 2		: Sina Amini
 *************************************************************************************/

#pragma once

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <deque>
#include <stdlib.h>

#include "Token.h"

#include "SymbolTable.h"
#include "InstructionTable.h"

using namespace std;

/***************************************************************
 * SyntaxAnalyzer
 * ------------------------------------------------------------
 * Class:
 * 		This class will check a vector of tokens from the
 * 		lexical analyzer, it will output the expected
 * 		form of RAT15S
 *
 ***************************************************************/
class SyntaxAnalyzer
{
	public:

		SyntaxAnalyzer()
		{
			error 				= false;
			print_production 	= false;
			current_line 		= 0;
		}

		~SyntaxAnalyzer()
		{

		}

		/***************************************************************
		 * SyntaxAnalyzer::		analyzeSyntax
		 * ------------------------------------------------------------
		 * Function:
		 *		This function will make sure the vector list of tokens
		 *		coming in isn't empty, cause if its empty there's no
		 *		reason to run. after found populated list it will then
		 *		open and start the process of the output file. once
		 *		open it then enters the second if which heads to the
		 *		start of the list and then calls a simulated Lexer call
		 *		which grabs the first token in list then takes it to the
		 *		rat15S which is the analyzer of Syntax and begins the
		 *		Syntax Analyzer as a whole. Once it has found an error
		 *		or finishes it will then output to the console where
		 *		the file was saved and then exit the function.
		 ***************************************************************/
		void analyzeSyntax(const vector<Token>& t_list,
								 string 		in_name,
								 bool 			print = false)
		{
			if(!t_list.empty())
			{
				in_name.erase(in_name.length()-4, in_name.length());

				string out_name = in_name.append("_output.txt");

				out_file.open(out_name.c_str());

				if(out_file.is_open())
				{
					print_production = print;

					//Set up internal token list (to be identical to given token list
					for(unsigned int index = 0; index < t_list.size(); ++index)
					{
						received_tokens.push_back(t_list.at(index));
					}

					//Get first lexeme and call first nonterminal in grammar
					simulatedLexerCall();
					rat15S();

					out_file << "Reached EOF with ";

					if(!error)
					{
			            cout << "Finished with no errors." << endl;
			            instr_table.printTable(out_file);
			            sym_table.printTable(out_file);

			            //Print out uninitialized list (if it exists; check done in method)
			            sym_table.printUninitialized(out_file);
					}
					else
					{
						cout << "Finished with errors." << endl;
					}

					cout << "Finished." << endl;
					cout << "\nCheck output file \"" << out_name << "\" for your results, "
							"which is within the same directory as this executable." << endl;

					out_file.close();

				}
				else
				{
					cout << "Error when creating and writing to output file." << endl;
				}
			}
			else
			{
				cout << "No tokens to analyze." << endl;
			}
		}

		/***************************************************************
		 * SyntaxAnalyzer::		emptyLists
		 * ------------------------------------------------------------
		 * Function:
		 *		This function will empty the list and clear them for use
		 ***************************************************************/
		//Clear lists (and reset error flag)
		void emptyLists()
		{
			input.clear();
			received_tokens.clear();
			sym_table.reset();
			instr_table.reset();
			error = false;
		}

	private:

		deque<Token> 	input;            		//use this "stack" to list characters grabbed from token list
		deque<Token> 	received_tokens;  		//for getting the token list from the lexer; should be deque

		SymbolTable 		sym_table;
		InstructionTable 	instr_table;

		bool 		 	print_production;          //production print toggle/switch
		bool 			error;                  //error toggle (to let the user know an error was found -
												// - while analyzing the syntax
		string 			current_lexeme;			//for getting the current lexeme (set by getCurTokenInfo())
		string 			current_token;			//for getting the current token type (set by getCurTokenInfo())
		int 			current_line;           //for getting the current token's line number
		ofstream 		out_file;               //for writing the syntax analysis to an output file

		string temp_qual;

		/***************************************************************
		 * SyntaxAnalyzer::		simulatedLexerCall
		 * ------------------------------------------------------------
		 * Function:
		 *		This will grab a token and it to the stack
		 ***************************************************************/
		void simulatedLexerCall()
		{
			if(!received_tokens.empty())
			{
				//Add to input stack, dequeue received tokens list
				input.push_back(received_tokens.front());
				received_tokens.pop_front();

				//Set new current lexeme and type
				getCurrentTokenInfo();
			}
		}

		/***************************************************************
		 * SyntaxAnalyzer::		errorExpectedThis
		 * ------------------------------------------------------------
		 * Function:
		 *		This is a error messaging function, simply pass in
		 *		the string that is expect and a general error message
		 *		is then output to the outfile. Then the error variable
		 *		will then be turned back to true. Then it will call
		 *		the function simulatedLexerCall to continue the program.
		 ***************************************************************/
		void errorExpectedThis(string expectedToken)
		{
			out_file << "\nERROR: Expected: " << expectedToken << endl;

			out_file << "Found the following: Token \"" << current_token << "\" and Lexeme \"" << current_lexeme
					 << "\" on line " << current_line << ".\nAttempting to continue syntax analysis.\n";

			error = true;

			simulatedLexerCall();
		}

		/***************************************************************
		 * SyntaxAnalyzer::		getCurTokenInfo
		 * ------------------------------------------------------------
		 * Function:
		 *		This function will get the current token's type and
		 *		lexeme while the input stack isn't empty. then prints
		 *		the lexeme to the outfile or outputs a error message
		 ***************************************************************/
		void getCurrentTokenInfo()
		{
			if(!input.empty())
			{
				Token temp 		= input.back();

				current_lexeme 	= temp.getActualValueOfToken();
				current_token 	= temp.getTypeOfToken();
				current_line	= temp.getLineNumber();


				if(print_production) //may be print
				{
					out_file <<"-----------------------------------------------------------" << endl;
					out_file << left << "Token: " << setw(14) << current_token
							 << "Lexeme: " << setw(14) << current_lexeme <<
							 	"Line: " << current_line << endl;
				}
			}
			else
			{
				cout << "Input deque is empty.\n";
			}
		}

//		 /***************************************************************
//		 * SyntaxAnalyzer::		rat15S
//		 * ------------------------------------------------------------
//		 * Function:
//		 *		This function will check for the syntax of what
//		 *		Separator that needs to be in-between scopes of code
//		 *		Mainly the function is check to see if there is a @@
//		 *		token and checks to see if it happens and then if not
//		 *		will output an error message.
//		 ***************************************************************/
//		void rat15S();
//
//		/***************************************************************
//		 * SyntaxAnalyzer::		optDeclarationList
//		 * ------------------------------------------------------------
//		 * Function:
//		 *		This function is a helper function to the function
//		 *		definition function. It will output what is expected
//		 *		and check to see if Function is the token and if it is
//		 *		it will call the functiondefinition() and if not
//		 *		it will report a error message.
//		 ***************************************************************/
//		void optFunctionDefinition();
//
//		/***************************************************************
//		 * SyntaxAnalyzer::		functionDefinition
//		 * ------------------------------------------------------------
//		 * Function:
//		 *		This function will output the expected token if print is
//		 *		turned on. it will then call function() once and if needed
//		 *		it will continue calling it till its wrong.
//		 ***************************************************************/
//		void functionDefinition();
//
//		/***************************************************************
//		 * SyntaxAnalyzer::		function
//		 * ------------------------------------------------------------
//		 * Function:
//		 *		This is the main Function Syntax function, meaning it
//		 *		checks to make sure the function's parameters are
//		 *		meant to standard. it will check for what needs to be
//		 *		in place. if what is expected isn't an error will be called
//		 *		and if it is meant it will grab the next token and do a
//		 *		similar syntax check inside the function in which is called.
//		 ***************************************************************/
//		void function();
//
//		/***************************************************************
//		 * SyntaxAnalyzer::		optParameterList
//		 * ------------------------------------------------------------
//		 * Function:
//		 *		This is the main Function Syntax function, meaning it
//		 *		checks to make sure the function's parameters are
//		 *		meant to standard. it will check for what needs to be
//		 *		in place. if what is expected isn't an error will be called
//		 *		and if it is meant it will grab the next token and do a
//		 *		similar syntax check inside the function in which is called.
//		 ***************************************************************/
//		void optParameterList();
//
//		/***************************************************************
//		 * SyntaxAnalyzer::		ParameterList
//		 * ------------------------------------------------------------
//		 * Function:
//		 *		This function will check for the syntax to a parameter
//		 *		list meaning there must be a parameter then a recursive
//		 *		call back to parameterlist meaning another parameter.
//		 *		also checks to make sure there is a comma in between each.
//		 ***************************************************************/
//		void ParameterList();
//
//		/***************************************************************
//		 * SyntaxAnalyzer::		parameter
//		 * ------------------------------------------------------------
//		 * Function:
//		 *		This function checks to make sure the parameters that
//		 *		are of the current token are correct to syntax standards.
//		 *		if correct a qualifier function should be called. if not
//		 *		it will send an our a corresponding error message.
//		 ***************************************************************/
//		void parameter();
//
//
//		/***************************************************************
//		 * SyntaxAnalyzer::		qualifier
//		 * ------------------------------------------------------------
//		 * Function:
//		 *		This function will check to see if the qualifier is
//		 *		correct in the sense if it is a int or boolean and if
//		 *		it is then grabs the next token. if not it will output
//		 *		a error message.
//		 ***************************************************************/
//		void qualifier();
//
//		/***************************************************************
//		 * SyntaxAnalyzer::		body
//		 * ------------------------------------------------------------
//		 * Function:
//		 *		this function will check to see if there a statement list
//		 *		with the proper enclosing separators. if so it will
//		 *		continue to statementlist() if not will output error.
//		 ***************************************************************/
//		void body();
//
//		/***************************************************************
//		 * SyntaxAnalyzer::		helperDeclarationList
//		 * ------------------------------------------------------------
//		 * Function:
//		 *		This function helps with dealing with declaration list
//		 *		in the sense it does a first check from a type. if correct
//		 *		it will then call declarationlist() if not sends out a
//		 *		corresponding error message.
//		 ***************************************************************/
//		void optDeclarationList();
//
//		/***************************************************************
//		 * SyntaxAnalyzer::		declarationList
//		 * ------------------------------------------------------------
//		 * Function:
//		 *		This function will check to see after the first
//		 *		Initial check in the helperDeclarationList() if the
//		 *		list has the correct syntax meaning checks for correct
//		 *		Terminating symbols and/or if the variables are bool or int,
//		 *		if not will send a correct error list.
//		 ***************************************************************/
//		void declarationList();
//
//		/***************************************************************
//		 * SyntaxAnalyzer::		declaration
//		 * ------------------------------------------------------------
//		 * Function:
//		 *		This function will check to see if the declaration has
//		 *		met standard and is a correct qualifier or id.
//		 ***************************************************************/
//		void declaration();
//
//		/***************************************************************
//		 * SyntaxAnalyzer::		iDs
//		 * ------------------------------------------------------------
//		 * Function:
//		 *		This function will check to see if the current token
//		 *		is correct of an identifier location. if correct
//		 *		it will then grab the next token and check that, if
//		 *		wrong will output a error messgae.
//		 ***************************************************************/
//	void IDs(bool add_to_sym, bool read = false);
//
//
//		/***************************************************************
//		 * SyntaxAnalyzer::		statementList
//		 * ------------------------------------------------------------
//		 * Function:
//		 *		This function will check to see if the current token
//		 *		its a identifier or a keyword to correspond to to a
//		 *		function. it will first try once and then can try again
//		 *		if successful will head to statement()
//		 ***************************************************************/
//		void statementList();
//
//		/***************************************************************
//		 * SyntaxAnalyzer::		statement
//		 * ------------------------------------------------------------
//		 * Function:
//		 *		This function will now call the correct function to the
//		 *		correct keyword to make sure the following token is
//		 *		correct. meaning if 'if' is called it will check the
//		 *		syntax of the next few tokens to make sure its correct.
//		 ***************************************************************/
//		void statement();
//
//		/***************************************************************
//		 * SyntaxAnalyzer::		compound
//		 * ------------------------------------------------------------
//		 * Function:
//		 *		This function will check to see if a { is in place them
//		 *		call the next token into the statement list function
//		 *		and once that is finished to make sure to have } at the
//		 *		end or output which bracket is missing.
//		 ***************************************************************/
//		void compound();
//
//		/***************************************************************
//		 * SyntaxAnalyzer::		assign
//		 * ------------------------------------------------------------
//		 * Function:
//		 *		This function will make sure proper assigning variable
//		 *		sytnax is followed meaning there is a := followed by an
//		 *		expression and leading with a identifier.
//		 ***************************************************************/
//		void assign();
//
//		/***************************************************************
//		 * SyntaxAnalyzer::		If
//		 * ------------------------------------------------------------
//		 * Function:
//		 *		This function will make sure proper If statement
//		 *		sytnax is followed meaning some kind of condition and
//		 *		then a statement or ifRulePrime.
//		 ***************************************************************/
//		void If();
//
//		/***************************************************************
//		 * SyntaxAnalyzer::		ifRulePrime
//		 * ------------------------------------------------------------
//		 * Function:
//		 *		This function is made to deal with left-factorization
//		 *		using the endif statement in between to give a correct
//		 *		and easy way to deal with left-factorization.
//		 ***************************************************************/
//		void ifRulePrime();
//
//		/***************************************************************
//		 * SyntaxAnalyzer::		SAreturn
//		 * ------------------------------------------------------------
//		 * Function:
//		 *		This function is meant to check proper syntax if a
//		 *		return is token is found and how it must be properly
//		 *		Executed.
//		 ***************************************************************/
//		void SAreturn();
//
//		/***************************************************************
//		 * SyntaxAnalyzer::		write
//		 * ------------------------------------------------------------
//		 * Function:
//		 *		This function will check proper syntax of a write function
//		 *		meaning it checks for write then ( and then calls
//		 *		expression function to check a proper expression and then
//		 *		checks to see if it ends with a ).
//		 ***************************************************************/
//		void write();
//
//		/***************************************************************
//		 * SyntaxAnalyzer::		read
//		 * ------------------------------------------------------------
//		 * Function:
//		 *		This function is meant to handle read function calls
//		 *		meaning a token read has been found and now must have a
//		 *		( and call IDs function and then end with a )
//		 ***************************************************************/
//		void read();
//
//		/***************************************************************
//		 * SyntaxAnalyzer::		whileSyntax
//		 * ------------------------------------------------------------
//		 * Function:
//		 *		This function is meant to handle the while syntax,
//		 *		this mean it checks for proper while format, meaning
//		 *		if theres a ( and condition then an end )
//		 ***************************************************************/
//		void whileSyntax();
//
//		/***************************************************************
//		 * SyntaxAnalyzer::		condition
//		 * ------------------------------------------------------------
//		 * Function:
//		 *		a Conditional statement means there is a expression then
//		 *		some kind of relational operator then another expression
//		 *		it will check this by calling each function one after
//		 *		another.
//		 ***************************************************************/
//		void condition();
//
//		/***************************************************************
//		 * SyntaxAnalyzer::		relationalOperator
//		 * ------------------------------------------------------------
//		 * Function:
//		 *		This function will check for proper relational
//		 *		operators and will output what needs to be each time.
//		 ***************************************************************/
//	string relationalOperator();
//
//		/***************************************************************
//		 * SyntaxAnalyzer::		expression
//		 * ------------------------------------------------------------
//		 * Function:
//		 *		This function will check for a proper expression meaning
//		 *		what will be called it uses term() and expressionPrime()
//		 ***************************************************************/
//		void expression();
//
//		/***************************************************************
//		 * SyntaxAnalyzer::		expressionPrime
//		 * ------------------------------------------------------------
//		 * Function:
//		 *		This function is used to handle left hand recursion,
//		 *		meaning it will add and subtract expressions.
//		 ***************************************************************/
//		void expressionPrime();
//
//		/***************************************************************
//		 * SyntaxAnalyzer::		term
//		 * ------------------------------------------------------------
//		 * Function:
//		 *		This function will check proper factor format of a new
//		 *		term meaning it will process a term.
//		 ***************************************************************/
//		void term();
//
//		/***************************************************************
//		 * SyntaxAnalyzer::		termPrime
//		 * ------------------------------------------------------------
//		 * Function:
//		 *		This function is for left hand recursion and will correctly
//		 *		make sure a term has proper syntax. needed because there
//		 *		are small cases that need to be handled.
//		 ***************************************************************/
//		void termPrime();
//
//		/***************************************************************
//		 * SyntaxAnalyzer::		factor
//		 * ------------------------------------------------------------
//		 * Function:
//		 *		This function is used to make sure there is a primary
//		 *		then another primary when factor is called and will
//		 *		call primary() to make sure what is being used is correct
//		 ***************************************************************/
//		void factor();
//
//		/***************************************************************
//		 * SyntaxAnalyzer::		primary
//		 * ------------------------------------------------------------
//		 * Function:
//		 *		This function is used to make sure the token is a
//		 *		identifier or integer, or IDs or expression or real or
//		 *		true or false. and will check to make sure the format is
//		 *		correct per type of primary.
//		 ***************************************************************/
//		void primary();
//
//		/***************************************************************
//		 * SyntaxAnalyzer::		TheListIsEmpty
//		 * ------------------------------------------------------------
//		 * Function:
//		 *		This is a error message output meaning the list or
//		 *		spot is empty or also known as epsilon
//		 ***************************************************************/
//		void TheListIsEmpty();
//
		/**************************************************************************************
		 * Class     		: CPSC 323-02
		 * Assignment		: Programming Assignment 3
		 * Assignment Name  : Syntax Analyzer
		 *
		 * Programmer 1		: Cameron Walters
		 * Programmer 2		: Sina Amini
		 *************************************************************************************/


		#include "SyntaxAnalyzer.h"


		 /***************************************************************
		 * SyntaxAnalyzer::		rat15S
		 * ------------------------------------------------------------
		 * Function:
		 *		This function will check for the syntax of what
		 *		Separator that needs to be in-between scopes of code
		 *		Mainly the function is check to see if there is a @@
		 *		token and checks to see if it happens and then if not
		 *		will output an error message.
		 ***************************************************************/
		void rat15S()
		{
			if(print_production)
			{
				out_file << "<Rat15S> ::= <Opt Function Definitions>   "
							"@@  <Opt Declaration List> @@ <Statement List>  " << endl;
			}

//				optFunctionDefinition();
//				simulatedLexerCall();

				if(current_lexeme == "@@")
				{
					simulatedLexerCall();
					optDeclarationList();

					if(current_lexeme == "@@")
					{
						simulatedLexerCall();
						statementList();
					}
					else
					{
						errorExpectedThis("@@");
					}
				}
				else
				{
					errorExpectedThis("@@");
				}


		};

		/***************************************************************
		 * SyntaxAnalyzer::		helperFunctionDefinition
		 * ------------------------------------------------------------
		 * Function:
		 *		This function is a helper function to the function
		 *		definition function. It will output what is expected
		 *		and check to see if Function is the token and if it is
		 *		it will call the functiondefinition() and if not
		 *		it will report a error message.
		 ***************************************************************/
		void optFunctionDefinition()
		{
			if(print_production)
			{
				out_file << "<Opt Function Definitions> ::= <Function "
							"Definitions> | <Empty>" << endl;
			}

			if(current_lexeme == "function")
			{
				functionDefinition();
			}
			else if(current_lexeme != "@@")
			{
				errorExpectedThis("function or nothing");
			}
			else
			{
				TheListIsEmpty();
			}
		};

		/***************************************************************
		 * SyntaxAnalyzer::		functionDefinition
		 * ------------------------------------------------------------
		 * Function:
		 *		This function will output the expected token if print is
		 *		turned on. it will then call function() once and if needed
		 *		it will continue calling it till its wrong.
		 ***************************************************************/
		void functionDefinition()
		{
			if(print_production)
			{
				out_file << "<Function Definitions>  ::= <Function> "
							"| <Function> <Function Definitions>" << endl;
			}

			//Same as first calling function, then functionDefinition over and over
			do
			{
				function();
			}while(current_lexeme == "function");
		};

		/***************************************************************
		 * SyntaxAnalyzer::		function
		 * ------------------------------------------------------------
		 * Function:
		 *		This is the main Function Syntax function, meaning it
		 *		checks to make sure the function's parameters are
		 *		meant to standard. it will check for what needs to be
		 *		in place. if what is expected isn't an error will be called
		 *		and if it is meant it will grab the next token and do a
		 *		similar syntax check inside the function in which is called.
		 ***************************************************************/
		void function()
		{
			if(print_production)
			{
				out_file << "<Function> ::= function  <Identifier>  "
							"[ <Opt Parameter List> ]   <Opt Declaration List>  <Body>" << endl;
			}

			if(current_lexeme == "function")
			{
				simulatedLexerCall();

				if(current_token == "Identifier")
				{
					simulatedLexerCall();

					if(current_lexeme == "[")
					{
						simulatedLexerCall();
						optParameterList();

						if(current_lexeme == "]")
						{
							simulatedLexerCall();
							optDeclarationList();
							body();
						}
						else
						{
							errorExpectedThis("]");
						}
					}
					else
					{
						errorExpectedThis("[");
					}
				}
				else
				{
					errorExpectedThis("an identifier");
				}
			}
			else
			{
				errorExpectedThis("function");
			}
		};

		/***************************************************************
		 * SyntaxAnalyzer::		ParameterList
		 * ------------------------------------------------------------
		 * Function:
		 *		This function will check for parameter syntax meaning
		 *		it checks for a parameter list or a an empty space
		 *		if found for parameterlist meaning theres an identifier
		 *		it will enter parameterlist() or send out an error.
		 ***************************************************************/
		void optParameterList()
		{
			if(print_production)
			{
				out_file << "<Opt Parameter List> ::=  <Parameter List>"
							"   |  <Empty>" << endl;
			}

			//to-do
			if(current_token == "Identifier")
			{
				ParameterList();
			}
			else if(current_lexeme != "]")
			{
				errorExpectedThis("Identifier");
			}
		};


		/***************************************************************
		 * SyntaxAnalyzer::		ParameterList
		 * ------------------------------------------------------------
		 * Function:
		 *		This function will check for the syntax to a parameter
		 *		list meaning there must be a parameter then a recursive
		 *		call back to parameterlist meaning another parameter.
		 *		also checks to make sure there is a comma in between each.
		 ***************************************************************/
		void ParameterList()
		{
			if(print_production)
			{
				out_file << "<Parameter List>  ::=  <Parameter>  "
							"| <Parameter> , <Parameter List>" << endl;
			}
				//To help terminate do-while loop
				bool hasComma 		= true;

				//Same as calling declaration first, then ',', ParameterList,
				//and looping back if necessary
				do
				{
					parameter();

					if(current_lexeme == ",")
					{
						simulatedLexerCall();
					}
					else
					{
						hasComma = false;
					}

				}while(hasComma);
		};


		/***************************************************************
		 * SyntaxAnalyzer::		parameter
		 * ------------------------------------------------------------
		 * Function:
		 *		This function checks to make sure the parameters that
		 *		are of the current token are correct to syntax standards.
		 *		if correct a qualifier function should be called. if not
		 *		it will send an our a corresponding error message.
		 ***************************************************************/
		void parameter()
		{
			if(print_production)
			{
				out_file << "<Parameter> ::=  <Identifier> : <Qualifier>" << endl;
			}

			if(current_token == "Identifier")
			{
				simulatedLexerCall();

				if(current_lexeme == ":")
				{
					simulatedLexerCall();
					qualifier();
				}
				else
				{
					errorExpectedThis(":");
				}
			}
			else
			{
				errorExpectedThis("identifier");
			}
		};


		/***************************************************************
		 * SyntaxAnalyzer::		qualifier
		 * ------------------------------------------------------------
		 * Function:
		 *		This function will check to see if the qualifier is
		 *		correct in the sense if it is a int or boolean or real
		 *		and if it is then grabs the next token. if not it will
		 *		output a error message.
		 ***************************************************************/
		void qualifier()
		{
			if(print_production)
			{
				out_file << "<Qualifier> ::= int   |  boolean   |  real" << endl;
			}

		//	if(current_lexeme == "int" 		||
		//	   current_lexeme == "boolean" 	||
		//	   current_lexeme == "real"		 )
		//	{
		//		simulatedLexerCall();
		//	}
		//	else
		//	{
		//		errorExpectedThis("int or boolean or real");
		//	}

			  if(current_lexeme == "int")
			  {
			    //completed
			    temp_qual = "int";
			    simulatedLexerCall();
			  }
			  else if(current_lexeme == "boolean")
			  {
			    //completed
			    temp_qual = "boolean";
			    simulatedLexerCall();
			  }
			  else if(current_lexeme == "real")
			  {
			    //completed
			    temp_qual = "real";
			    simulatedLexerCall();
			  }
			  else
			  {
			    //error
				  errorExpectedThis("int or boolean or real");
			  }
		};

		/***************************************************************
		 * SyntaxAnalyzer::		body
		 * ------------------------------------------------------------
		 * Function:
		 *		this function will check to see if there a statement list
		 *		with the proper enclosing separators. if so it will
		 *		continue to statementlist() if not will output error.
		 ***************************************************************/
		void body()
		{
			if(print_production)
			{
				out_file << "<Body>  ::=  {  < Statement List> }" << endl;
			}

			if(current_lexeme == "{")
		    {
				simulatedLexerCall();
				statementList();

				if(current_lexeme == "}")
		    	{
					simulatedLexerCall();
		    	}
				else
				{
					errorExpectedThis("}");
				}
			}
			else
			{
				errorExpectedThis("{");
			}
		};



		/***************************************************************
		 * SyntaxAnalyzer::		optDeclarationList
		 * ------------------------------------------------------------
		 * Function:
		 *		This function helps with dealing with declaration list
		 *		in the sense it does a first check from a type. if correct
		 *		it will then call declarationlist() if not sends out a
		 *		corresponding error message.
		 ***************************************************************/
		void optDeclarationList()
		{
			if(print_production)
			{
				out_file << "<Opt Declaration List> ::= <Declaration List>   "
							"| <Empty>" << endl;
			}

			//to-do
			if(current_lexeme == "int" 		||
			   current_lexeme == "boolean" 	||
			   current_lexeme == "real")
			{
				declarationList();
			}
			else if(current_token == "Unknown")
			{
				errorExpectedThis("int, boolean, real, or nothing");
			}
			else
			{
				TheListIsEmpty();
			}
		};

		/***************************************************************
		 * SyntaxAnalyzer::		declarationList
		 * ------------------------------------------------------------
		 * Function:
		 *		This function will check to see after the first
		 *		Initial check in the helperDeclarationList() if the
		 *		list has the correct syntax meaning checks for correct
		 *		Terminating symbols and/or if the variables are bool or int,
		 *		if not will send a correct error list.
		 ***************************************************************/
		void declarationList()
		{
			if(print_production)
			{
				out_file << "<Declaration List>  := <Declaration> ;  "
							"| <Declaration> ; <Declaration List>" << endl;
			}
			//To help terminate do-while loop
			bool hasSemicolon 	= false;
			bool completed 		= false;

			//Same as calling declaration first, then ;, declarationList,
			//and looping back if necessary
			do
			{
				declaration();


				if(current_lexeme == ";")
				{
					hasSemicolon = true;

					simulatedLexerCall();

					if(current_lexeme != "int" 		&&
					   current_lexeme != "boolean" 	&&
					   current_lexeme != "real")
					{
						completed = true;
					}
				}
				else
				{
					hasSemicolon = false;
				}
			}while(!completed && hasSemicolon);

			if(!hasSemicolon)
			{
				errorExpectedThis(";");
			}

		};

		/***************************************************************
		 * SyntaxAnalyzer::		declaration
		 * ------------------------------------------------------------
		 * Function:
		 *		This function will check to see if the declaration has
		 *		met standard and is a correct qualifier or id.
		 ***************************************************************/
		void declaration()
		{
			if(print_production)
			{
				out_file << "<Declaration> ::=  <Qualifier> <IDs>" << endl;
			}

			qualifier();
			IDs(true,true);

			  //Reset temp_qual when finished
			  temp_qual = "NULL";
		};


		/***************************************************************
		 * SyntaxAnalyzer::		iDs
		 * ------------------------------------------------------------
		 * Function:
		 *		This function will check to see if the current token
		 *		is correct of an identifier location. if correct
		 *		it will then grab the next token and check that, if
		 *		wrong will output a error messgae.
		 ***************************************************************/
		void IDs(bool add_to_sym, bool read)
		{
			if(print_production)
			{
				out_file << "<IDs> ::=  <Identifier>    | <Identifier>, <IDs>" << endl;
			}

		//	//to-do; possible backtrack
		//	bool more_id = false;
		//
		//	//Same as calling identifier, then calling IDs if comma exists
		//	if(current_token == "Identifier")
		//	{
		//		simulatedLexerCall();
		//
		//		do
		//		{
		//			if(current_lexeme == ",")
		//			{
		//				simulatedLexerCall();
		//
		//				if(current_token == "Identifier")
		//				{
		//					simulatedLexerCall();
		//
		//					more_id = true;
		//				}
		//				else
		//				{
		//					errorExpectedThis("an identifier");
		//				}
		//			}
		//			else
		//			{
		//				more_id = false;
		//			}
		//
		//		}while(more_id);
		//	}
		//	else
		//	{
		//		errorExpectedThis("an identifier");
		//	}

			  //boolean to scan for more ids
			  bool more_id = false;

			  //Same as calling ids initially, then calling ids again if comma exists
			  if(current_token == "Identifier")
			  {
			    if(add_to_sym)
			    {
			      //add to symbol table (current lexeme, current qualifier under, and current line
			      sym_table.insert(current_lexeme, temp_qual, current_line, out_file);
			      sym_table.addUnitialized(sym_table.getLastInsertedML());
			    }
			    else if(read) //if called read procedure
			    {
			      //Remove from uninitialized list; error checking done by sym_table while calling methods
			      int addr = sym_table.verifyInstance(current_lexeme, current_line, out_file); //error checking done in find function
			      sym_table.setInitialized(addr);

			      //Object code; pop TOS to ML (previously called STDIN, which pushed stream to TOS)
			      instr_table.generateInstruction("POPM",addr);
			    }
			    else //verify that the instance exists in the symbol table; errors handled by sym_table
			    {
			      sym_table.verifyInstance(current_lexeme, current_line, out_file);
			    }
			    simulatedLexerCall();

			    do
			    {
			      if(current_lexeme == ",")
			      {
			    	  simulatedLexerCall();
			        if(current_token == "Identifier")
			        {
			          if(add_to_sym)
			          {
			            //add to symbol table (current lexeme, current qualifier under, and current line
			            sym_table.insert(current_lexeme, temp_qual, current_line, out_file);
			            sym_table.addUnitialized(sym_table.getLastInsertedML());
			          }
			          else if(read) //if called read procedure
			          {
			            //Set current memory location as initialized; error checking done by sym_table
			            int addr2 = sym_table.verifyInstance(current_lexeme, current_line, out_file); //error checking done in find function
			            sym_table.setInitialized(addr2);

			            //Object code; Push stream onto TOS, then pop stack to address required
			            instr_table.generateInstruction("STDIN",-999);
			            instr_table.generateInstruction("POPM",addr2);

			          }
			          else //verify that the instance exists in the symbol table; errors handled by sym_table
			          {
			            sym_table.verifyInstance(current_lexeme, current_line, out_file);
			          }

			          simulatedLexerCall();
			          more_id = true;
			        }
			        else
			        {
			          //error
			        	errorExpectedThis("an identifier");
			        }
			      }
			      else
			      {
			        more_id = false;
			      }
			    }while(more_id);
			  }
			  else
			  {
			    //error
				  errorExpectedThis("an identifier");
			  }
		};

		/***************************************************************
		 * SyntaxAnalyzer::		statementList
		 * ------------------------------------------------------------
		 * Function:
		 *		This function will check to see if the current token
		 *		its a identifier or a keyword to correspond to to a
		 *		function. it will first try once and then can try again
		 *		if successful will head to statement()
		 ***************************************************************/
		void statementList()
		{
			if(print_production)
			{
				out_file << "<Statement List> ::=  <Statement> "
							"| <Statement> <Statement List>" << endl;
			}


			//Same as calling statement, then (if needed)
			// recalling statement list
			do
			{
				statement();
			}while( current_lexeme == "{" 			||
					current_token  == "Identifier" 	||
					current_lexeme == "if" 			||
					current_lexeme == "return" 		||
					current_lexeme == "write" 		||
					current_lexeme == "read" 		||
					current_lexeme == "while");
		};

		/***************************************************************
		 * SyntaxAnalyzer::		statement
		 * ------------------------------------------------------------
		 * Function:
		 *		This function will now call the correct function to the
		 *		correct keyword to make sure the following token is
		 *		correct. meaning if 'if' is called it will check the
		 *		syntax of the next few tokens to make sure its correct.
		 ***************************************************************/
		void statement()
		{
			if(print_production)
			{
				out_file << "<Statement> ::=  <Compound> | <Assign> | <If> "
							"| <Return> | <Write> | <Read> | <While>" << endl;
			}

			//to-do; a backtrack procedure (?)

			if(current_lexeme 		== "{")
			{
				compound();
			}
			else if(current_token 	== "Identifier")
			{
				assign();
			}
			else if(current_lexeme 	== "if")
			{
				If();
			}
			else if(current_lexeme 	== "return")
			{
				SAreturn();
			}
			else if(current_lexeme 	== "write")
			{
				write();
			}
			else if(current_lexeme 	== "read")
			{
				read();
			}
			else if(current_lexeme 	== "while")
			{
				whileSyntax();
			}
			else
			{
				errorExpectedThis("{, an identifier, if, return, write, read, or while");
			}
		};

		/***************************************************************
		 * SyntaxAnalyzer::		compound
		 * ------------------------------------------------------------
		 * Function:
		 *		This function will check to see if a { is in place them
		 *		call the next token into the statement list function
		 *		and once that is finished to make sure to have } at the
		 *		end or output which bracket is missing.
		 ***************************************************************/
		void compound()
		{
			if(print_production)
			{
				out_file << "<Compound> ::= {  <Statement List>  }" << endl;
			}

			if(current_lexeme == "{")
			{
				simulatedLexerCall();
				statementList();

				if(current_lexeme == "}")
				{
					simulatedLexerCall();
				}
				else
				{
					errorExpectedThis("}");
				}
			}
			else
			{
				errorExpectedThis("{");
			}
		};

		/***************************************************************
		 * SyntaxAnalyzer::		assign
		 * ------------------------------------------------------------
		 * Function:
		 *		This function will make sure proper assigning variable
		 *		sytnax is followed meaning there is a := followed by an
		 *		expression and leading with a identifier.
		 ***************************************************************/
		void assign()
		{
			if(print_production)
			{
				out_file << "<Assign> ::=   <Identifier> := <Expression> ;" << endl;
			}

			if(current_token == "Identifier")
			{
				int save = sym_table.verifyInstance(current_lexeme, current_line, out_file);

				simulatedLexerCall();

				if(current_lexeme == ":=")
				{
					simulatedLexerCall();
					expression();

				      //Insert an instruction based off that address into instr_table
				      instr_table.generateInstruction("POPM", save);

					if(current_lexeme == ";")
					{
						simulatedLexerCall();
					}
					else
					{
						errorExpectedThis(";");
					}
				}
				else
				{
					errorExpectedThis(":=");
				}
			}
			else
			{
				errorExpectedThis("an identifier");
			}
		};

		/***************************************************************
		 * SyntaxAnalyzer::		If
		 * ------------------------------------------------------------
		 * Function:
		 *		This function will make sure proper If statement
		 *		sytnax is followed meaning some kind of condition and
		 *		then a statement or ifRulePrime.
		 ***************************************************************/
		void If()
		{
			if(print_production)
			{
				out_file << "<ifRule> ::= if ( <Condition>  ) "
							"<Statement> <newIfRule>" << endl;
			}

			if(current_lexeme == "if")
			{
				simulatedLexerCall();

				if(current_lexeme == "(")
				{
					simulatedLexerCall();
					condition();

					if(current_lexeme == ")")
					{
						simulatedLexerCall();
						statement();
						ifRulePrime();
					}
					else
					{
						errorExpectedThis(")");
					}
				}
				else
				{
					errorExpectedThis("(");
				}
			}
			else
			{

				errorExpectedThis("if");
			}
		};

		/***************************************************************
		 * SyntaxAnalyzer::		ifRulePrime
		 * ------------------------------------------------------------
		 * Function:
		 *		This function is made to deal with left-factorization
		 *		using the endif statement in between to give a correct
		 *		and easy way to deal with left-factorization.
		 ***************************************************************/
		void ifRulePrime()
		{
			if(print_production)
			{
				out_file << "<newIfRule> ::= endif | else <Statement> endif" << endl;
			}

		//	if(current_lexeme == "endif")
		//	{
		//		simulatedLexerCall();
		//	}
		//	else if(current_lexeme == "else")
		//	{
		//		simulatedLexerCall();
		//		statement();
		//
		//		if(current_lexeme == "endif")
		//		{
		//			simulatedLexerCall();
		//		}
		//		else
		//		{
		//			errorExpectedThis("endif");
		//		}
		//	}
		//	else
		//	{
		//		errorExpectedThis("endif or else");
		//	}
		//
			if(current_lexeme == "endif")
			  {
			    //Object Code; Back patching for if skip (no else)
			    instr_table.backPatch(instr_table.getCurInstrAddress());

			    simulatedLexerCall();

			    //Set up label instruction (if EOF is afterwards)
			    if(current_lexeme == "@@")
			    {
			      instr_table.generateInstruction("LABEL", -999);
			    }
			  }

			  else
			  {
				  errorExpectedThis("endif");
			  }
		};

		/***************************************************************
		 * SyntaxAnalyzer::		SAreturn
		 * ------------------------------------------------------------
		 * Function:
		 *		This function is meant to check proper syntax if a
		 *		return is token is found and how it must be properly
		 *		Executed.
		 ***************************************************************/
		void SAreturn()
		{
			if(print_production)
			{
				out_file << "<Return> ::=  return; | return <Expression> ;" << endl;
			}

			if(current_lexeme == "return")
			{

				simulatedLexerCall();

				if(current_lexeme != ";")
				{
					expression();
				}

				if(current_lexeme == ";")
				{
					simulatedLexerCall();
				}
				else
				{
					errorExpectedThis(";");
				}
			}
			else
			{
				errorExpectedThis("return");
			}
		};

		/***************************************************************
		 * SyntaxAnalyzer::		write
		 * ------------------------------------------------------------
		 * Function:
		 *		This function will check proper syntax of a write function
		 *		meaning it checks for write then ( and then calls
		 *		expression function to check a proper expression and then
		 *		checks to see if it ends with a ).
		 ***************************************************************/
		void write()
		{
			if(print_production)
			{
				out_file << "<Write> ::=   write ( <Expression> );" << endl;
			}

			if(current_lexeme == "write")
			{
				simulatedLexerCall();

				if(current_lexeme == "(")
				{
					simulatedLexerCall();
					expression();

				      //Object code stuff
				      instr_table.generateInstruction("STDOUT", -999);

					if(current_lexeme == ")")
					{
						simulatedLexerCall();

						if(current_lexeme == ";")
						{
							simulatedLexerCall();
						}
						else
						{
							errorExpectedThis(";");
						}
					}
					else
					{
						errorExpectedThis(")");
					}
				}
				else
				{
					errorExpectedThis("(");
				}
			}
			else
			{
				errorExpectedThis("write");
			}
		};

		/***************************************************************
		 * SyntaxAnalyzer::		read
		 * ------------------------------------------------------------
		 * Function:
		 *		This function is meant to handle read function calls
		 *		meaning a token read has been found and now must have a
		 *		( and call IDs function and then end with a )
		 ***************************************************************/
		void read()
		{
			if(print_production)
			{
				out_file << "<Read> ::= read ( <IDs> );" << endl;
			}

			if(current_lexeme == "read")
			{
				simulatedLexerCall();

				if(current_lexeme == "(")
				{
					simulatedLexerCall();

				      //Object code; push stream to TOS
				      instr_table.generateInstruction("STDIN",-999);

					IDs(false, true);

					if(current_lexeme == ")")
					{
						simulatedLexerCall();

						if(current_lexeme == ";")
						{
							simulatedLexerCall();
						}
						else
						{
							errorExpectedThis(";");
						}
					}
					else
					{
						errorExpectedThis(")");
					}
				}
				else
				{
					errorExpectedThis("(");
				}
			}
			else
			{
				errorExpectedThis("read");
			}
		};

		/***************************************************************
		 * SyntaxAnalyzer::		whileSyntax
		 * ------------------------------------------------------------
		 * Function:
		 *		This function is meant to handle the while syntax,
		 *		this mean it checks for proper while format, meaning
		 *		if theres a ( and condition then an end )
		 ***************************************************************/
		void whileSyntax()
		{
			if(print_production)
			{
				out_file << "<While> ::= while ( <Condition>  )  <Statement>" << endl;
			}

			if(current_lexeme == "while")
			{
			    //Object code work
			    int addr = instr_table.getCurInstrAddress();
			    instr_table.generateInstruction("LABEL", -999);

				simulatedLexerCall();

				if(current_lexeme == "(")
				{
					simulatedLexerCall();
					condition();

					if(current_lexeme == ")")
					{
						simulatedLexerCall();
						statement();

				        //Grab instruction address from jump stack
				        instr_table.generateInstruction("JUMP", addr);
				        instr_table.backPatch(instr_table.getCurInstrAddress());
					}
					else
					{
						errorExpectedThis(")");
					}
				}
				else
				{
					errorExpectedThis("(");
				}
			}
			else
			{
				errorExpectedThis("while");
			}
		};

		/***************************************************************
		 * SyntaxAnalyzer::		condition
		 * ------------------------------------------------------------
		 * Function:
		 *		a Conditional statement means there is a expression then
		 *		some kind of relational operator then another expression
		 *		it will check this by calling each function one after
		 *		another.
		 ***************************************************************/
		void condition()
		{
			if(print_production)
			{
				out_file << "<Condition> ::= <Expression>  "
							"<relationalOperator>   <Expression>" << endl;
			}

			expression();
			 string instr = relationalOperator();
			expression();

			  if(instr == "EQU" || instr == "GRT" || instr == "LES")
			  {
				  instr_table.generateInstruction(instr, -999);
				  instr_table.pushJumpStack(instr_table.getCurInstrAddress());
				  instr_table.generateInstruction("JUMPZ", -111);
			  }
			  else if(instr == "GRT-EQU")
			  {
				  instr_table.generateInstruction("GRT", -999);
				  instr_table.pushJumpStack(instr_table.getCurInstrAddress());

				  instr_table.generateInstruction("EQU", -999);
				  instr_table.pushJumpStack(instr_table.getCurInstrAddress());
				  instr_table.generateInstruction("JUMPZ", -111);

			  }
			  else if(instr == "LES-EQU")
			  {
				  instr_table.generateInstruction("LES", -999);
				  instr_table.pushJumpStack(instr_table.getCurInstrAddress());

				  instr_table.generateInstruction("EQU", -999);
				  instr_table.pushJumpStack(instr_table.getCurInstrAddress());
				  instr_table.generateInstruction("JUMPZ", -111);

			  }

		};

		/***************************************************************
		 * SyntaxAnalyzer::		relationalOperator
		 * ------------------------------------------------------------
		 * Function:
		 *		This function will check for proper relational
		 *		operators and will output what needs to be each time.
		 ***************************************************************/
		string relationalOperator()
		{
			string returningString;

			if(print_production)
			{
				out_file << "<relationalOperator> ::=   = | != | > "
							"| < | => | <=" << endl;
			}

		//	if(	current_lexeme == "=" 	||
		//		current_lexeme == "!=" 	||
		//		current_lexeme == ">" 	||
		//		current_lexeme == "<" 	||
		//		current_lexeme == "=>" 	||
		//		current_lexeme == "<=" 	 )
		//	{
		//		simulatedLexerCall();
		//	}
		//	else
		//	{
		//		errorExpectedThis("=, !=, >, <, =>, <=");
		//	}

			if(current_lexeme == "=")
			  {
					//completed
					simulatedLexerCall();
					returningString = "EQU";
			  }
			  else if(current_lexeme == ">")
			  {
					//completed
					simulatedLexerCall();
					returningString = "GRT";
			  }
			  else if(current_lexeme == "<")
			  {
					//completed
					simulatedLexerCall();
					returningString = "LES";
			  }
			  else if(current_lexeme == "<=")
			  {
				  simulatedLexerCall();
				  returningString = "LES-EQU";
			  }
			  else if(current_lexeme == "=>")
			  {
				  simulatedLexerCall();
				  returningString = "GRT-EQU";
			  }
			  else
			  {
					//error
				  	errorExpectedThis("=, !=, >, <, =>, <=");
				  	returningString = "NULL";
			  }

			return returningString;
		};

		/***************************************************************
		 * SyntaxAnalyzer::		expression
		 * ------------------------------------------------------------
		 * Function:
		 *		This function will check for a proper expression meaning
		 *		what will be called it uses term() and expressionPrime()
		 ***************************************************************/
		void expression()
		{
			if(print_production)
			{
				out_file << "<expression> ::= <term> <expressionPrime>" << endl;
			}

			term();
			expressionPrime();
		};

		/***************************************************************
		 * SyntaxAnalyzer::		expressionPrime
		 * ------------------------------------------------------------
		 * Function:
		 *		This function is used to handle left hand recursion,
		 *		meaning it will add and subtract expressions.
		 ***************************************************************/
		void expressionPrime()
		{
			if(print_production)
			{
				out_file << "<expressionPrime>  ::= + <Term> <expressionPrime>"
							"  | - <Term> <expressionPrime>  | <Empty>" << endl;
			}

			if(current_lexeme == "+")
			{
				simulatedLexerCall();
				term();

			    instr_table.generateInstruction("ADD", -999);

				expressionPrime();
			}
			else if (current_lexeme == "-")
			{
				simulatedLexerCall();
				term();

			    instr_table.generateInstruction("SUB", -999);

				expressionPrime();
			}
			else if(current_token == "Unknown")
			{
				errorExpectedThis("+, -, or nothing");
			}
			else
			{
				TheListIsEmpty();
			}
		};

		/***************************************************************
		 * SyntaxAnalyzer::		term
		 * ------------------------------------------------------------
		 * Function:
		 *		This function will check proper factor format of a new
		 *		term meaning it will process a term.
		 ***************************************************************/
		void term()
		{
			if(print_production)
			{
				out_file << "<term> ::= <Factor> <termPrime>" << endl;
			}

			factor();
			termPrime();
		};

		/***************************************************************
		 * SyntaxAnalyzer::		termPrime
		 * ------------------------------------------------------------
		 * Function:
		 *		This function is for left hand recursion and will correctly
		 *		make sure a term has proper syntax. needed because there
		 *		are small cases that need to be handled.
		 ***************************************************************/
		void termPrime()
		{
			if(print_production)
			{
				out_file << "<termPrime> ::= * <Factor> <termPrime> | "
							"/ <Factor> <termPrime> | <Empty>" << endl;
			}

			if(current_lexeme == "*")
			{
				simulatedLexerCall();
				factor();

			    instr_table.generateInstruction("MUL", -999);

				termPrime();
			}
			else if(current_lexeme == "/")
			{
				simulatedLexerCall();
				factor();

			    instr_table.generateInstruction("DIV", -999);

				termPrime();
			}
			else
			{
				TheListIsEmpty();
			}
		};

		/***************************************************************
		 * SyntaxAnalyzer::		factor
		 * ------------------------------------------------------------
		 * Function:
		 *		This function is used to make sure there is a primary
		 *		then another primary when factor is called and will
		 *		call primary() to make sure what is being used is correct
		 ***************************************************************/
		void factor()
		{
			if(print_production)
			{
				out_file << "<Factor> ::= - <Primary> | <Primary>" << endl;
			}

			if(current_lexeme == "-")
			{
				simulatedLexerCall();
				primary();
			}
			else
			{
				primary();
			}
		};

		/***************************************************************
		 * SyntaxAnalyzer::		primary
		 * ------------------------------------------------------------
		 * Function:
		 *		This function is used to make sure the token is a
		 *		identifier or integer, or IDs or expression or real or
		 *		true or false. and will check to make sure the format is
		 *		correct per type of primary.
		 ***************************************************************/
		void primary()
		{
			if(print_production)
			{
				out_file << "<Primary> ::= <Identifier> | <Integer> "
							"| <Identifier> [<IDs>]  | ( <Expression> )  "
							"| <Real> | true | false" << endl;
			}

			if(current_token == "Identifier")
			{
			    //Object code; Sets up address to look for, adds to "initialized" list, then creates the instruction
			    int addr = sym_table.verifyInstance(current_lexeme, current_line, out_file);
			    sym_table.setInitialized(addr);
			    instr_table.generateInstruction("PUSHM", addr);

				simulatedLexerCall();

				if(current_lexeme == "[")
				{
					simulatedLexerCall();
					IDs(false, false);

					if(current_lexeme == "]")
					{
						simulatedLexerCall();
					}
					else
					{
						errorExpectedThis("]");
					}
				}
			}
			else if(current_token == "Integer" ||
					current_token == "Real")
			{

			    instr_table.generateInstruction("PUSHI", atoi(current_lexeme.c_str()));

				simulatedLexerCall();
			}
			else if(current_lexeme == "(")
			{
				simulatedLexerCall();
				expression();

				if(current_lexeme == ")")
				{
					simulatedLexerCall();
				}
				else
				{
					errorExpectedThis(")");
				}
			}
			else if(current_lexeme == "true")
			{
			    //Object code; PUSHI 1 (true)
			    instr_table.generateInstruction("PUSHI", 1);

				simulatedLexerCall();
			}
			else if(current_lexeme == "false")
			{
			    //Object code; PUSHI 1 (true)
			    instr_table.generateInstruction("PUSHI", 0);

				simulatedLexerCall();
			}
			else
			{
				errorExpectedThis("one or more identifiers, an integer, "
									"an expression, real, true, or false");
			}
		};

		/***************************************************************
		 * SyntaxAnalyzer::		TheListIsEmpty
		 * ------------------------------------------------------------
		 * Function:
		 *		This is a error message output meaning the list or
		 *		spot is empty or also known as epsilon
		 ***************************************************************/
		void TheListIsEmpty()
		{
			if(print_production)
			{
				out_file << "<Empty> ::= epsilon" << endl;
			}
		};




};
