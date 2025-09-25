#include <iostream>
using namespace std;

#include "DataHelper.h"

int main(int argc, char *argv[])
{
   if(argc != 2 && argc != 3 && argc != 4)
   {
      cerr << "Usage: " << argv[0] << " DHFile [State [Key]]" << endl;
      return -1;
   }

   DataHelper DHFile(argv[1]);

   if(argc == 2)   // list available states
   {
      cout << "Available states:" << endl;
      for(string S : DHFile.GetListOfKeys())
         cout << "   " << S << endl;
      return 0;
   }

   if(DHFile.Exist(argv[2]) == false)
   {
      cerr << "State \"" << argv[2] << "\" does not exist" << endl;
      return -1;
   }

   if(argc == 4)
   {
      if(DHFile[argv[2]].Exist(argv[3]) == false)
      {
         cerr << "Key \"" << argv[3] << "\" does not exist" << endl;
         return -1;
      }

      cout << DHFile[argv[2]][argv[3]].GetRawRepresentation() << endl;
   }
   if(argc == 3)
   {
      cout << "State: " << argv[2] << endl;
      vector<string> ListOfData = DHFile[argv[2]].GetListOfKeys();
      for(int j = 0; j < (int)ListOfData.size(); j++)
         cout << "   " << ListOfData[j] << " = " << DHFile[argv[2]][ListOfData[j]] << endl;
      cout << endl;
   }

   return 0;
}





