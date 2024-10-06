//// interpreter for simplified infix expression with {+, -, *, / } operations;
//// keyboard input, single digit numbers only and no spaces are allowed;
//// compile: $> g++ prog1.cpp
//// run with: 2+3*4/2+3+4*2
#include <cstdlib> //for atoi()
#include <iostream>

#include <list>

#include <string>
#include <vector>
#include <fstream> // file reading
#include <unordered_map>


using namespace std;

// using file input

int exp(), term(), fact(), exp2(int), term2(int), fact2(int), Num();

//int word(string);

void wordFound(string);

void Declarations(), Declaration(string);

void Statements(), Statement(string);

void print_st(), assign_st(string);


string myWord();
int myChar();


string prog, tempString;
//string curWord; //string for reading 1-line input expression (program)


int indexx = 0; //global index for program string
int tableIn = 0;


// SYMBOL TABLE	//
const int mySize = 100;
struct symbolTable
{
	char id;
	string type;
	int val;

	symbolTable()	// Constructor for our symbol table
	{
		// Defaults
		id = '0';
		type = "EMPTY";
		val = -1;
	}

};
unordered_map<int, symbolTable> myTable;


/////***************************/
////
////void fileReading()
////{
////	string line;
////	ifstream myfile;
////	myfile.open("myProg.txt");
////
////	while (getline(myfile, line)) {
////		prog += line + " ";	// This allows us to seperate program instructions by spaces to make it readable with code
////
////	}
////
////	myfile.close();
////}
////
//////void nextWord(istringstream iss(string));

ifstream myfile;






int main(int argc, const char** argv)
{

	myfile.open("myProg.txt");
	//getline(myfile, s1);
	while (!myfile.eof())
	{
		while (getline(myfile, tempString))	// change how it is interpreted, not input file
		{
			prog += tempString + ' '; // fix this
		}
		prog.pop_back();
		////getline(myfile, tempString);   //read one line  
		////while (!myfile.eof())
		////{
		////	prog += tempString;
		////	getline(myfile, tempString); //read next line
		////}


		// something line myWord(string input), reads word and returns// update prog each iteration?
		// getline(myfile, prog)
		// curWord = myWord;
		// wholeProg += prog;
		// getline(myfile, prog)
		string curWord = myWord();
		if (curWord == "program")
		{
			Declarations();
			Statements();
		}
		else
		{
			cout << "Error" << endl; // Make proper error
			exit(1);
		}

		myfile.close();

	}

}









// exp() returns value from exp2(term())
int exp()		// exp: tail-end recursion to call our non-terminals
{

	return exp2(term());	// if exp is called, we goto exp2 with our term
}

// term() returns value from term2(fact())
int term() 	// term: using tail-end recursion to call non-terminals
{

	return term2(fact()); // term calls fact() -> fact2() -> term2()
}

int fact()
{

	return fact2(Num());
}

// using input 'inp' we make our local variable 'result' be 'inp'
// if the current indexx is less than our prog.length then it is not at the end of string
// we get a single char from program string and make it local variable char 'a'
// if 'a' is '+' we have 'result' value change to exp2(result + term())
// if 'a' is '-' we have 'result' value change to exp2(result - term())
// after all the calls, the 'result' is returned

int exp2(int inp)		// implements right-recursive form to get our 'inp' by
{						// calling exp2 either adding or subtracting our result and our 'term'

	int result = inp;
	if (indexx < prog.length()) //if not the end of program string
	{
		char a = prog.at(indexx++); //get one chr from program string

		while (a == ' ' && (indexx < prog.length()))
		{
			a = prog.at(indexx++);
		}

		if (a == '+')
			result = exp2(result + term()); //handles t+t
		else if (a == '-')
			result = exp2(result - term()); //handles t-t

	}
	return result;
}


// using input 'inp' we make it a local variable 'result' be 'inp'
// if our 'indexx' is less than the length of 'prog' then it is not the end of string, so we continue
// we then get char 'a' to be a single char from the string
// if 'a' is '*' we have 'result' value change to term2(result * term())
// if 'a' is '/' we have 'result' value change to term2(result / term())
// if 'a' is either '+' or '-' then we go back one position
// after all these we return our result

int term2(int inp)
{

	int result = inp;
	if (indexx < prog.length()) //if not the end of program string
	{
		char a = prog.at(indexx++); //get one chr from program string

		while (a == ' ' && (indexx < prog.length()))
		{
			a = prog.at(indexx++);
		}

		if (a == '*')
			result = term2(result * fact()); //handles consecutive * operators
		else if (a == '/')
			result = term2(result / fact()); //handles consecutive / operators
		else if (a == ')' || a == '+' || a == '-' || a == ';') //if + or -, get back one position
			indexx--;

	}
	return result;
}

// fact2() does our power, if our current character is a '^', we make our result
// be the base to our next character, and so forth, nested so it can recursively check
// for the upcoming characters
int fact2(int inp)
{
	int result = inp;
	if (indexx < prog.length())
	{
		char a = prog.at(indexx++);

		while (a == ' ' && (indexx < prog.length()))
		{
			a = prog.at(indexx++);
		}

		if (a == '^')
		{
			result = fact2(pow(result, fact()));  // Correctly handle right-associative
		}
		else
			indexx--;  // Step back if not '^'

	}
	return result;
}


int Num()
{
	if (indexx < prog.length())
	{
		char a = prog.at(indexx++);
		while (a == ' ' && (indexx < prog.length()))
		{
			a = prog.at(indexx++);
		}

		if (a == '(')			// if left parantheses at indexx
		{
			return exp();
		}

		if (isdigit(a))
		{

			int num = a - '0';  // using ascii values to get num

			while (indexx < prog.length() && isdigit(prog.at(indexx)))
			{

				a = prog.at(indexx++);	// moving to next char
				num = num * 10 + (a - '0');  // Multiplying our initial num by 10 to move numbers place
				// as well as getting ascii value of next char
				//cout << num << endl;

			}

			return num;  // Return the multi-digit number
		}

		return atoi(&a);

	} // End of indexx < prog.length()





}


