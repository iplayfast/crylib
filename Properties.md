# Introduction #

In CryLib, properties are the internal data in a class that needs to be known in order to rebuild the class, to the same functionality as it is currently.   This does not imply that all data contained in the class is a property, rather only the data needed to recreate the class is.


# Details #

The main property functions are made up of:
```
virtual PropertyList * 	PropertyNames () const;
virtual int 	GetPropertyCount () const;
virtual bool 	HasProperty (const PropertyParser &PropertyName) const;
const char * 	GetProperty (const char *PropertyName, String &Result) const;
virtual const char * 	GetProperty (const PropertyParser &PropertyName, String &Result) const;
virtual bool 	HasProperty (const PropertyParser &PropertyName) const;
virtual bool 	SetProperty (const PropertyParser &PropertyName, const char *PropertyValue);
virtual bool 	SetPropertyAsObject (Property *Value);
```

PropertyParser is a class used to help with parsing out the property name, or optionally the property name indexed. (Eg Name or Name[3](3.md)).