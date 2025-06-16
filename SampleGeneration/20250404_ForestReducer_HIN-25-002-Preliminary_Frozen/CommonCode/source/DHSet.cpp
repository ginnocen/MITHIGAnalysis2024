#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

#include "DataHelper.h"

int main(int argc, char *argv[])
{
   if(argc != 6 && argc != 3)
   {
      cerr << "Usage: " << argv[0] << " DHFile State Key ValueType Value" << endl;
      cerr << "Usage: " << argv[0] << " DHFile ConfigFile" << endl;
      cerr << "   ConfigFile is a 4-column file each containing \"State Key ValueType Value\"" << endl;
      return -1;
   }

   DataHelper DHFile(argv[1]);

   if(argc == 6)
   {
      string ValueType = string(argv[4]);

      if(ValueType == "int" || ValueType == "integer")
         DHFile[argv[2]][argv[3]] = atoll(argv[5]);
      else if(ValueType == "int" || ValueType == "float")
         DHFile[argv[2]][argv[3]] = atof(argv[5]);
      else
         DHFile[argv[2]][argv[3]] = string(argv[5]);
   }
   if(argc == 3)
   {
      ifstream in(argv[2]);
      while(in)
      {
         char ch[10240];
         ch[0] = '\0';

         in.getline(ch, 10239, '\n');
         if(ch[0] == '\0')
            continue;

         stringstream str(ch);
         string State = "", Key = "", ValueType = "", Value;
         str >> State >> Key >> ValueType >> Value;

         if(State == "" || Key == "" || ValueType == "")
            continue;
         if(State[0] == '#')   // comment
            continue;

         if(ValueType == "int" || ValueType == "integer")
            DHFile[State][Key] = stoll(Value);
         else if(ValueType == "int" || ValueType == "float")
            DHFile[State][Key] = stof(Value);
         else
            DHFile[State][Key] = Value;
      }
   }

   DHFile.SaveToFile();

   return 0;
}





