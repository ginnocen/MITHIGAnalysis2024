//---------------------------------------------------------------------------
#ifndef DATAHELPER_H_6427_FHEAD_JANHIWFWKIGHWKIDAGFBAFKER
#define DATAHELPER_H_6427_FHEAD_JANHIWFWKIGHWKIDAGFBAFKER
//---------------------------------------------------------------------------
#include <string>
#include <cstring>
#include <istream>
#include <ostream>
#include <fstream>
#include <map>
using namespace std;
//---------------------------------------------------------------------------
#include "StateContainer.h"
//---------------------------------------------------------------------------
class DataHelper;
//---------------------------------------------------------------------------
ostream &operator <<(ostream &out, DataHelper &data);
istream &operator >>(istream &in, DataHelper &data);
//---------------------------------------------------------------------------
class DataHelper
{
private:
   map<string, StateContainer> States;
   char ConstantHeader[32];
   string CurrentFileName;
public:
   DataHelper();
   DataHelper(string FileName);
   ~DataHelper(); 
private:
   void Initialize();
   void CleanUp();
public:
   StateContainer &operator [](string Key);
   vector<string> GetListOfKeys();
   bool Exist(string Key);
   void Touch(string Key);
   void Insert(string Key, StateContainer NewState);
   void Erase(string Key);
   string GetRepresentation();
   string GetRepresentation(string Key);
   string GetRepresentation(string Key, string Item);
   string GetRawRepresentation();
   string GetRawRepresentation(string Key);
   string GetRawRepresentation(string Key, string Item);
   DataHelper &operator =(DataHelper &other);
   void LoadFromFile(string FileName);
   void LoadFromFile();
   void SaveToFile(string FileName);
   void SaveToFile();
   void LoadFromStream(istream &in);
   void SaveToStream(ostream &out);
};
//---------------------------------------------------------------------------
DataHelper::DataHelper()
{
   Initialize();
}
//---------------------------------------------------------------------------
DataHelper::DataHelper(string FileName)
{
   Initialize();
   LoadFromFile(FileName);
}
//---------------------------------------------------------------------------
DataHelper::~DataHelper()
{
   CleanUp();
}
//---------------------------------------------------------------------------
void DataHelper::Initialize()
{
   memset(ConstantHeader, '\0', 32);
   strcpy(ConstantHeader, "DataHelperFHead6427");
   CurrentFileName = "";
}
//---------------------------------------------------------------------------
void DataHelper::CleanUp()
{
   States.clear();
}
//---------------------------------------------------------------------------
StateContainer &DataHelper::operator [](string Key)
{
   Touch(Key);
   
   return States[Key];
}
//---------------------------------------------------------------------------
vector<string> DataHelper::GetListOfKeys()
{
   vector<string> Keys;

   for(map<string, StateContainer>::iterator iter = States.begin(); iter != States.end(); iter++)
      Keys.push_back(iter->first);
      
   return Keys;
}
//---------------------------------------------------------------------------
bool DataHelper::Exist(string Key)
{
   if(States.find(Key) == States.end())
      return false;
   return true;
}
//---------------------------------------------------------------------------
void DataHelper::Touch(string Key)
{
   if(States.find(Key) != States.end())
      return;
      
   StateContainer NewState;
   States.insert(pair<string, StateContainer>(Key, NewState));
}
//---------------------------------------------------------------------------
void DataHelper::Insert(string Key, StateContainer NewState)
{
   Touch(Key);
   
   States[Key] = NewState;
}
//---------------------------------------------------------------------------
void DataHelper::Erase(string Key)
{
   if(States.find(Key) == States.end())
      return;
   
   States.erase(States.find(Key));
}
//---------------------------------------------------------------------------
string DataHelper::GetRepresentation()
{
   string Representation = "[";

   bool FirstItem = true;
   
   for(map<string, StateContainer>::iterator iter = States.begin(); iter != States.end(); iter++)
   {
      if(FirstItem == true)
         FirstItem = false;
      else
         Representation = Representation + ", ";
      
      Representation = Representation + "\"" + iter->first + "\" -- " + iter->second.GetRepresentation();
   }

   Representation = Representation + "]";

   return Representation;
}
//---------------------------------------------------------------------------
string DataHelper::GetRepresentation(string Key)
{
   if(States.find(Key) == States.end())
      return "STATENOTFOUND";
   
   return States[Key].GetRepresentation();
}
//---------------------------------------------------------------------------
string DataHelper::GetRepresentation(string Key, string Item)
{
   if(States.find(Key) == States.end())
      return "STATENOTFOUND";
   
   return States[Key].GetRepresentation(Item);
}
//---------------------------------------------------------------------------
string DataHelper::GetRawRepresentation()
{
   string Representation = "[";

   bool FirstItem = true;
   
   for(map<string, StateContainer>::iterator iter = States.begin(); iter != States.end(); iter++)
   {
      if(FirstItem == true)
         FirstItem = false;
      else
         Representation = Representation + ", ";
      
      Representation = Representation + "\"" + iter->first + "\" -- " + iter->second.GetRawRepresentation();
   }

   Representation = Representation + "]";

   return Representation;
}
//---------------------------------------------------------------------------
string DataHelper::GetRawRepresentation(string Key)
{
   if(States.find(Key) == States.end())
      return "STATENOTFOUND";
   
   return States[Key].GetRawRepresentation();
}
//---------------------------------------------------------------------------
string DataHelper::GetRawRepresentation(string Key, string Item)
{
   if(States.find(Key) == States.end())
      return "STATENOTFOUND";
   
   return States[Key].GetRawRepresentation(Item);
}
//---------------------------------------------------------------------------
DataHelper &DataHelper::operator =(DataHelper &other)
{
   States = other.States;
   
   return *this;
}
//---------------------------------------------------------------------------
void DataHelper::LoadFromFile(string FileName)
{
   CurrentFileName = FileName;
   LoadFromFile();
}
//---------------------------------------------------------------------------
void DataHelper::LoadFromFile()
{
   ifstream in(CurrentFileName.c_str());
   LoadFromStream(in);
   in.close();
}
//---------------------------------------------------------------------------
void DataHelper::SaveToFile(string FileName)
{
   CurrentFileName = FileName;
   SaveToFile();
}
//---------------------------------------------------------------------------
void DataHelper::SaveToFile()
{
   ofstream out(CurrentFileName.c_str());
   SaveToStream(out);
   out.close();
}
//---------------------------------------------------------------------------
void DataHelper::LoadFromStream(istream &in)
{
   char Header[33];
   in.read(Header, 32);
   Header[32] = '\0';
   
   if(strcmp(Header, ConstantHeader) != 0)   // wrong file type
      return;
   
   char EntryCountChar[4];
   in.read(EntryCountChar, 4);
   int EntryCount = Char4ToInteger(EntryCountChar);
   
   for(int i = 0; i < EntryCount; i++)
   {
      char KeySizeChar[4] = "";
      in.read(KeySizeChar, 4);
      int KeySize = Char4ToInteger(KeySizeChar);
      
      char *ch = new char[KeySize+1];
      in.read(ch, KeySize);
      ch[KeySize] = '\0';
      
      string Key = ch;
      delete[] ch;
      
      StateContainer NewState;
      NewState.LoadFromStream(in);
      
      States.insert(pair<string, StateContainer>(Key, NewState));
   }
}
//---------------------------------------------------------------------------
void DataHelper::SaveToStream(ostream &out)
{
   out.write(ConstantHeader, 32);
   
   char EntryCount[4];
   IntegerToChar4(States.size(), EntryCount);
   out.write(EntryCount, 4);
   
   for(map<string, StateContainer>::iterator iter = States.begin(); iter != States.end(); iter++)
   {
      char KeySize[4];
      IntegerToChar4(iter->first.size(), KeySize);

      out.write(KeySize, 4);
      out.write(iter->first.c_str(), iter->first.size());

      iter->second.SaveToStream(out);
   }
}
//---------------------------------------------------------------------------
ostream &operator <<(ostream &out, DataHelper &data)
{
   out << data.GetRepresentation();
   return out;
}
//---------------------------------------------------------------------------
istream &operator >>(istream &in, DataHelper &data)
{
   data.LoadFromStream(in);
   return in;
}
//---------------------------------------------------------------------------
#endif

