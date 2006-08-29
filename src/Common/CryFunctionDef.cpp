
#include "CryFunctionDef.h"
#include "CryXML.h"
#include "ClassString.h"
#include "ClassFunction.h"

using namespace Crystal;
using namespace std;
void FunctionDef::SetDefaultValues()
{
		Return = "void";
		Param = "();\n";
		SetProperty("Value","");

CryString xml( "<CryString Value=\"\" />\
");
	xml.Replace("\\\"","\"");// replace \" with "  
CryXML x;
	x.LoadFrom(xml);
	x.SaveTo(*this);
}

CryFunctionDefList *FunctionDef::GetFunctions(const char *Type) const
{
// if a type has been defined and it's not this class, check subclasses for it
	if (Type && !IsA(Type))
	   return CryString::GetFunctions(Type);
	// otherwise get any functions in subclasses
	CryFunctionDefList *l = CryString::GetFunctions();
CryString s;
  s = "\\ class FunctionDef;";
	s+="const char * GetValue(CryString &Result) const;";
	s+="void SetValue(const char * _Value);";
	s+="const char *GetReturn(CryString &Result) const;";
	s+="void SetReturn(CryString _Return);";
	s+="const char *GetParam(CryString &Result) const;";
	s+="void SetParam(CryString _Param);";
  l->LoadFromString(s,";");
  return l;
}

//////////////////////
bool ListFunctionDef::IteratedFunction(ListFunctionDef::IControl *Control,FunctionDef *Item)
{
//	switch(Control->State) ...
	return false;
}
FunctionDef* ListFunctionDef::AddOwned(FunctionDef * Item)
{
	CryList::AddOwned(Item);
	return Item;
}
FunctionDef* ListFunctionDef::Add(FunctionDef * Item)
{
	CryList::Add(Item);
	return Item;
}
void ListFunctionDef::SetDefaultValues()
{
		SetProperty("Values","");

CryString xml( "<CryList />\
");
	xml.Replace("\\\"","\"");// replace \" with "  
CryXML x;
	x.LoadFrom(xml);
	x.SaveTo(*this);
}
CryFunctionDefList *ListFunctionDef::GetFunctions(const char *Type) const
{
// if a type has been defined and it's not this class, check subclasses for it
	if (Type && !IsA(Type))
	   return CryList::GetFunctions(Type);
	// otherwise get any functions in subclasses
	CryFunctionDefList *l = CryList::GetFunctions();
CryString s;
  s = "\\ Class ListFunctionDef;";
	s+="const char * GetItems(CryString &Result) const;";
	s+="void SetItems(const char * _Items);";
	s+="virtual bool IteratedFunction(EmptyObject *Control,EmptyObject *Item);";
	s+="virtual bool IteratedFunction(EmptyObject *Control,EmptyObject *Item)";
  l->LoadFromString(s,";");
	return l;
}

