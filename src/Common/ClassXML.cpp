/***************************************************************************
 *   Copyright (C) 2006 by Chris Bruner                                    *
 *   chris@Martha.crystal.com                                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <ctype.h>
#include "ClassXML.h"
#include "ClassArray.h"
#include "ClassProperty.h"
#include "ClassException.h"

//#include "CryFactory.h"
#define DEBUGGING
using namespace Crystal;


void XMLNode::SaveTo(Stream &ToStream) const
{
    for (int i=0;i<ToStream.Tag;i++)	//add some indenting
        ToStream.printf("  ");
    if (Type!="")
        ToStream.printf("<%s ",(const char *)Type);
    PropertyList::PropertyIterator *li = _Attributes->CreateIterator();
    if (li->GotoFirst())
    {
        do
        {
            String a;
//		const CryProperty *p = (const CryProperty *) li->Get();
            String Value;
            li->GetValue(Value);
            Value.Replace("&","&amp;");
			Value.Replace("<","&lt;");
            Value.Replace(">","&gt;");
            Value.Replace("\\","\\\\");
            Value.Replace("\"","&quot;");
            Value.Replace("\"","\\\"");
            Value.Replace("\n","\\n");
            if (Type!="")
                a.printf("%s=\"%s\" ",li->GetName()->AsPChar() ,Value.AsPChar());
            else
                a.printf("<%s>%s</%s>",li->GetName()->AsPChar(),Value.AsPChar(),li->GetName()->AsPChar());
            ToStream.printf("%s",a.AsPChar());

        }
        while (li->GotoNext());
    }
    _Attributes->DeleteIterator(li);
    if (SubNodes.HasItems())// now save any sub trees
    {
        ListIterator *lI = SubNodes.CreateIterator();
        XMLNode *sn;
        ToStream.printf(">\n");	// end of header (need footer)
        if (lI->GotoFirst())
            do
            {
                sn = (XMLNode *) lI->Get();
                ToStream.Tag++;	// indent more
                sn->SaveTo(ToStream);
                ToStream.Tag--;	// indent less
            }
            while (lI->GotoNext());
        SubNodes.DeleteIterator(lI);
        for (int i=0;i<ToStream.Tag;i++)	//add some indenting
            ToStream.printf("  ");
        ToStream.printf("</%s>\n",(const char *)Type);// footer
	}
	else
	{
		if (Type!="")
			ToStream.printf("/>\n");//end of header + footer
	}
}


/* Should load an xml file into an Object assuming that the Object can contain it.
First should check that the outer tag matches the ToObject if so, then
Add any attributes to the outer tag.
If Subnodes are present and TObject Is a container then
   Create the type, (with ToObject as parent), then recurse with the created object being the new ToObject
*/

