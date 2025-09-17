#include <iostream>
using namespace std;

#include "DataHelper.h"

int main(int argc, char *argv[])
{
   if(argc != 3 && argc != 4)
   {
      cerr << "Usage: " << argv[0] << " DHFile State [Key]" << endl;
      return -1;
   }

   DataHelper DHFile(argv[1]);

   if(argc == 3)
      DHFile.Erase(argv[2]);
   if(argc == 4)
      if(DHFile.Exist(argv[2]))
         DHFile[argv[2]].Delete(argv[3]);

   DHFile.SaveToFile();

   return 0;
}





