//To avoid using multiple definitions (although not really required)
#pragma once

//Official Headers
#include <string>
#include <vector>

using namespace std;

class Instructions
{
  public:

	Instructions(int 	inAddress,
				 string inOperation,
				 int	inOperand = -999)
    {
		  address 	= inAddress;
		  operation = inOperation;
		  operand 	= inOperand;
    }

    ~Instructions()	{}

    int 	getAddress()	{return address;   }
    string 	getOperation()	{return operation; }
    int 	getOperand()	{return operand;   }

    void setAddress		(int    inAddress)		{address 	= inAddress;}
    void setOperation	(string inOperation)	{operation 	= inOperation;}
    void setOperand		(int    inOperand)		{operand 	= inOperand;}

  private:

    int 	address;
    string 	operation;
    int 	operand;
};

class InstructionTable
{
  public:
	InstructionTable()	{instruction_address = 1;}

    ~InstructionTable()	{}

    int 					getCurInstrAddress()	{return instruction_address;}
    vector<Instructions>	getInstrTable()			{return instruction_table;}
    vector<int> 			getJumpStackList()		{return jump_stack;}

    //Generate instruction and add to instr_table
    void generateInstruction(string inOperation, int inOperand)
    {
    	Instructions temp(instruction_address, inOperation, inOperand);
    	instruction_table.push_back(temp);
    	instruction_address++;
    }

    //Pop from jump stack
    int popJumpStack()
    {
		int address = jump_stack.back();
		jump_stack.pop_back();

		return address;
    }

    //Push to jump stack
    void pushJumpStack(int intialAddress)
    {
    	jump_stack.push_back(intialAddress);
    }

    //Back patch methodm
    void backPatch(int jumpToAddress)
    {
		  int address = popJumpStack();
		  instruction_table.at(address-1).setOperand(jumpToAddress);
    }

    //Print instruction table to wherever needed (ostream); starts from 1
    void printTable(ostream& outStream)
    {
		  int temp = 0; //for skipping -999 in the print

		  //Headline portion
		  outStream << "Assembly Code Listing:" << endl
			 << "----------------------" << endl;
		  for(unsigned int index = 0; index < instruction_table.size(); ++index)
		  {
			  outStream << left << setw(6) << instruction_table.at(index).getAddress()
				   << setw(8) << instruction_table.at(index).getOperation();

				temp = instruction_table.at(index).getOperand();

				if(	temp != -999)
				{
					outStream << instruction_table.at(index).getOperand() << endl;
				}
				else
				{
					outStream << endl;
				}
		  }
    }

    //Reset class (for later reuse)
    void reset()
    {
    	instruction_table.clear();
    	jump_stack.clear();
    	instruction_address = 1;
    }

  private:
    	vector<Instructions> 	instruction_table;  //Instruction table
    	vector<int> 			jump_stack;     //Jump_stack list
    	int 					instruction_address;          //Instruction address
};
