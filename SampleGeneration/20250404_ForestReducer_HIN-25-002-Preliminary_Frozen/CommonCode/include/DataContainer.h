//---------------------------------------------------------------------------
#ifndef DATACONTAINER_H_6427_FHEAD_AJGKFVNGERINHGANGKFKAKNARGN
#define DATACONTAINER_H_6427_FHEAD_AJGKFVNGERINHGANGKFKAKNARGN
//---------------------------------------------------------------------------
#include <string>
#include <istream>
#include <ostream>
#include <sstream>
#include <algorithm>
using namespace std;
//---------------------------------------------------------------------------
class DataContainer;
union DataMessenger;
//---------------------------------------------------------------------------
ostream &operator <<(ostream &out, DataContainer data);
istream &operator >>(istream &in, DataContainer &data);
//---------------------------------------------------------------------------
void IntegerToChar4(int Number, char ChNumber[4]);
long long Char4ToInteger(char ChNumber[4]);
//---------------------------------------------------------------------------
class DataContainer
{
   friend class DataHelper;
   friend class StateContainer;
public:
   enum {DataTypeNone, DataTypeString, DataTypeDouble, DataTypeInteger};
private:
   string StringValue;
   double DoubleValue;
   long long IntegerValue;
   int Type;
public:
   DataContainer();
   DataContainer(string value);
   DataContainer(double value);
   DataContainer(long long value);
   // DataContainer(DataContainer &other);
   // DataContainer(const DataContainer &other);
   ~DataContainer();
   long long GetInteger();
   double GetDouble();
   string GetString();
   string GetRepresentation();
   string GetRawRepresentation();
   int GetType();
   DataContainer &operator =(string value);
   DataContainer &operator =(double value);
   DataContainer &operator =(int value);
   DataContainer &operator =(long long value);
   // DataContainer &operator =(DataContainer &other);
   bool operator <(string value) const { return *this < DataContainer(value); }
   bool operator <(double value) const { return *this < DataContainer(value); }
   bool operator <(long long value) const { return *this < DataContainer(value); }
   bool operator <(const DataContainer &other) const;
   bool operator <=(string value) const { return *this <= DataContainer(value); }
   bool operator <=(double value) const { return *this <= DataContainer(value); }
   bool operator <=(long long value) const { return *this <= DataContainer(value); }
   bool operator <=(const DataContainer &other) const { return *this < other || *this == other; }
   bool operator >(string value) const { return *this > DataContainer(value); }
   bool operator >(double value) const { return *this > DataContainer(value); }
   bool operator >(long long value) const { return *this > DataContainer(value); }
   bool operator >(const DataContainer &other) const { return *this >= other && *this != other; }
   bool operator >=(string value) const { return *this >= DataContainer(value); }
   bool operator >=(double value) const { return *this >= DataContainer(value); }
   bool operator >=(long long value) const { return *this >= DataContainer(value); }
   bool operator >=(const DataContainer &other) const { return !(*this < other); }
   bool operator ==(string value) const { return *this == DataContainer(value); }
   bool operator ==(double value) const { return *this == DataContainer(value); }
   bool operator ==(long long value) const { return *this == DataContainer(value); }
   bool operator ==(const DataContainer &other) const;
   bool operator !=(string value) const { return *this != DataContainer(value); }
   bool operator !=(double value) const { return *this != DataContainer(value); }
   bool operator !=(long long value) const { return *this != DataContainer(value); }
   bool operator !=(const DataContainer &other) const { return !(*this == other); }
   DataContainer operator +(const DataContainer &other);   // none: nothing, string: concatenation, numbers: addition
   DataContainer operator -(const DataContainer &other);   // none: nothing, string: nothing, numbers: subtraction
   DataContainer operator *(const DataContainer &other);   // none: nothing, string * string: concatenation, string * number: repetition, numbers: multiplication
   DataContainer operator /(const DataContainer &other);   // none: nothing, string: nothing, numbers: division
   DataContainer operator -();   // none: nothing, string: reverse, numbers: negation
   void SaveToStream(ostream &out);
   void LoadFromStream(istream &in);
};
//---------------------------------------------------------------------------
union DataMessenger
{
   char CharValue[20];   // 20 bytes should be enough for all numbers?
   double DoubleValue;
   long long LongLongValue;
   int IntegerValue;
};
//---------------------------------------------------------------------------
DataContainer::DataContainer()
{
   StringValue = "";
   IntegerValue = 0;
   DoubleValue = 0;
   Type = DataTypeNone;
}
//---------------------------------------------------------------------------
DataContainer::DataContainer(string value)
{
   StringValue = value;
   IntegerValue = 0;
   DoubleValue = 0;
   Type = DataTypeString;
}
//---------------------------------------------------------------------------
DataContainer::DataContainer(double value)
{
   StringValue = "";
   IntegerValue = 0;
   DoubleValue = value;
   Type = DataTypeDouble;
}
//---------------------------------------------------------------------------
DataContainer::DataContainer(long long value)
{
   StringValue = "";
   IntegerValue = value;
   DoubleValue = 0;
   Type = DataTypeInteger;
}
//---------------------------------------------------------------------------
/*
DataContainer::DataContainer(DataContainer &other)
{
   Type = other.Type;
   StringValue = other.StringValue;
   DoubleValue = other.DoubleValue;
   IntegerValue = other.IntegerValue;
}
*/
//---------------------------------------------------------------------------
/*
DataContainer::DataContainer(const DataContainer &other)
{
   Type = other.Type;
   StringValue = other.StringValue;
   DoubleValue = other.DoubleValue;
   IntegerValue = other.IntegerValue;
}
*/
//---------------------------------------------------------------------------
DataContainer::~DataContainer()
{
}
//---------------------------------------------------------------------------
long long DataContainer::GetInteger()
{
   return IntegerValue;
}
//---------------------------------------------------------------------------
double DataContainer::GetDouble()
{
   return DoubleValue;
}
//---------------------------------------------------------------------------
string DataContainer::GetString()
{
   return StringValue;
}
//---------------------------------------------------------------------------
string DataContainer::GetRepresentation()
{
   if(Type == DataTypeNone)
      return "NONE";
   else if(Type == DataTypeString)
      return "\"" + StringValue + "\"";
   else if(Type == DataTypeDouble)
   {
      stringstream str;
      str << DoubleValue;
      return str.str();
   }
   else if(Type == DataTypeInteger)
   {
      stringstream str;
      str << IntegerValue;
      return str.str();
   }
   
   return "";
}
//---------------------------------------------------------------------------
string DataContainer::GetRawRepresentation()
{
   if(Type == DataTypeNone)
      return "NONE";
   else if(Type == DataTypeString)
      return StringValue;
   else if(Type == DataTypeDouble)
   {
      stringstream str;
      str << DoubleValue;
      return str.str();
   }
   else if(Type == DataTypeInteger)
   {
      stringstream str;
      str << IntegerValue;
      return str.str();
   }
   
   return "";
}
//---------------------------------------------------------------------------
int DataContainer::GetType()
{
   return Type;
}
//---------------------------------------------------------------------------
DataContainer &DataContainer::operator =(string value)
{
   StringValue = value;
   Type = DataTypeString;

   return *this;
}
//---------------------------------------------------------------------------
DataContainer &DataContainer::operator =(double value)
{
   DoubleValue = value;
   Type = DataTypeDouble;

   return *this;
}
//---------------------------------------------------------------------------
DataContainer &DataContainer::operator =(int value)
{
   IntegerValue = value;
   Type = DataTypeInteger;

   return *this;
}
//---------------------------------------------------------------------------
DataContainer &DataContainer::operator =(long long value)
{
   IntegerValue = value;
   Type = DataTypeInteger;
   
   return *this;
}
//---------------------------------------------------------------------------
/*
DataContainer &DataContainer::operator =(DataContainer &other)
{
   Type = other.Type;
   StringValue = other.StringValue;
   DoubleValue = other.DoubleValue;
   IntegerValue = other.IntegerValue;
   return *this;
}
*/
//---------------------------------------------------------------------------
bool DataContainer::operator <(const DataContainer &other) const
{
   // type hierarchy: None, String, Numbers
   
   // get done with the "None"'s
   if(Type == DataTypeNone && other.Type != DataTypeNone)
      return true;
   if(Type == DataTypeNone && other.Type == DataTypeNone)
      return false;
   if(Type != DataTypeNone && other.Type == DataTypeNone)
      return false;
   
   // get done with strings
   if(Type == DataTypeString && other.Type != DataTypeString)
      return true;
   if(Type != DataTypeString && other.Type == DataTypeString)
      return false;
   if(Type == DataTypeString && other.Type == DataTypeString)
   {
      if(StringValue < other.StringValue)
         return true;
      if(StringValue > other.StringValue)
         return false;
   }
   
   // compare numbers
   if(Type == DataTypeDouble && other.Type == DataTypeDouble)
   {
      if(DoubleValue < other.DoubleValue)
         return true;
      if(DoubleValue > other.DoubleValue)
         return false;
   }
   if(Type == DataTypeInteger && other.Type == DataTypeDouble)
   {
      if(IntegerValue < other.DoubleValue)
         return true;
      if(IntegerValue > other.DoubleValue)
         return false;
   }
   if(Type == DataTypeDouble && other.Type == DataTypeInteger)
   {
      if(DoubleValue < other.IntegerValue)
         return true;
      if(DoubleValue > other.IntegerValue)
         return false;
   }
   if(Type == DataTypeInteger && other.Type == DataTypeInteger)
   {
      if(IntegerValue < other.IntegerValue)
         return true;
      if(IntegerValue > other.IntegerValue)
         return false;
   }
   
   return false;
}
//---------------------------------------------------------------------------
bool DataContainer::operator ==(const DataContainer &other) const
{
   if(Type == DataTypeNone && other.Type != DataTypeNone)
      return false;
   if(Type == DataTypeString && other.Type != DataTypeString)
      return false;
   if((Type == DataTypeDouble || Type == DataTypeInteger) && (other.Type != DataTypeDouble && other.Type != DataTypeInteger))
      return false;
   
   if(Type == DataTypeString && StringValue != other.StringValue)
      return false;
   if(Type == DataTypeDouble && other.Type == DataTypeDouble && DoubleValue != other.DoubleValue)
      return false;
   if(Type == DataTypeInteger && other.Type == DataTypeDouble && IntegerValue != other.DoubleValue)
      return false;
   if(Type == DataTypeDouble && other.Type == DataTypeInteger && DoubleValue != other.IntegerValue)
      return false;
   if(Type == DataTypeInteger && other.Type == DataTypeInteger && IntegerValue != other.IntegerValue)
      return false;
   
   return true;
}
//---------------------------------------------------------------------------
DataContainer DataContainer::operator +(const DataContainer &other)
{
   // get done with "None"'s
   if(Type == DataTypeNone)
      return other;
   if(other.Type == DataTypeNone)
      return *this;

   if(Type == DataTypeString)
   {
      DataContainer result = *this;
      
      if(other.Type == DataTypeString)
         result.StringValue = result.StringValue + other.StringValue;
      else if(other.Type == DataTypeInteger || other.Type == DataTypeDouble)
      {
         stringstream str;

         if(other.Type == DataTypeInteger)
            str << other.IntegerValue;
         if(other.Type == DataTypeDouble)
            str << other.DoubleValue;

         result.StringValue = result.StringValue + str.str();
      }
      
      return result;
   }
   
   if((Type == DataTypeInteger || Type == DataTypeDouble) && other.Type == DataTypeString)
      return *this;
   
   if(Type == DataTypeInteger && other.Type == DataTypeInteger)
   {
      DataContainer result = *this;
      result.IntegerValue = IntegerValue + other.IntegerValue;
      return result;
   }
   if(Type == DataTypeInteger && other.Type == DataTypeDouble)
   {
      DataContainer result = other;
      result.DoubleValue = IntegerValue + other.DoubleValue;
      return result;
   }
   if(Type == DataTypeDouble && other.Type == DataTypeInteger)
   {
      DataContainer result = *this;
      result.DoubleValue = DoubleValue + other.IntegerValue;
      return result;
   }
   if(Type == DataTypeDouble && other.Type == DataTypeDouble)
   {
      DataContainer result = *this;
      result.DoubleValue = DoubleValue + other.DoubleValue;
      return result;
   }
   
   DataContainer Empty;
   Empty.Type = DataTypeNone;
   return Empty;
}
//---------------------------------------------------------------------------
DataContainer DataContainer::operator -(const DataContainer &other)
{
   if(Type == DataTypeNone || Type == DataTypeString)
      return *this;

   if((Type == DataTypeDouble || Type == DataTypeInteger)
      && other.Type != DataTypeDouble && other.Type != DataTypeInteger)
      return *this;

   if(Type == DataTypeDouble && other.Type == DataTypeDouble)
   {
      DataContainer result = *this;
      result.DoubleValue = DoubleValue - other.DoubleValue;
      return result;
   }
   if(Type == DataTypeDouble && other.Type == DataTypeInteger)
   {
      DataContainer result = *this;
      result.DoubleValue = DoubleValue - other.IntegerValue;
      return result;
   }
   if(Type == DataTypeInteger && other.Type == DataTypeDouble)
   {
      DataContainer result = other;
      result.DoubleValue = IntegerValue - other.DoubleValue;
      return result;
   }
   if(Type == DataTypeInteger && other.Type == DataTypeInteger)
   {
      DataContainer result = *this;
      result.IntegerValue = IntegerValue - other.IntegerValue;
      return result;
   }
   
   DataContainer Empty;
   Empty.Type = DataTypeNone;
   return Empty;
}
//---------------------------------------------------------------------------
DataContainer DataContainer::operator *(const DataContainer &other)
{
   // get done with "None" category
   if(Type == DataTypeNone || other.Type == DataTypeNone)
      return *this;

   // string * string = concatenation, string * integer = repetition, string * double = repetition (round down)
   if(Type == DataTypeString)
   {
      if(other.Type == DataTypeString)
      {
         DataContainer result = *this;
         result.StringValue = StringValue + other.StringValue;
         return result;
      }
      if(other.Type == DataTypeInteger || other.Type == DataTypeDouble)
      {
         long long Repetition = 1;
         if(other.Type == DataTypeInteger)
            Repetition = other.IntegerValue;
         if(other.Type == DataTypeDouble)
            Repetition = (int)other.DoubleValue;

         if(Repetition == 0)
         {
            DataContainer result;
            result = "";
            return result;
         }

         DataContainer result = StringValue;

         if(Repetition < 0)
         {
            reverse(result.StringValue.begin(), result.StringValue.end());
            Repetition = -Repetition;
         }

         string Temp = result.StringValue;
         for(int i = 1; i < Repetition; i++)
            result.StringValue = result.StringValue + Temp;

         return result;
      }
   }

   // numbers....usual stuff
   if(Type == DataTypeDouble && other.Type == DataTypeDouble)
   {
      DataContainer result = *this;
      result.DoubleValue = DoubleValue * other.DoubleValue;
      return result;
   }
   if(Type == DataTypeDouble && other.Type == DataTypeInteger)
   {
      DataContainer result = *this;
      result.DoubleValue = DoubleValue * other.IntegerValue;
      return result;
   }
   if(Type == DataTypeInteger && other.Type == DataTypeDouble)
   {
      DataContainer result = other;
      result.DoubleValue = IntegerValue * other.DoubleValue;
      return result;
   }
   if(Type == DataTypeInteger && other.Type == DataTypeInteger)
   {
      DataContainer result = *this;
      result.DoubleValue = IntegerValue * other.IntegerValue;
      return result;
   }

   DataContainer Empty;
   Empty.Type = DataTypeNone;
   return Empty;
}
//---------------------------------------------------------------------------
DataContainer DataContainer::operator /(const DataContainer &other)
{
   if(Type == DataTypeNone || Type == DataTypeString)
      return *this;

   if((Type == DataTypeDouble || Type == DataTypeInteger)
      && other.Type != DataTypeDouble && other.Type != DataTypeInteger)
      return *this;

   if(Type == DataTypeDouble && other.Type == DataTypeDouble)
   {
      DataContainer result = *this;
      result.DoubleValue = DoubleValue / other.DoubleValue;
      return result;
   }
   if(Type == DataTypeDouble && other.Type == DataTypeInteger)
   {
      DataContainer result = *this;
      result.DoubleValue = DoubleValue / other.IntegerValue;
      return result;
   }
   if(Type == DataTypeInteger && other.Type == DataTypeDouble)
   {
      DataContainer result = other;
      result.DoubleValue = IntegerValue / other.DoubleValue;
      return result;
   }
   if(Type == DataTypeInteger && other.Type == DataTypeInteger)
   {
      if(IntegerValue % other.IntegerValue == 0)
      {
         DataContainer result = *this;
         result.IntegerValue = IntegerValue / other.IntegerValue;
         return result;
      }
      else
      {
         DataContainer result;
         result.Type = DataTypeDouble;
         result.DoubleValue = (double)IntegerValue / other.IntegerValue;
         return result;
      }
   }
   
   DataContainer Empty;
   Empty.Type = DataTypeNone;
   return Empty;
}
//---------------------------------------------------------------------------
DataContainer DataContainer::operator -()
{
   if(Type == DataTypeNone)
      return *this;

   if(Type == DataTypeString)
   {
      DataContainer result = *this;
      reverse(result.StringValue.begin(), result.StringValue.end());
      return result;
   }
   if(Type == DataTypeDouble)
   {
      DataContainer result = *this;
      result.DoubleValue = -DoubleValue;
      return result;
   }
   if(Type == DataTypeInteger)
   {
      DataContainer result = *this;
      result.IntegerValue = -IntegerValue;
      return result;
   }
   
   DataContainer Empty;
   Empty.Type = DataTypeNone;
   return Empty;
}
//---------------------------------------------------------------------------
void DataContainer::SaveToStream(ostream &out)
{
   char TypeChar = (char)Type;

   out.put(TypeChar);
   
   if(Type == DataTypeNone)   // No data, do nothing
      return;
   
   if(Type == DataTypeString)
   {
      char DataSize[4];
      IntegerToChar4(StringValue.size(), DataSize);
      
      out.write(DataSize, 4);
      out.write(StringValue.c_str(), StringValue.size());
   }
   else if(Type == DataTypeDouble)
   {
      DataMessenger messenger;
      messenger.DoubleValue = DoubleValue;
      
      out.write(messenger.CharValue, 20);
   }
   else if(Type == DataTypeInteger)
   {
      DataMessenger messenger;
      messenger.LongLongValue = IntegerValue;
      
      out.write(messenger.CharValue, 20);
   }
}
//---------------------------------------------------------------------------
void DataContainer::LoadFromStream(istream &in)
{
   char TypeChar = '\0';

   in.get(TypeChar);
   
   Type = (int)TypeChar;

   if(Type == DataTypeNone)
      return;
      
   if(Type == DataTypeString)
   {
      char DataSize[4];
      in.read(DataSize, 4);

      long long Size = Char4ToInteger(DataSize);
      
      char *ch = new char[Size+1];
      in.read(ch, Size);
      ch[Size] = '\0';
      
      StringValue = ch;
      delete[] ch;
   }
   else if(Type == DataTypeDouble)
   {
      DataMessenger messenger;
      in.read(messenger.CharValue, 20);
      
      DoubleValue = messenger.DoubleValue;
   }
   else if(Type == DataTypeInteger)
   {
      DataMessenger messenger;
      in.read(messenger.CharValue, 20);
      
      IntegerValue = messenger.LongLongValue;
   }
}
//---------------------------------------------------------------------------
ostream &operator <<(ostream &out, DataContainer data)
{
   out << data.GetRepresentation();
   return out;
}
//---------------------------------------------------------------------------
istream &operator >>(istream &in, DataContainer &data)
{
   data.LoadFromStream(in);
   return in;
}
//---------------------------------------------------------------------------
void IntegerToChar4(int Number, char ChNumber[4])
{
   for(int i = 3; i >= 0; i--)
   {
      ChNumber[i] = (char)(Number % 128);
      Number = (Number - Number % 128) / 128;
   }
}
//---------------------------------------------------------------------------
long long Char4ToInteger(char ChNumber[4])
{
   long long Answer = 0;
   
   for(int i = 0; i < 4; i++)
      Answer = Answer * 128 + ChNumber[i];
      
   return Answer;
}
//---------------------------------------------------------------------------
#endif