void Declarations()
{
	if (indexx < prog.length())
	{
		string curWord = myWord();	// reads next word
		if (curWord == "begin")
		{
			return; // was exit(1)
		}
		else if (curWord == "int" || curWord == "double")
		{
			Declaration(curWord);
		}

		Declarations();
	}
}

void Declaration(string inpWord)
{
	if (indexx < prog.length())
	{
		string varType = inpWord;

		char curChar = prog.at(indexx++);
		while (curChar == ' ' && (indexx < prog.length()))
		{
			curChar = prog.at(indexx++);
		}

		myTable[tableIn].id = curChar;	// make the id the current char, in this case ID
		myTable[tableIn].type = varType;	// the var type of this char, is our input word
		tableIn++;

		curChar = prog.at(indexx++);				// iterates to next character
		while (curChar == ' ' && (indexx < prog.length())) // skips spaces
		{
			curChar = prog.at(indexx++);
		}
		if (curChar == ',')	// if next character is a comma, recursively call declaration
		{
			Declaration(varType);

		}
		else if (curChar == ';')	// if next character a semicolon, exit
		{
			return; // was exit(1)
		}
	}
}



void Statements()
{
	if (indexx < prog.length())
	{
		string curWord = myWord();
		if (curWord == "end")
		{
			return;
		}
		else
			Statement(curWord);

		Statements();
	}
}


void Statement(string inpStr)
{
	if (indexx < prog.length())
	{
		string curWord = inpStr;
		if (curWord == "print")
		{
			print_st();
		}
		else
		{
			assign_st(curWord);
		}
	}
}

void print_st()
{
	if (indexx < prog.length())
	{
		int tempIND;
		char myID = prog.at(indexx++);
		while (myID == ' ' && (indexx < prog.length()))
		{
			myID = prog.at(indexx++);
		}

		if (isalpha(myID))
		{
			char nextCh = prog.at(indexx++);
			while (nextCh == ' ' && (indexx < prog.length()))
			{
				nextCh = prog.at(indexx++);
			}

			if (nextCh == ';')
			{
				for (auto itr = myTable.begin(); itr != myTable.end(); itr++)
				{

					if (itr->second.id == myID)
					{
						tempIND = itr->first; // Gets index of value
						//itr->second.val = temp;
					}


				}

				if (myTable[tempIND].id == myID)
				{
					cout << myTable[tempIND].val << endl;
				}
				else
				{
					cout << "**** SEMANTIC ERROR ****" << endl;
				}
			}
			else
			{
				cout << "**** SEMANTIC ERROR ****" << endl;
			}

		}
		else
		{
			indexx--;
			int expRes = exp();
			cout << expRes << endl;

		}
	}
}
/**** ^ Works ^ ****/

/**** issues here *****/
void assign_st(string inpID)
{
	if (indexx < prog.length())
	{

		char myID = inpID.at(0); // input id
		int idIND;
		int nextIND;


		char curChar = prog.at(indexx++);
		while (curChar == ' ' && (indexx < prog.length()))
		{
			curChar = prog.at(indexx++);
		}

		if (curChar == '=') // Handles if char is =
		{

			char nextCh = prog.at(indexx++);
			while (nextCh == ' ' && (indexx < prog.length()))
			{
				nextCh = prog.at(indexx++);
			}

			if (isalpha(nextCh))	// checks to see if nextCh is an id(char)
			{
				for (auto itr = myTable.begin(); itr != myTable.end(); itr++)
				{
					if (itr->second.id == myID)	// searches for index of left side ID
					{
						idIND = itr->first; // Gets index of value
						//itr->second.val = temp;
					}
					if (itr->second.id == nextCh)	// searches for a matching ID in the symbol table for right side
					{
						nextIND = itr->first;
					}


				}

				if (myTable[nextIND].id == nextCh)
				{
					myTable[idIND].val = myTable[nextIND].val;

				}
				else
				{
					cout << "**** SEMANTIC ERROR ****" << endl;
				}
			}

			else
			{
				indexx--;
				int expRes = exp();

				for (auto itr = myTable.begin(); itr != myTable.end(); itr++)
				{

					if (itr->second.id == myID)	// searches for a matching id using iterators
					{
						idIND = itr->first; // Gets index of value
						//itr->second.val = temp;
					}

				}

				if (myTable[idIND].id == myID)	// using found index, the result of exp
				{
					myTable[idIND].val = expRes;		// is made that id's value
				}
				else
				{
					cout << "**** SEMANTIC ERROR ****" << endl;
				}
			}
		}

		else
		{
			cout << "**** SYNTAX ERROR ****" << endl;
		}
	}
}




string myWord()
{

	if (indexx < prog.length()) // local
	{

		string tempString;

		while ((prog.at(indexx) == ' ' || prog.at(indexx) == ';') && (indexx < prog.length()))
		{

			indexx++;
		}

		while (indexx < prog.length() && isalpha(prog.at(indexx)))
		{

			tempString += prog.at(indexx++);  // Multiplying our initial num by 10 to move numbers place
			//cout << num << endl;

		}

		return tempString;
	}
}



int myChar()
{

	char a = prog.at(indexx++); //get one chr from program string
	while (a == ' ' && (indexx < prog.length()))
	{
		a = prog.at(indexx++);
	}
	return atoi(&a); //converts a char to a numeric number and return


}

/****************************************************************/