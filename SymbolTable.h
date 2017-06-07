//To avoid using multiple definitions (although not really required)
#pragma once

//Official Headers
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

//Custom Headers
#include "LexicalAnalyzer.h"
#include "Token.h"

using namespace std;

//Global variable (for memory location)
static int memory_address = 1000;

//Symbol class
class Symbol
{
  public:
    //identifier, type, memory location
    Symbol(string inIdentifier, string inQualifier)
    {
		  identifier 	= inIdentifier;
		  qualifier 	= inQualifier;

		  //Increment memory address after assigning it to a Symbol
		  memory_location 	= memory_address;
		  memory_address 	= memory_address + 1;
    }

    ~Symbol()	{}

    //Overloaded Operators
    //Equals operator; Checks if equal (except mem_loc)
    const bool operator== (const Symbol& inSymbol)
    {
    	return ((identifier == inSymbol.qualifier)
    				&& (qualifier == inSymbol.qualifier));
    }

    const string 	getIdentifier()			{ return identifier; }
    const string 	getQualifier()			{ return qualifier; }
    const int 		getMemoryLocation()		{ return memory_location; }


    void printSelf(ostream& outStream)
    {
    	outStream << "Identifier: " << qualifier << "\nQualifier: " << qualifier
         << "\nMemory Location: " << memory_location << endl;
    }

  private:
    string 	identifier;
    string 	qualifier;
    int 	memory_location;  	//memory location (default is -1)

};

//Symbol Table class
class SymbolTable
{
  public:
    SymbolTable()	{ at_least_one_symbol = false; }
    ~SymbolTable()	{}

    //Insertion into sym_table method
    void insert(string inIdentifier, string inQualantifier, int line, ostream& outStream)
    {
		  //If identifier not found in table already
		  if(find(inIdentifier) == -999)
		  {
				//Set up new symbol (to be added to sym_table)
				Symbol temp_Symbol(inIdentifier, inQualantifier);

				//Add to symbol table
				symbol_table.push_back(temp_Symbol);

				//Toggle "at_least_one_symbol" to true
				at_least_one_symbol = true;
		  }
		  else
		  {
			  error_message(inIdentifier, line, "Redeclaration or variable name already in use.", outStream);
		  }
    }

    //To see if the id was declared; Returns memory location if found (else, error message)
    int verifyInstance(string inIdentifier, int line, ostream& outStream)
    {
		  int memoryLocation = find(inIdentifier);

		  if(memoryLocation == -999)
		  {
			error_message(inIdentifier, line, "Undeclared variable.", outStream);
		  }

		  return memoryLocation;
    }

    //Type-checking method; fix to do error handling here
    void typeCheck(string inIdentifier1, string inIdentifier2, string inOperation, int line, ostream& outStream)
    {
		  //Get memory locations; Produces errors if this step fails
		  int firstMemoryLocation 	= find(inIdentifier1);
		  int secondMemoryLocation  = find(inIdentifier2);

		  //Get types of each memory location, and return true if they match (else, false)
		  string firstQualifier 	= symbol_table.at(firstMemoryLocation  - 1000).getQualifier();
		  string secondQualifier 	= symbol_table.at(secondMemoryLocation - 1000).getQualifier();

		  if(firstQualifier != secondQualifier)
		  {
			if(firstQualifier == "boolean"
							&& (inOperation == "+" || inOperation == "-"
								|| inOperation == "/" || inOperation == "*"))
			{
			  error_message(inIdentifier1, line, "Trying to perform arithmetic on boolean.", outStream);
			}
			else if (secondQualifier == "boolean"
									&& (inOperation == "+" || inOperation == "-"
										|| inOperation == "/" || inOperation == "*"))
			{
			  error_message(inIdentifier2, line, "Trying to perform arithmetic on boolean.", outStream);
			}
			else //?
			{
			  error_message(inIdentifier1, line, "Mismatched type on left and right of operation.", outStream);
			}
		  }
		}

