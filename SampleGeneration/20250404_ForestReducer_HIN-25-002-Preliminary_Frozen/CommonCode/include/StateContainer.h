//---------------------------------------------------------------------------
#ifndef STATECONTAINER_H_6427_FHEAD_JFRNHGKIARFNHSGKNLAGRKIR
#define STATECONTAINER_H_6427_FHEAD_JFRNHGKIARFNHSGKNLAGRKIR
//---------------------------------------------------------------------------
#include <string>
#include <vector>
#include <map>
using namespace std;
//---------------------------------------------------------------------------
#include "DataContainer.h"
//---------------------------------------------------------------------------
class StateContainer;
//---------------------------------------------------------------------------
ostream &operator <<(ostream &out, StateContainer &data);
istream &operator >>(istream &in, StateContainer &data);
//---------------------------------------------------------------------------
class StateContainer
{
   friend class DataHelper;
private:
   map<string, DataContainer> Data;
public:
   StateContainer();
   StateContainer(StateContainer &other);
   StateContainer(const StateContainer &other);
   ~StateContainer();
private:
   void Initialize();
   void CleanUp();
public:
   vector<string> GetListOfKeys();
   DataContainer &operator [](string Key);   // get single object
   void Insert(string Key, DataContainer NewData);
   bool Exist(string Key);
   void Touch(string Key);
   void Delete(string Key);
   string GetRepresentation(string Key);   // get representation of single value
   string GetRepresentation();   // get representation of whole object
   string GetRawRepresentation(string Key);   // get representation of single value
   string GetRawRepresentation();   // get representation of whole object
   void SaveToStream(ostream &out);
   void LoadFromStream(istream &in);
   StateContainer &operator =(const StateContainer &other);
};
//---------------------------------------------------------------------------
StateContainer::StateContainer()
{
}
//---------------------------------------------------------------------------
StateContainer::StateContainer(StateContainer &other)
{
   Data = other.Data;
}
//---------------------------------------------------------------------------
StateContainer::StateContainer(const StateContainer &other)
{
   Data = other.Data;
}
//---------------------------------------------------------------------------
StateContainer::~StateContainer()
{
}
//---------------------------------------------------------------------------
void StateContainer::Initialize()
{
}
//---------------------------------------------------------------------------
void StateContainer::CleanUp()
{
   Data.clear();
}
//---------------------------------------------------------------------------
vector<string> StateContainer::GetListOfKeys()
{
   vector<string> Keys;
   for(map<string, DataContainer>::iterator iter = Data.begin(); iter != Data.end(); iter++)
      Keys.push_back(iter->first);
   
   return Keys;
}
//---------------------------------------------------------------------------
DataContainer &StateContainer::operator [](string Key)
{
   Touch(Key);   
   return Data[Key];
}
//---------------------------------------------------------------------------
void StateContainer::Insert(string Key, DataContainer NewData)
{
   Touch(Key);
   Data[Key] = NewData;
}
//---------------------------------------------------------------------------
bool StateContainer::Exist(string Key)
{
   if(Data.find(Key) == Data.end())
      return false;
   return true;
}
//---------------------------------------------------------------------------
void StateContainer::Touch(string Key)
{
   if(Data.find(Key) != Data.end())
      return;
      
   DataContainer NewData;
   Data.insert(pair<string, DataContainer>(Key, NewData));
}
//---------------------------------------------------------------------------
void StateContainer::Delete(string Key)
{
   if(Data.find(Key) == Data.end())
      return;
   
   Data.erase(Data.find(Key));
}
//---------------------------------------------------------------------------
string StateContainer::GetRepresentation(string Key)
{
   if(Data.find(Key) == Data.end())
      return "DATANOTFOUND";
      
   return Data[Key].GetRepresentation();
}
//---------------------------------------------------------------------------
string StateContainer::GetRepresentation()
{
   bool FirstEntry = true;

   string Representation = "{";
   
   for(map<string, DataContainer>::iterator iter = Data.begin(); iter != Data.end(); iter++)
   {
      if(FirstEntry == true)
         FirstEntry = false;
      else
         Representation = Representation + ", ";
         
      Representation = Representation + "\"" + iter->first + "\": " + iter->second.GetRepresentation();
   }

   Representation = Representation + "}";

   return Representation;
}
//---------------------------------------------------------------------------
string StateContainer::GetRawRepresentation(string Key)
{
   if(Data.find(Key) == Data.end())
      return "DATANOTFOUND";
      
   return Data[Key].GetRawRepresentation();
}
//---------------------------------------------------------------------------
string StateContainer::GetRawRepresentation()
{
   bool FirstEntry = true;

   string Representation = "{";
   
   for(map<string, DataContainer>::iterator iter = Data.begin(); iter != Data.end(); iter++)
   {
      if(FirstEntry == true)
         FirstEntry = false;
      else
         Representation = Representation + ", ";
         
      Representation = Representation + "\"" + iter->first + "\": " + iter->second.GetRawRepresentation();
   }

   Representation = Representation + "}";

   return Representation;
}
//---------------------------------------------------------------------------
void StateContainer::SaveToStream(ostream &out)
{
   char EntryCount[4];
   IntegerToChar4(Data.size(), EntryCount);
   
   out.write(EntryCount, 4);
   
   for(map<string, DataContainer>::iterator iter = Data.begin(); iter != Data.end(); iter++)
   {
      char KeySize[4];
      IntegerToChar4(iter->first.size(), KeySize);

      out.write(KeySize, 4);      
      out.write(iter->first.c_str(), iter->first.size());
      
      iter->second.SaveToStream(out);
   }
}
//---------------------------------------------------------------------------
void StateContainer::LoadFromStream(istream &in)
{
   CleanUp();
   
   char EntryCountChar[4];
   in.read(EntryCountChar, 4);
   int EntryCount = Char4ToInteger(EntryCountChar);
   
   for(int i = 0; i < EntryCount; i++)
   {
      char KeySizeChar[4];
      in.read(KeySizeChar, 4);
      int KeySize = Char4ToInteger(KeySizeChar);

      string Key = "";
      
      char *ch = new char[KeySize+1];
      in.read(ch, KeySize);
      ch[KeySize] = '\0';
      Key = ch;
      delete[] ch;
      
      DataContainer NewData;
      NewData.LoadFromStream(in);
      
      Data.insert(pair<string, DataContainer>(Key, NewData));
   }
}
//---------------------------------------------------------------------------
StateContainer &StateContainer::operator =(const StateContainer &other)
{
   Data = other.Data;
   
   return *this;
}
//---------------------------------------------------------------------------
ostream &operator <<(ostream &out, StateContainer &data)
{
   out << data.GetRepresentation();
   return out;
}
//---------------------------------------------------------------------------
istream &operator >>(istream &in, StateContainer &data)
{
   data.LoadFromStream(in);
   return in;
}
//---------------------------------------------------------------------------
#endif
