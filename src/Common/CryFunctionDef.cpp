
#include "CryFunctionDef.h"
#include "CryXML.h"
#include "ClassString.h"
#include "ClassFunction.h"

using namespace Crystal;
using namespace std;
void BuilderFunctionDef::SetDefaultValues()
{
		Return = "void";
		Param = "();\n";
		SetProperty("Value","");

String xml( "<CryString Value=\"\" />\
");
	xml.Replace("\\\"","\"");// replace \" with "  
CryXML x;
	x.LoadFrom(xml);
	x.SaveTo(*this);
}

FunctionDefList *BuilderFunctionDef::GetFunctions(const char *Type) const
{
// if a type has been defined and it's not this class, check subclasses for it
	if (Type && !IsA(Type))
	   return String::GetFunctions(Type);
	// otherwise get any functions in subclasses
	FunctionDefList *l = String::GetFunctions();
String s;
  s = "\\ class BuilderFunctionDef;";
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
bool ListFunctionDef::IteratedFunction(ListFunctionDef::IControl *Control,BuilderFunctionDef *Item)
{
//	switch(Control->State) ...
	return false;
}
BuilderFunctionDef* ListFunctionDef::AddOwned(BuilderFunctionDef * Item)
{
	List::AddOwned(Item);
	return Item;
}
BuilderFunctionDef* ListFunctionDef::Add(BuilderFunctionDef * Item)
{
	List::Add(Item);
	return Item;
}
void ListFunctionDef::SetDefaultValues()
{
		SetProperty("Values","");

String xml( "<CryList />\
");
	xml.Replace("\\\"","\"");// replace \" with "  
CryXML x;
	x.LoadFrom(xml);
	x.SaveTo(*this);
}
FunctionDefList *ListFunctionDef::GetFunctions(const char *Type) const
{
// if a type has been defined and it's not this class, check subclasses for it
	if (Type && !IsA(Type))
	   return List::GetFunctions(Type);
	// otherwise get any functions in subclasses
	FunctionDefList *l = List::GetFunctions();
String s;
  s = "\\ Class ListFunctionDef;";
	s+="const char * GetItems(CryString &Result) const;";
	s+="void SetItems(const char * _Items);";
	s+="virtual bool IteratedFunction(EmptyObject *Control,EmptyObject *Item);";
	s+="virtual bool IteratedFunction(EmptyObject *Control,EmptyObject *Item)";
  l->LoadFromString(s,";");
	return l;
}

