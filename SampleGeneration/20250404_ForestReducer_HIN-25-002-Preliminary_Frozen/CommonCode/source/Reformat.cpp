#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

int main(int argc, char *argv[])
{
   if(argc < 2)
   {
      cout << "Usage: Reformat number_of_columns" << endl;
      return -1;
   }

   int Column = 2;
   stringstream str(argv[1]);
   str >> Column;

   vector<string> Temp(Column);
   
   while(cin)
   {
      for(int i = 0; i < Column; i++)
      {
         Temp[i] = "";
         cin >> Temp[i];
      }

      for(int i = 0; i < Column; i++)
      {
         if(i != 0)
            cout << " ";
         cout << Temp[i];
      }
      cout << endl;
   }

   return 0;
}

