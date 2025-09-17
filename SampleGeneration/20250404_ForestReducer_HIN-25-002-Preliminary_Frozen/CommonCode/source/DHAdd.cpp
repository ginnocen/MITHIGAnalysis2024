#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include "DataHelper.h"

int main(int argc, char *argv[])
{
   if(argc < 3)
   {
      cerr << "Usage: " << argv[0] << " TargetFile SourceFile1 SourceFile2 ..." << endl;
      return -1;
   }

   cout << "Target file: " << argv[1] << endl;

   DataHelper DHFileTarget(argv[1]);

   for(int iF = 2; iF < argc; iF++)
   {
      cout << "Source file " << iF - 1 << ": " << argv[iF] << endl;
      
      DataHelper DHFile(argv[iF]);

      vector<string> States = DHFile.GetListOfKeys();

      for(int iS = 0; iS < (int)States.size(); iS++)
      {
         vector<string> Keys = DHFile[States[iS]].GetListOfKeys();

         for(int iK = 0; iK < (int)Keys.size(); iK++)
            DHFileTarget[States[iS]].Insert(Keys[iK], DHFile[States[iS]][Keys[iK]]);
      }
   }

   DHFileTarget.SaveToFile(argv[1]);

   return 0;
}