int gint=0;
void XMLNode::SaveTo(Object &ToObject) const
{
::gint++;
	if (!ToObject.IsA(Type))
	{
		ToObject.IsA(Type);
		throw Exception(this,ExceptionSaveToDifferentType,"SaveTo Object of different Type");
	}
	PropertyList::PropertyIterator * pi = _Attributes->CreateIterator();
	if (pi->GotoFirst())
	{
		do
		{

			const char *c = pi->GetName()->AsPChar();

//        if (ToObject.HasProperty(p->GetName()))
			if (ToObject.CanHaveProperty(PropertyParser(c)))
			{
				String Result;
				pi->GetValue(Result);
				ToObject.SetProperty(c,Result.AsPChar());

			}
		}
		while (pi->GotoNext());
	}
	_Attributes->DeleteIterator(pi);
	List::ListIterator *li = SubNodes.CreateIterator();
	if (li->GotoFirst())
	{
		do
		{
			XMLNode *current = (XMLNode *) li->Get();
			Object *o = current->CreateObjectFromNode(&ToObject);
			if (o->IsA(CProperty))
			{
			Property *p = (Property *)o;
			const char *s = *p->GetName();
				if (ToObject.HasProperty(PropertyParser(s)))
				{
					ToObject.SetProperty(p);
					delete o;
					continue;
				}
			}
			if (ToObject.IsA(CProperty))
			{
			Property *p = (Property *)&ToObject;
				p->SetPropertyOwned(o);
				continue;
			}
			if (ToObject.IsContainer())
			{
			Container *cc = (Container *)&ToObject;
				cc->AddOwned(o);
//				current->SaveTo(*o);
/*				if (current->Type=="Property")
				{
					String Result;
//					ToObject.SetProperty(current->Type.AsPChar(),current->_Attributes(Result));
					PropertyList::PropertyIterator *ai = current->_Attributes->CreateIterator();
					if (ai->GotoFirst())
					{
						do
						{
							String Value;
							const char *Name = ai->GetName()->AsPChar();
							const char *_Value = ai->GetValue(Value);
							ToObject.SetProperty(Name,Value);
						}
						while (ai->GotoNext());
					}
					current->_Attributes->DeleteIterator(ai);

				}
				else
				{
					if (current->Type=="data")
					{
					String Result;
						PropertyList::PropertyIterator *ai = current->_Attributes->CreateIterator();
						if (ai->GotoFirst())
						{
							do
							{
								String Value;
								Container *c = (Container *) &ToObject;
								const char *Name = ai->GetName()->AsPChar();
								const char *_Value = ai->GetValue(Value);
								size_t Size;
								int Index;
								size_t pos;
								if (c->IsA(CSimpleArray))
								{
									Value.scanf("%d ",&Index);
									pos = Value.Pos(" ",Value.Tell())+1;
									if (pos<=Value.Length())
										Value.SeekFromStart(pos);
								}
								Value.scanf("%d ",&Size);
									pos = Value.Pos(" ",Value.Tell())+1;
									if (pos<=Value.Length())
										Value.SeekFromStart(pos);
//								char *Buffer = new char[Size];		// This might cause problems due to delete []Item needed for arrays.
								char *Buffer = (char *) ::operator new(Size); // so instead we create one item that's the size of Size
								for(size_t i =0;i<Size;i++)
								{
								int tmp;
									Value.scanf("%X ",&tmp);
									Buffer[i] = (char )tmp;
									pos = Value.Pos(" ",Value.Tell())+1;
									if (pos<=Value.Length())
										Value.SeekFromStart(pos);
								}
								if (c->IsA(CSimpleArray))
								{
								SimpleArray *s = (SimpleArray *)c;
									s->AddOwned((EmptyObject *)Buffer,Size,Index++);
								}
								else
									c->AddOwned((EmptyObject *)Buffer,Size);
							}
							while (ai->GotoNext());
						}
						current->_Attributes->DeleteIterator(ai);
					}
					else
					{
					Object *o =  // useful when debugging
						current->CreateObjectFromNode(&ToObject);
						Container *c = (Container *) &ToObject;
						c->AddOwned(o);
					}
				}
*/
			}
			else // The object isn't a container so the property must be one
			{
				if (ToObject.IsA(CProperty))
				{
				Property *p = (Property *)&ToObject;
					p->SetName(Name);
					p->SetValueOwned(o);
				}
				else
				{
					if (o->IsA(CProperty))
					{
					Property *p =(Property *)o;
					const char *c= *p->GetName();
						if (ToObject.CanHaveProperty(PropertyParser(c)))
						{
							ToObject.SetPropertyAsObject(p);
						}
						delete p;
					}
					else
					{
					delete o;
					throw Exception("Setting Unknown ObjectType as Property");
					}
				}
			}
		}
		while (li->GotoNext());
	}
	SubNodes.DeleteIterator(li);
}

