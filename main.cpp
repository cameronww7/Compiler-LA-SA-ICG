/**************************************************************************************
 * Class     		: CPSC 323-02
 * Assignment		: Programming Assignment 3
 * Assignment Name  : Syntax Analyzer
 *
 * Programmer 1		: Cameron Walters
 * Programmer 2		: Sina Amini
 *************************************************************************************/

#include <iostream> //for console io
#include <string>   //for using the string datatype

#include "LexicalAnalyzer.h"
#include "SyntaxAnalyzer.h" //For initializing syntax analyzer

using namespace std;

string getFileName();


int main ()
{
	string 		fileName;
	ifstream 	inputFile;	   // Input File

	LexicalAnalyzer currentLexicalAnalyzer;
	SyntaxAnalyzer currentSyntaxAnalyzer;



	fileName = getFileName();

	while(fileName != "exit")
	{
		currentLexicalAnalyzer.setLineNumberBacktoDefault();

		inputFile.open(fileName.c_str());

		if(inputFile)
		{
			currentLexicalAnalyzer.readFile(fileName);
			currentSyntaxAnalyzer.analyzeSyntax(currentLexicalAnalyzer.getTokenList(), fileName);


		}
		else
		{
			cout << "\n\tError >> enter valid file name\n\n";
		}


		//Empty the token list and SA lists for later print-outs
		currentLexicalAnalyzer.emptyTokenList();
		currentSyntaxAnalyzer.emptyLists();

		inputFile.close();
		fileName = getFileName();
	}
	cout << "\nThank you for Using our program, have a Wonderful day\n";
	return 0;
}

/***************************************************************
 * getFileName
 * ------------------------------------------------------------
 * Function:
 * 		This function will get from the user via console, the
 * 		file it needs to be processed, the input file.
 ***************************************************************/
string getFileName()
{
  string input;

  cout << "Enter file name for Tokenize Data file (exit to quit) : ";
  cin >> input;
  cin.ignore(100000, '\n');

  return input;
}