		//Scan symbol table for a symbol, returns memory location if found (else, -999)
		//possibly fix to allow stuff like int x; bool x;
		int find(string inIdentifier)
		{
		  for(unsigned int index = 0; index < symbol_table.size(); ++index)
		  {
				if(inIdentifier == symbol_table.at(index).getIdentifier())
				{
				  return symbol_table.at(index).getMemoryLocation();
				}
		  }

		  //If not found, return -999 (doesn't exist)
		  return -999;
    }

    //Add to uninitialized list
    void addUnitialized(int memoryLocation)
    {
    	uninitialized_list.push_back(memoryLocation);
    }

    //Remove from unused list, given memory location (if it exists still in the list)
    void setInitialized(int memoryLocation)
    {
      //If uninitialized list is not empty, scan for the ml to remove from list
      if(!uninitialized_list.empty())
      {
        for(unsigned int index = 0; index < uninitialized_list.size(); ++index)
        {
          //If memory location match, erase position and leave method
          if(memoryLocation == uninitialized_list.at(index))
          {
        	  uninitialized_list.erase(uninitialized_list.begin() + index);
          }
        }
      }
    }

    //Get last added memory location from symbol table
    int getLastInsertedML()
    {
      return symbol_table.back().getMemoryLocation();
    }

    //Print symbol table to wherever needed
    void printTable(ostream& outStream)
    {
		  //Headline portion
    	outStream << "\nSymbol Table:" << endl
			      << "-------------" << endl;

    	outStream << left << setw(14) << "Identifier"
    			  << setw(18) << "Memory Location"
			      << "Type" << endl;

    	outStream << left << setw(14) << "----------"
			      << setw(18) << "---------------"
			      << "----" << endl;

		  //Instances portion
		  for(unsigned int index = 0; index < symbol_table.size(); ++index)
		  {
			outStream << left << setw(14) << symbol_table.at(index).getIdentifier()
					  << setw(18) << symbol_table.at(index).getMemoryLocation()
					  << symbol_table.at(index).getQualifier();

			if(symbol_table.at(index).getQualifier() == "int")
			{
				outStream << "eger";
			}

				outStream << endl;
		  }
    }

    //Print all uninitialized variables (that didn't cause errors)
    void printUninitialized(ostream& outStream)
    {
		  //If not empty, there are some unused variables
		  if(!uninitialized_list.empty())
		  {
			  outStream << "\nWarning: The following are uninitialized variables: " << endl;
			  outStream << left << setw(14) << "Identifier"
					    << setw(18) << "Memory Location"
						<< "Type" << endl;

			  outStream << left << setw(14) << "----------"
					    << setw(18) << "---------------"
						<< "----" << endl;

				//Temporary variables used in loop
				int 	temp_MemoryLocation;
				string 	temp_Identifier;
				string 	temp_Qualifier;

				//Instances portion
				for(unsigned int index = 0; index < uninitialized_list.size(); ++index)
				{
					  //Set up temps to print; -1000 to get index value in sym_table
					temp_MemoryLocation = uninitialized_list.at(index);
					temp_Identifier  	= symbol_table.at(temp_MemoryLocation-1000).getIdentifier();
					temp_Qualifier 		= symbol_table.at(temp_MemoryLocation-1000).getQualifier();

					  outStream << left << setw(14) << temp_Identifier
						 << setw(18) << temp_MemoryLocation
						 << temp_Qualifier;

				  if(temp_Qualifier == "int")
				  {
					  outStream << "eger";
				  }

				  outStream << endl;
				}
		  }
    }

    //Clear each list and reset memory_address (for later reuse)
    void reset()
    {
		  symbol_table.clear();
		  uninitialized_list.clear();
		  memory_address = 1000;
		  at_least_one_symbol = false;
    }

  private:
    vector<Symbol> 	symbol_table;  //symbol table vector (might change to hash table for efficiency)
    vector<int> 	uninitialized_list;   //uninitialized variables list

    bool at_least_one_symbol;  //to make sure the uninitialized variables print works fine

  //Error message (close program)
  void error_message(string inIdentifier, int line, string message, ostream& outStream)
  {
	  outStream << "Error message: \"" << message << "\" with the following identifier: \""
			 << inIdentifier << "\" on line: " << line << endl;

		//End program
		cout << "Unrecoverable error occurred. ";
		cout << "Ending program..." << endl;


		//Exit program with errors
		exit(-999);
  }
};