// load this Node and subnodes from the stream
void XMLNode::LoadFrom(const Stream &FromStream)
{
	bool	 WithinQuote = false,GettingEndName = false,GettingEquals=false,GettingAttribute = false;
    bool GettingAttributeValue = false,FoundLetter = false,AtStart = true,Escaped = false,AddChar;

#ifdef DEBUGGING
	bool AtEnd = false,Done = false;
	String v;
#endif
	char Buff[2];
#ifdef DEBUGGING

	char LastChar;
#endif

	String LocName,Attribute,Value,EndName;
	AtStart = !GettingName; // normally GettingName is false on construction, but if being called recursivly
	// it will be true (becaues '<' was already found
	Buff[0] = Buff[1] = '\0';
	while (!FromStream.Eof())
	{
#ifdef DEBUGGING
		LastChar = Buff[0];
#endif

		FromStream.Read(&Buff[0],1);
#ifdef DEBUGGING
		Buff[1] = '\0';
		v += Buff;
#endif
		if (Escaped)
		{
			AddChar = true;
			if (Buff[0]=='>') // at end of definition?
			{
				if (GettingAttribute && (Attribute.Length()==0) && (Value.Length()==0) )
					return; // done!
				throw Exception(&FromStream,"Bad /> sequence near line ",FromStream.Tag);
			}
		}
		else
		{
			AddChar = false;
			switch (Buff[0])
			{
				case '\n':
                    FromStream.SetTag(FromStream.Tag + 1); // next line
                    break;
                case '=':
                    if (WithinQuote)
                        break;
                    if (GettingName)     // handles the case of = right after name
                    {
                        Type.SetValue(LocName);
                        SetName(LocName);
                        GettingName = false;
                        GettingAttribute = true;
                        GettingEquals = true;
                    }

                    if ((GettingAttribute && FoundLetter) || GettingEquals)
                    {
                        GettingAttributeValue = true;
                        GettingEquals = false;
                        GettingAttribute = false;
                        FoundLetter = false;
                        break;
                    }
                    throw Exception(&FromStream,"Misplaced = near line ",FromStream.Tag);
                case '>':
                    if (GettingEndName)
                    {
                        if (EndName!=LocName)
                            throw Exception(&FromStream,"Name End Name mismatch near line ",FromStream.Tag);
                        Type.SetValue(LocName);
                        return;
                    }
                    if (GettingAttribute || GettingName)   // no more attributes
                    {
                        if (GettingName)
                            SetName(LocName);
                        GettingName = false;
                        GettingAttribute = false;
                        GettingEquals = false;
                        GettingEndName = true;
                        break;
                    }
                    throw Exception(&FromStream,"Bad > near line ",FromStream.Tag);
                case ' ':
                    if (WithinQuote)
                        break;
                    if (GettingName || GettingAttribute)
                    {
                        if (FoundLetter)
                        {
                            if (GettingName)
                            {
                                Type.SetValue(LocName);
                                SetName(LocName);
                                GettingName = false;
                                GettingAttribute = true;
                                GettingEquals = true;
                            }
                            else
                                if (GettingAttribute)
                                {
                                    GettingAttribute = false;
                                    GettingEquals = true;
                                }
                        }
                        FoundLetter = false;
                    }
                    break;
                case '"':
                    WithinQuote = !WithinQuote;
                    if (GettingAttributeValue)
                    {
                        if (FoundLetter)
                        {
                            Value.Replace("&lt;","<");
                            Value.Replace("&gt;",">");
                            Value.Replace("\\\"","\"");
                            Value.Replace("&quot;","\"");
                            Value.Replace("&amp;","&");
                            Value.Replace("\\n","\n");
                            Value.Replace("\\\\","\\");

                            AddAttribute(Attribute,Value);
                            Attribute.Clear();
                            Value.Clear();
                            GettingAttribute=true;
                            GettingAttributeValue = false;
                            FoundLetter = false;
                        }
                        else
                            FoundLetter = true;
                        continue;
                    }
                    else
                        throw Exception(&FromStream,"Unexpected \" near line ",FromStream.Tag);
                    //                break;  // unreachable
                case '/':
                    if ((GettingName && FoundLetter) || GettingAttributeValue || GettingAttribute)
                    {
                        Escaped = true;
                        continue;
                    }
                    else
                        return;
                    //                break;  // unreachable
                case '<':
                    if (AtStart)
                    {
                        AtStart = false;
                        GettingName = true;
                    }
                    else
                        if (GettingEndName)
                            // found SubNode or end name
                        {
                            XMLNode *tempSubNodes = new XMLNode();
                            tempSubNodes->GettingName = true;
                            tempSubNodes->LoadFrom(FromStream);
                            if (strcmp(tempSubNodes->GetName(),"")==0)// subnode got </ and returned
                            {
                                delete tempSubNodes;
                            }
                            else
                            {
                                SubNodes.AddOwned(tempSubNodes);
                            }

                        }
                        else
                            throw Exception(&FromStream,"Bad < near line ",FromStream.Tag);
                    break;
                default:
                    if ((Buff[0]=='.') || (isalnum(Buff[0])) || (isprint(Buff[0])))
                        AddChar = true;
            }
        }
        if (Escaped)// if it got here the escape (slash) was cancelled, so we need to put the slash back in
        {
            if (GettingName)
                LocName.strcat("/");
            if (GettingEndName)
                EndName.strcat("/");
            if (GettingAttribute)
                Attribute.strcat("/");
            if (GettingAttributeValue)
                Value.strcat("/");
            Escaped = false;
        }

        if (AddChar || WithinQuote)
        {
            if (isalpha(Buff[0]) || GettingAttributeValue)
                FoundLetter = true;
            if (GettingName)
                LocName.strcat(Buff);
            if (GettingEndName)
                EndName.strcat(Buff);
            if (GettingAttribute)
                Attribute.strcat(Buff);
            if (GettingAttributeValue)
                Value.strcat(Buff);
        }

    }
}
/* TODO : Saving and loading a file will change the implementation */
void XMLNode::LoadFrom(const Object &FromObject)
{
	Type = FromObject.ChildClassName();
	PropertyList *pn = FromObject.PropertyNames(); // creates a list of the property names
	if (pn->Sortable())       /* TODO : Need to reimplment this for CryPropertyList */
		pn->Sort(0);
	if (FromObject.IsA(CContainer))
	{
	Container * c= (Container *) &FromObject;
		LoadFrom(*c,pn);
	}
	else
		LoadFrom(FromObject,pn);
	delete pn;
}

