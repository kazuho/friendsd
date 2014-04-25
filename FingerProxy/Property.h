/**
 * Property.h
 */

#ifndef PROPERTY_H
#define PROPERTY_H

#include "hashmap.h"
#include "sstring.h"

class Property {
public:
	typedef Property MyType;
protected:
	sstring mValueStr;
	double mValueNum;
	bool mIsNum;
public:
	Property() : mValueStr(), mValueNum(0.0), mIsNum(false) {}
	Property(const sstring& inValue);
	bool IsNum() const { return mIsNum; }
	operator double() const { return mValueNum; }
	operator const sstring&() const { return mValueStr; }
};

bool operator==(const Property&, const Property&);

class PropertyMap : public hashmap<sstring, Property, sstring::hashcode> {
public:
	bool Init(const sstring& inFilename);
	bool IsDefined(const sstring& x) const { return find(x) != end(); }
	const Property& operator[](const sstring& x) const;
};

#endif
