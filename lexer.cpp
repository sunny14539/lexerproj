#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// Checks if 'c' is an alphabetical letter, a digit, a punctuation, or none.
int check_letter(char c)
{
	if (isalpha(c))
	{
		return 1;
	}
	else if (isdigit(c))
	{
		return 2;
	}
	else if (ispunct(c))
	{
		return 3;
	}
	else
	{
		return 0;
	}
}

void check_state(char c, int &state, string &temp, vector<string> keywords, vector<string> tokens, ofstream& myfile)
{
   // Checks the state of the FSM
	switch (state)
	{
   // IF the state is 2...
	case 2:
      // ... THEN check to see if 'temp' is a keyword
		for (int i = 0; i < keywords.size(); i++)
		{
         // IF temp is a keyword...
			if (temp == keywords[i])
			{
            // ... THEN output the keyword
				cout << fixed << left << setw(15) << tokens[0] << "\t" << temp << endl;
            // Put the token/lexeme into a file
				myfile << fixed << left << setw(15) << tokens[0] << "\t" << temp << endl;
            // Reset the state to 0
				state = 0;
            // Reset 'temp'
				temp = "";
				break;
			}
		}
      // IF the current character is a punctuation,
      // THEN 'temp' is an identifier
		if (ispunct(c))
		{
         // IF the current character is equal to '$'...
			if (c == '$')
			{
            // ... THEN add the character to 'temp'
				temp.push_back(c);
            // Output the identifier
				cout << fixed << left << setw(15) << tokens[1] << "\t" << temp << endl;
            // Put the token/lexeme into a file
				myfile << fixed << left << setw(15) << tokens[1] << "\t" << temp << endl;
            // Reset the state to 0
				state = 0;
            // Reset 'temp'
				temp = "";
			}
         // ELSE the current character is not equal to '$'
			else
			{
            // Output the seperator
				cout << fixed << left << setw(15) << tokens[1] << "\t" << temp << endl;
            // Put the token/lexeme into a file
				myfile << fixed << left << setw(15) << tokens[1] << "\t" << temp << endl;
            // Reset 'temp'
				temp = "";
            // Change the state to 4
				state = 4;
            // Call the function with the changed state
				check_state(c, state, temp, keywords, tokens, myfile);
			}
		}
		break;
   // IF the state is 3...
	case 3:
      // IF 'c' is a punctuation...
		if (ispunct(c))
		{
         // ... Output the number stored in 'temp'
			cout << fixed << left << setw(15) << tokens[2] << "\t" << temp << endl;
         // Put the token/lexeme into a file
			myfile << fixed << left << setw(15) << tokens[2] << "\t" << temp << endl;
         // Reset 'temp'
			temp = "";
         // Change the state to 4
			state = 4;
         // Call the function with the changed state
			check_state(c, state, temp, keywords, tokens, myfile);
		}
      // IF 'c' is an alphabetical letter...
		if (isalpha(c))
		{
         // ... Output the number stored in 'temp'
			cout << fixed << left << setw(15) << tokens[2] << "\t" << temp << endl;
         // Put the token/lexeme into a file
			myfile << fixed << left << setw(15) << tokens[2] << "\t" << temp << endl;
         // Reset 'temp'
			temp = "";
         // Add the current character to 'temp'
			temp.push_back(c);
         // Change the state to 2
			state = 2;
         // Call the function with the changed state
			check_state(c, state, temp, keywords, tokens, myfile);
		}
		break;
   // IF the state is 4...
	case 4:
      // ... Add the current character to 'temp'
		temp.push_back(c);
		if(c == '%' && temp != "%%")
		{
			return;
		}
      // Create a char vector of operators
		vector<char> operators = { '+', '-', '*', '/', '<', '>', '=' };
		for (int i = 0; i < operators.size(); i++)
		{
         // IF the current character is an operator...
			if (c == operators[i])
			{
            // ... Output the operator stored in 'temp'
				cout << fixed << left << setw(15) << tokens[4] << "\t" << temp << endl;
            // Put the token/lexeme into a file
				myfile << fixed << left << setw(15) << tokens[4] << "\t" << temp << endl;
            // Reset 'temp'
				temp = "";
            // Reset the state to 0
				state = 0;
				return;
			}
		}
      // Output the separator
		cout << fixed << left << setw(15) << tokens[3] << "\t" << temp << endl;
      // Put the token/lexeme into a file
		myfile << fixed << left << setw(15) << tokens[3] << "\t" << temp << endl;
      // Reset 'temp'
		temp = "";
      // Reset the state to 0
		state = 0;
		break;
	}
}

void state_change(char c, int &state, string &temp, vector<string> keywords, vector<string> tokens, ofstream& myfile)
{
   // Call 'check_letter'
	int CL = check_letter(c);
	switch (CL)
	{
   // IF 'c' is a white space
	case 0:
      // IF the state is not 0, call 'state_change'
		if (state != 0) { state_change(c, state, temp, keywords, tokens, myfile); }
		break;
   // IF 'c' is an alphabetical letter
	case 1:
      // IF the state is equal to 0 or 2, add 'c' to temp and change the state to 2
		if (state == 0 || state == 2) { temp.push_back(c); state = 2; }
      // Call 'check_state'
		check_state(c, state, temp, keywords, tokens, myfile);
		break;
   // IF 'c' is a numeric value
	case 2:
      // IF the state is equal to 0 or 3, add 'c' to temp and change the state to 3
		if (state == 0 || state == 3) { temp.push_back(c); state = 3; }
      // Call 'check_state'
		check_state(c, state, temp, keywords, tokens, myfile);
		break;
   // IF 'c' is a punctuation
	case 3:
      // IF the state is equal to 0 or 4, change the state to 4
		if (state == 0 || state == 4) { state = 4; }
      // Call 'check_state'
		check_state(c, state, temp, keywords, tokens, myfile);
		break;
	}
}

int main()
{
	char c;
	int state = 0; // Initial state
	string temp = ""; // Empty string

	string token_display = "Token";
	string lexeme_display = "Lexeme";

	ofstream myfile("output.txt");

	cout << fixed << left << setw(15) << token_display << "\t" << lexeme_display << endl;
	myfile << fixed << left << setw(15) << token_display << "\t" << lexeme_display << endl;
	cout << "============================" << endl;
	myfile << "============================" << endl;

   // Create a string vector of keywords
	vector<string> keywords = { "function", "return", "int", "compound", "assign", "if", "print", "get", "while", "put", "else", "endif" };
   // Create a string vector of the tokens
	vector<string> tokens = { "Keyword", "Identifier", "Integer", "Separator", "Operator" };

	fstream file;
	file.open("test.txt", ios::in);
	if (file.is_open())
	{
		while (file >> c)
		{
         // IF 'c' is equal to '!'...
			if (c == '!')
			{
            // ... then delete all the characters between the '!' and the next '!'...
            // ... because comments are not counted as tokens
				while (file >> c, c != '!') { continue; }
				continue;
			}
         // Call 'state_change'
			state_change(c, state, temp, keywords, tokens, myfile);
		}
		myfile.close();
	}
	else
	{
		cerr << "Cannot open test.txt" << endl;
	}
	return 0;
}