void XMLNode::LoadFrom(const Container &FromObject,PropertyList *pn)
{
String Result;
Iterator *i = pn->_CreateIterator();
int index = 0;
	try
	{
		if (i->GotoFirst())
		{
			do
			{
				XMLNode *n = new XMLNode("data");
				if (i->IsObject())
				{
					n->LoadFrom(*i->GetObject());
					AddSubNode(n);
				}
				else
				{
					String e;
					String t;
					n->Type = "";
					t.printf("data ");
						if (i->IsA(CSimpleArray))
							e.printf("%d ",index); // we need to include offset into array
						size_t size = i->GetItemSize();
						char *c = (char *)i->Get();
						e.printf("%d ",size);
						for (size_t ii=0;ii<size;ii++)
							e.printf("0x%X ",0xff & c[ii]);

					n->AddAttribute(t,e); // add property name and value
					n->Type = "data";
					AddSubNode(n);
				}
				index++;
			} while(i->GotoNext());
		}
	}
	catch (Exception &e)
	{
		FromObject.DeleteIterator(i);
		throw e;
	}
}

void XMLNode::LoadFrom(const Object &FromObject,PropertyList *pn)
{
String Result;
PropertyList::PropertyIterator *i = pn->CreateIterator();
	try
	{
		if (i->GotoFirst())
		{
			do
			{
				const String *item =  i->GetName();
				const char *c = *item;
				Result.Clear();
				//        Result.SeekFromStart(0);
				const char *v = FromObject.GetProperty(c,Result);
				if (v==NULL)
					v = "";
				if ((strcmp(v,"*")==0) && Result=="[]") // if result is different then returned then special case
				{
				PropertyParser pp(c);
					// a distinction needs to be made here. Is property the container, or FromObject the container?
					if (FromObject.GetIsPropertyContainer(pp))
					{ // property is the container
						Object *o = FromObject.GetCopyOfPropertyAsObject(pp);
						//                    n->AddAttribute("Property",c);
						if (o->IsA(CXMLNode))
							SubNodes.AddOwned(o);
						else
						{
							XMLNode *n = new XMLNode(c);
							n->LoadFrom(*o);
							delete o;
							if (n->Type.Pos(" ")>=0)
								throw Exception("Can't have space in Type");
							SubNodes.AddOwned(n);
						}
					}
                    else
                    {
                        if (FromObject.IsContainer())
						{
					 /*	String NewC;
							NewC = "*";
							NewC += c;
							FromObject.GetProperty(NewC,Result);
							XMLNode *n = new XMLNode(c);
							n->Type = "Property";
							n->AddAttribute(c,Result);
							SubNodes.AddOwned(n);
*/
							Container *cc = (Container *)&FromObject;
							Container::Iterator *I = cc->_CreateIterator();
							int index=0;// in case it's an array
							if (cc->GotoFirst(I))
								do
								{
									XMLNode *n = new XMLNode(c);
									//                        CryXMLNode *tSubNodes = new CryXMLNode(c);
									if (I->IsObject())
									{
									Object *o = (Object *)I->Get();
									n->LoadFrom(*o);
									}
									else
									{
										String e;
										String t;
//										cc->GetEleType(t);
										n->Type = "";
										t.printf("data ");
//										if (!cc->SaveAsText(I,e))  // try to save it to e
//										{
											if (cc->IsA(CSimpleArray))
												e.printf("%d ",index++); // we need to include offset into array
											size_t size = I->GetItemSize();
											char *c = (char *)I->Get();
											e.printf("%d ",size);
											for (size_t i=0;i<size;i++)
												e.printf("0x%X ",0xff & c[i]);
  //										}
										n->AddAttribute(t,e); // add property name and value
										n->Type = "data";
//										n->LoadFrom(e);
									}
									SubNodes.AddOwned(n);
								}
								while (cc->GotoNext(I));
							cc->DeleteIterator(I);

						}
						else    // The object isn't a container so the property must be one
						{
							XMLNode *n = new XMLNode(c);
							PropertyParser PropertyName(item->AsPChar());
							Property *o = FromObject.GetPropertyAsCryProperty(PropertyName);
							n->LoadFrom(*o);
							SubNodes.AddOwned(n);
							if (o->IsA(CArray))
							{
								((Array *) o)->Clear();
							}
							delete o;
						}
					}
				}
				else
				{
					AddAttribute(c,v); // add property name and value
				}
			}
			while (i->GotoNext());

		}
		pn->DeleteIterator(i);
	}
	catch (Exception &e)
	{
		pn->DeleteIterator(i);
		throw e;
	}
}

