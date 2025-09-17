#include <iostream>
#include <vector>
#include <string>
using namespace std;

#include "DataHelper.h"

int main(int argc, char *argv[])
{
   if(argc == 1)
   {
      cout << "Usage: " << argv[0] << " DHFile" << endl;
      return -1;
   }

   DataHelper data(argv[1]);

   vector<string> ListOfStates = data.GetListOfKeys();

   for(int i = 0; i < (int)ListOfStates.size(); i++)
   {
      cout << "State: " << ListOfStates[i] << endl;

      vector<string> ListOfData = data[ListOfStates[i]].GetListOfKeys();
      for(int j = 0; j < (int)ListOfData.size(); j++)
         cout << "   " << ListOfData[j] << " = " << data[ListOfStates[i]][ListOfData[j]] << endl;

      cout << endl;
   }

   return 0;
}






