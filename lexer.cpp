#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

int check_letter(char c)
{
  if(isalpha(c))
  {
    return 1;
  }
  else if(isdigit(c))
  {
    return 2;
  }
  else if(ispunct(c))
  {
    return 3;
  }
  else
  {
    return 0;
  }
}

void check_state(char c, int &state, string &temp, vector<string> keywords, vector<string> tokens)
{
  switch(state)
  {
    case 2:
      for(int i = 0; i < keywords.size(); i++)
      {
        if(temp == keywords[i])
        {
          cout << fixed << left << setw(15) << tokens[0] << "\t" << temp << endl;
          state = 0;
          temp = "";
          break;
        }
      }
      if(ispunct(c))
      {
        if(c == '$')
        {
          temp += c;
          cout << fixed << left << setw(15) << tokens[1] << "\t" << temp << endl;
          state = 0;
          temp = "";
        }
        else
        {
          cout << fixed << left << setw(15) << tokens[1] << "\t" << temp << endl;
          temp = "";
          state = 4;
          check_state(c, state, temp, keywords, tokens);
        }
      }
      break;
    case 3:
      if(ispunct(c))
      {
        temp = "";
        state = 4;
        check_state(c, state, temp, keywords, tokens);
      }
      else
      {
        cout << fixed << left << setw(15) << tokens[2] << "\t" << temp << endl;
        temp = "";
        state = 0;
      }
      break;
    case 4:
      cout << fixed << left << setw(15) << tokens[3] << "\t" << c << endl;
      state = 0;
      break;
  }
}


void state_change(char c, int &state, string &temp, vector<string> keywords, vector<string> tokens)
{
  int CL = check_letter(c);
  switch(CL)
  {
    case 0:
      if(state != 0) { state_change(c, state, temp, keywords, tokens); }
      break;
    case 1:
      if(state == 0 || state == 2){ temp += c; state = 2; }
      check_state(c, state, temp, keywords, tokens);
      break;
    case 2:
      if(state == 0 || state == 3) { temp += c; state = 3; }
      check_state(c, state, temp, keywords, tokens);
      break;
    case 3:
      if(state == 0 || state == 4) { state = 4; }
      check_state(c, state, temp, keywords, tokens);
  }
}

int main()
{
  char c;
  int state = 0; // Initial state
  bool change_state = false;
  string temp = "";

  vector<string> keywords = {"function", "return", "int"};
  vector<string> tokens = {"Keyword", "Identifier", "Integer", "Separator", "Operator"};

  fstream file;
  file.open("test.txt", ios::in);
  if(file.is_open())
  {
    while(file >> c)
    {
      state_change(c, state, temp, keywords, tokens);
    }
  }
  else
  {
    cerr << "Cannot open test.txt" << endl;
  }
  return 0;
}