Object *XMLNode::CreateObjectFromNode(Object *Parent)
{
    //    Object *f = CryList::FactoryCreate(Type,Parent);
    Object *f;
    const char *c = Type;
    const char *ParentType;
    if (Parent)
        ParentType = Parent->ChildClassName();
    else
        ParentType = 0;
    String NameToCreate;
    const PropertyParser PropertyName(Type);
    if (Parent)
        f = Parent->Create(PropertyName,Parent);
    else
        f = Create(PropertyName,Parent);  // let virtual function handle derived classes
    if (f==0)
        f = Parent->Create(PropertyParser(c),Parent);
    if (f==0)
        throw Exception("Class %s failed construction", c);
    SaveTo(*f);
    return f;
    /*
    	// next load the object with attributes
    	const CryList::ListNode *n = Attributes->FirstNode();
    	while(n)
    	{
    		const CryProperty *p;
    		p = (const CryProperty *)n->Item;
    #ifdef DEBUGGING
    		const char *c = p->GetName();
    #endif
    		if (f->HasProperty(p->GetName()))
    			f->SetProperty(p->GetName(),p->GetValue());
    		n = n->Next;
    	}
    	const ListNode *ln = SubNodes.FirstNode();
    	while (ln)
    	{
    		if (f->IsContainer())
    		{
    			CryXMLNode *current = (CryXMLNode *)ln->Item;
    #ifdef DEBUGGING
    			Object *o =
    #endif
    			  current->CreateObjectFromNode(f);// Object Created goes into the continer "f"
    		}
    		else // The object isn't a container so the property must be one
    		{
    			CryXMLNode *current = (CryXMLNode *)ln->Item;
                   Object *o = current->CreateObjectFromNode(f);
                   if (o->IsA(TCryProperty))
                   {
                       f->SetPropertyAsObject((CryProperty *)o);
                       delete o;
                   }
                   else
                   {
                       delete o;
                       throw Exception("Setting Unknown ObjectType as Property");
                   }
               }
               ln = ln->Next;
           }
           return f;*/
}

void XMLNode::AddAttribute(const char *Name,const char *Value)
{
    _Attributes->AddProperty(Name,Value);
}


Object *XML::CreateObjectFromNode(Object *Parent)
{
    XMLNode *head = (XMLNode *) FirstNode()->Item;
    if (head)
        return head->CreateObjectFromNode(Parent);
    else
    {
        throw Exception("Creating Object from empty XML node");
        //    return 0;
    }
}

